#include "cart.h"
#include <QDebug>

Cart::Cart() {}

void Cart::loadFile(QString cartPath) {
    QFile file(cartPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw "Error while reading the cart file";

    std::string cur_del = "header";

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        const std::string lineStd = line.toStdString();
        const char * lineBuffer = lineStd.c_str();

        auto find = std::find(std::begin(delimiters), std::end(delimiters), lineBuffer);

        if (find != std::end(delimiters)) {
            cur_del = lineStd.substr(0, lineStd.size()-1);;
        } else {

            raw_data[cur_del] += lineStd;

        }
    }

    unsigned long gfx_size = raw_data["__gfx__"].size();

    raw_data["gfx_map"] = raw_data["__gfx__"];
    raw_data["gfx_map"].erase(0, gfx_size/2);
    raw_data["__gfx__"].erase(gfx_size/2, gfx_size);

    QImage spritesheet = getSpritesheetImage();
    getAllSprites(spritesheet);

    unsigned long sprite_id = 0;
    int curY = 0;
    int curX = 0;

    // sorry too lazy to make this look nice (it's just removing the new lines)
    raw_data["__map__"].erase(std::remove(raw_data["__map__"].begin(), raw_data["__map__"].end(), '\n'), raw_data["__map__"].end());
    raw_data["gfx_map"].erase(std::remove(raw_data["gfx_map"].begin(), raw_data["gfx_map"].end(), '\n'), raw_data["gfx_map"].end());

    for (unsigned int i = 0; i < raw_data["gfx_map"].length(); i+=2) {
        raw_data["__map__"].append(std::string() + raw_data["gfx_map"][i+1] + raw_data["gfx_map"][i]);
    }

    for (unsigned int i = 0; i < raw_data["__map__"].length(); i+=2) {
        std::string hex_string = std::string() + raw_data["__map__"][i] + raw_data["__map__"][i + 1];

        sprite_id = unsigned(std::strtol(hex_string.c_str(), nullptr, 16));

        map[curY][curX] = int(sprite_id);

        if (curX >= 127) {
            curY += 1;
            curX = 0;
        } else {
            curX += 1;
        }
    }
}

void Cart::saveFile(QString cartPath){
    QFile file(cartPath);
    if (file.open(QIODevice::ReadWrite)){
        QTextStream out(&file);

        out << "pico-8 cartridge // http://www.pico-8.com" << endl << "version 16" << endl;

        out << "__lua__" << endl;
        for(unsigned int i = 0;i < raw_data["__lua__"].length(); i++){
            out << raw_data["__lua__"][i];
        }

        out << "__gfx__" << endl;
        for(unsigned int i = 0; i < raw_data["__gfx__"].length(); i++){
            out << raw_data["__gfx__"][i];
        }
        //second half of the map
        for(unsigned int i = 0; i < 128*32; i++){
            if (i%64==0 && i>0){
                out << endl;
            }
            int tile = map[32+int(i/128)][i%128];
            QString hex;
            hex.setNum(tile, 16);
            if (hex.length()==1){
                hex="0"+hex;
            }
            QString reverse;
            reverse.append(hex.at(1));
            reverse.append(hex.at(0));
            out << reverse;
        }
        out << endl;

        out << "__label__" << endl;
        for(unsigned int i = 0; i < raw_data["__label__"].length(); i++){
            out << raw_data["__label__"][i];
        }
        out << "__gff__" << endl;
        for(unsigned int i = 0; i < raw_data["__gff__"].length(); i++){
            out << raw_data["__gff__"][i];
        }

        //first half of the map
        out << "__map__" << endl;
        for(unsigned int i = 0;i < 128*32; i++){
            if (i%128==0 && i>0){
                out << endl;
            }
            int tile = map[int(i/128)][i%128];
            QString hex;
            hex.setNum(tile, 16);
            if (hex.length()==1){
                hex="0"+hex;
            }
            out << hex;
        }
        out << endl;

        out << "__sfx__" << endl;
        for(unsigned int i = 0; i < raw_data["__sfx__"].length(); i++){
            out << raw_data["__sfx__"][i];
        }
        out << "__music__" << endl;
        for(unsigned int i = 0; i < raw_data["__music__"].length(); i++){
            out << raw_data["__music__"][i];
        }


        file.close();
    }
}

QImage Cart::getMapImage() {

    QImage mapImage = QImage(128*8, 64*8, QImage::Format_RGBA8888);

    mapImage.fill(Qt::black);

    for(int y=0;y<64;y++){
        for(int x=0;x<128;x++){
            QImage sprite = spritesImage[map[y][x]];

            QPoint destPos = QPoint(x*8, y*8);
            QPainter painter(&mapImage);
            painter.drawImage(destPos, sprite);
            painter.end();
        }
    }

    return mapImage;
}

QImage Cart::getLevelImage(int id){
    QImage levelImage = QImage(128, 128, QImage::Format_RGBA8888);

    levelImage.fill(Qt::black);

    int start_x=id%8*16;
    int start_y=int(id/8)*16;

    for(int y=start_y;y<start_y+16;y++){
        for(int x=start_x;x<start_x+16;x++){
            QImage sprite = spritesImage[map[y][x]];

            QPoint destPos = QPoint((x-start_x)*8, (y-start_y)*8);
            QPainter painter(&levelImage);
            painter.drawImage(destPos, sprite);
            painter.end();
        }
    }

    return levelImage;
}

void Cart::getAllSprites(QImage spritesheet) {

    int curX = 0;
    int curY = 0;

    std::vector<QImage> sprites;

    for (unsigned int i = 0; i < raw_data["__gfx__"].size(); i+=8) {

        curX = i % 128;

        curY = int(std::floor(i / 128) * 8);
        QRect rect(curX, curY, 8, 8);
        QImage spriteImage = spritesheet.copy(rect);
        sprites.push_back(spriteImage);
    }

    spritesImage = sprites;
}

QImage Cart::getSpritesheetImage() {

    QImage spritesheet(128, 64, QImage::Format_RGBA8888);

    int color_int = 0;
    int sheetX = 0;
    int sheetY = 0;
    char color;
    for (unsigned int i = 1; i < raw_data["__gfx__"].length(); i++) {
        color = raw_data["__gfx__"][i];
        color_int = int(std::strtol(&color, nullptr, 16));

        if ((sheetX) >= 128) {
            sheetY += 1;
            sheetX = 0;
        } else {
            sheetX += 1;
        }

        if (sheetX < 128) {
            spritesheet.setPixel(sheetX, sheetY, colors[color_int]);
        }


    }
    return spritesheet;
}

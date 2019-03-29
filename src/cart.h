#include <QString>
#include <QRgb>
#include <QFile>
#include <QImage>
#include <cmath>
#include "qpainter.h"
#include <algorithm>
#ifndef CART_H
#define CART_H


class Cart {

    public:
        Cart();

        void loadFile(QString);
        void getAllSprites(QImage);
        QImage getSpritesheetImage();
        QImage getMapImage();

        const std::string delimiters[7] = {"__lua__\n", "__gfx__\n", "__label__\n", "__gff__\n", "__map__\n", "__sfx__\n", "__music__\n"};

        const QRgb colors[16] = {qRgb(0, 0, 0), qRgb(29, 43, 83), qRgb(126, 37, 83), qRgb(0, 135, 81), qRgb(171, 82, 54),
                                 qRgb(95, 87, 79), qRgb(194, 195, 199), qRgb(255, 241, 232), qRgb(255, 0, 77), qRgb(255, 163, 0),
                                 qRgb(255, 236, 39), qRgb(0, 228, 54), qRgb(41, 173, 255), qRgb(131, 118, 156), qRgb(255, 119, 168),
                                 qRgb(255, 204, 170)};

        std::map<std::string, std::string> raw_data;
        std::vector<QImage> spritesImage;
        int map[64][128];

};

#endif

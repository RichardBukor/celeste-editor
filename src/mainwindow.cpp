#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), celesteCart()
{
    ui->setupUi(this);
    //connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveCart()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadCart()));

    //create grid
    grid = QImage(128, 128, QImage::Format_RGBA8888);
    grid.fill(qRgba(0,0,0,0));
    for(int y=0;y<128;y++){
        for(int x=0;x<128;x++){
            if (x%8==0 || y%8==0){
                grid.setPixel(x, y, qRgb(100, 100, 100));
            }
        }
    }
    selected_grid = QImage(8, 8, QImage::Format_RGBA8888);
    selected_grid.fill(qRgba(0,0,0,0));
    for(int y=0;y<8;y++){
        for(int x=0;x<8;x++){
            if (x==0 || y==0 || x==7 || y==7){
                selected_grid.setPixel(x, y, qRgb(255, 255, 255));
            }
        }
    }

    this->setMouseTracking(true);
}

void MainWindow::setImage(){
    QImage levelImage = celesteCart.getLevelImage(level);
    if(grid_enabled){
        QPainter painter(&levelImage);
        painter.drawImage(0, 0, grid);
        painter.drawImage(selected_grid_spot_x*8, selected_grid_spot_y*8, selected_grid);
        painter.end();
    }
    pixmap.convertFromImage(levelImage);
    ui->label->setPixmap(pixmap.scaledToWidth(ui->label->width()));
}

void MainWindow::adjust_size(int w, int h){
    int x=0;
    int y=0;
    if (w>h-0.75*ui->menuBar->height()){
        h-=ui->menuBar->height();
        x=(w-h)/2;
        w=h;
        y=0;
    }else{
        y=(h-w)/2-ui->menuBar->height();
        h=w+ui->menuBar->height();
    }
    ui->label->setGeometry(x,y,w,h);
    ui->label->setPixmap(pixmap.scaledToWidth(w));
}

void MainWindow::resizeEvent(QResizeEvent *event){
    if (has_loaded){
        adjust_size(event->size().width(),event->size().height());
    }
}

void MainWindow::saveCart() {
     // make this later
}

void MainWindow::loadCart() {

    QFileDialog fileDialog(this, (""),
                           QDir::homePath(),
                           ("Pico-8 Cart (*.p8)"));

    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (fileDialog.exec()) {

        QString fileName = fileDialog.selectedFiles().at(0);

        celesteCart.loadFile(fileName);

        setImage();

        has_loaded=true;

        adjust_size(size().width(),size().height());
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (has_loaded){
        if (event->key()==Qt::Key::Key_F || event->key()==Qt::Key::Key_S){
            if (event->key()==Qt::Key::Key_F){
                level++;
            }else if(event->key()==Qt::Key::Key_S){
                level--;
            }

            if (level < 0) {
                level = 31;
            }

            level = level % 32;
            setImage();
        }else if(event->key()==Qt::Key::Key_Q){
            grid_enabled=!grid_enabled;
            setImage();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(has_loaded){
        QPoint pos = QCursor::pos();
        int new_selected_grid_spot_x = 16*(pos.x()-this->x()-ui->label->x())/(ui->label->width());
        int new_selected_grid_spot_y = 16*(pos.y()-this->y()-ui->label->y())/(ui->label->height())-1;
        if (new_selected_grid_spot_x != selected_grid_spot_x || new_selected_grid_spot_y != selected_grid_spot_y){
            selected_grid_spot_x=new_selected_grid_spot_x;
            selected_grid_spot_y=new_selected_grid_spot_y;
            if (grid_enabled){
                setImage();
            }
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

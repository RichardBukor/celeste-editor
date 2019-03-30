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
}

void MainWindow::resizeEvent(QResizeEvent *event){
    if (has_loaded){
        int x=0;
        int y=0;
        int w=event->size().width();
        int h=event->size().height();
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
        std::vector<QImage> sprites = celesteCart.spritesImage;

        QImage map = celesteCart.getMapImage();
        QImage levelImage = celesteCart.getLevelImage(level);

        pixmap.convertFromImage(levelImage);
        ui->label->setPixmap(pixmap.scaledToWidth(ui->label->width()));

        has_loaded=true;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (has_loaded){
        if (event->key()==Qt::Key::Key_F){
            level++;
        }else if(event->key()==Qt::Key::Key_S){
            level--;
        }
        level=std::max(level,0);
        level=std::min(level,31);
        QImage levelImage = celesteCart.getLevelImage(level);
        pixmap.convertFromImage(levelImage);
        ui->label->setPixmap(pixmap.scaledToWidth(ui->label->width()));
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

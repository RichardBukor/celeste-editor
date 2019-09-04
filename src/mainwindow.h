#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cart.h"
#include <QWidget>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        void setImage();
        void adjust_size(int, int);
        void resizeEvent(QResizeEvent *);
        void keyPressEvent(QKeyEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);
        ~MainWindow();

    public slots:
        void saveCart();
        void loadCart();

    private:
        Ui::MainWindow *ui;
        Cart celesteCart;
        int level=0;
        bool has_loaded=false;
        QPixmap pixmap = QPixmap();
        QImage grid;
        bool grid_enabled=true;
        QImage selected_grid;
        int selected_grid_spot_x=0;
        int selected_grid_spot_y=0;
};

#endif

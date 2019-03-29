#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cart.h"
#include <QWidget>
#include <algorithm>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        void keyPressEvent(QKeyEvent *);
        ~MainWindow();

    public slots:
        void saveCart();
        void loadCart();

    private:
        Ui::MainWindow *ui;
        Cart celesteCart;
        int level=0;
        bool has_loaded=false;

};

#endif

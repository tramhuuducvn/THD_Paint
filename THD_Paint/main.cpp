#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
        QApplication application(argc, argv);
        MainWindow THDPaint;
        THDPaint.show();

        return application.exec();
}

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
        QApplication application(argc, argv);
        qDebug() << QStyleFactory::keys();
        application.setStyle(QStyleFactory::create("Fusion"));

        MainWindow THDPaint;
        THDPaint.show();

        return application.exec();
}

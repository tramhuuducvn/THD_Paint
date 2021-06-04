#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <PaintArea.h>

class MainWindow : public QMainWindow{
                Q_OBJECT

        private slots:
                void open();
                void save();
                void penColor();
                void penWidth();
                void about();

        private:
                PaintArea* paintArea;

                QMenu *fileMenu;
                QAction *openAct;
                QMenu *saveAsMenu;
                QList<QAction *> saveAsActs;
                QAction *exitAct;
                QAction *printAct;

                QMenu *optionMenu;
                QAction *penColorAct;
                QAction *penWidthAct;
                QAction *clearScreenAct;

                QMenu *helpMenu;
                QAction *aboutAct;
                QAction *aboutQtAct;

                void createActions();
                void createMenus();
                bool maybeSave();
                bool saveFile(const QByteArray &fileFormat);


        protected:
                void closeEvent(QCloseEvent *event) override;

        public :
                MainWindow();
};



#endif //!MAINWINDOW_H

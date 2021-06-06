#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QtWidgets>
#include <QMainWindow>
#include <PaintArea.h>

class MainWindow : public QMainWindow{
                Q_OBJECT

        private slots:
                void open();
                void save();
                void penColor();
                void penWidth();
                void penWidthCb(int w);
                void about();
                void undo();
                void redo();

                void pencil();
                void eraser();
                void rectangle();
                void ellipse();

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

                QToolBar *toolbar;
                QPushButton *colorButton;
                QComboBox *widthCb;

                QPushButton *pencilButton;
                QPushButton *eraserButton;
                QPushButton *rectangleButton;
                QPushButton *ellipseButton;

                QAction *undoAct;
                QAction *redoAct;

                void createActions();
                void createMenus();
                void createToolBar();

                bool maybeSave();
                bool saveFile(const QByteArray &fileFormat);


        protected:
                void closeEvent(QCloseEvent *event) override;
//                void mouseMoveEvent(QMouseEvent* event) override;

        public :
                MainWindow();
                ~MainWindow();
};



#endif //!MAINWINDOW_H

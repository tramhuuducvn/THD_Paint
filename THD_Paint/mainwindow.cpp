#include <QtWidgets>

#include "mainwindow.h"
#include "PaintArea.h"

QColor c = QColor(239, 204, 153);
QColor w = QColor(255, 255, 255);

MainWindow::MainWindow(){
        paintArea = new PaintArea;
        setCentralWidget(paintArea);

        QPixmap pix = QPixmap("img/pencil_icon.png");
        this->setCursor(QCursor(pix, 0, pix.height()));

        createActions();
        createMenus();
        createToolBar();

        this->pencilButton->setPalette(QPalette(c));
        this->setWindowIcon(QIcon("img/paint_icon.png"));
        this->setWindowTitle(tr("THD Paint Application"));
        this->resize(700, 700);
}

void MainWindow::closeEvent(QCloseEvent *event) {
        if(maybeSave()) {
                event->accept();
        }
        else {
                event->ignore();
        }
}

// SLOT event ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::open() {
        if(maybeSave()){
                QString fileName = QFileDialog::getOpenFileName(this,  tr("Open File"), QDir::currentPath());

                if (!fileName.isEmpty()) {
                            paintArea->openImage(fileName);
                }
        }

}

void MainWindow::save(){
        QAction *action = qobject_cast<QAction *>(sender());
        QByteArray fileFormat = action->data().toByteArray();
         saveFile(fileFormat);
}

void MainWindow::penColor(){
        QColor newColor = QColorDialog::getColor(paintArea->getPenColor());

        if (newColor.isValid()) {
                paintArea->setPenColor(newColor);
                this->colorButton->setPalette(QPalette(newColor));
        }
}


void MainWindow::penWidth(){
        bool ok = false;
        int newWidth = QInputDialog::getInt(this, tr("Customize Paint Width"), tr("Select pen width:"), paintArea->getPenWidth(), 1, 50, 1, &ok);

        if(ok == true) {
                paintArea->setPenWidth(newWidth);
                this->widthCb->setCurrentIndex(newWidth - 1);
        }
}

void MainWindow::penWidthCb(int w){
        this->paintArea->setPenWidth(w + 1);
}

void MainWindow::pencil() {
        QPixmap pix = QPixmap("img/pencil_icon.png");
        this->setCursor(QCursor(pix, 0, pix.height()));
        this->paintArea->setPenColor(this->colorButton->palette().button().color());
        //-------------------------------------------------------------------------------------------------------------------
        this->pencilButton->setPalette(QPalette(c));
        this->eraserButton->setPalette(QPalette(w));
        this->rectangleButton->setPalette(QPalette(w));
        this->ellipseButton->setPalette(QPalette(w));
}

void MainWindow::eraser() {
        QPixmap pix = QPixmap("img/eraser_icon.png");
        this->setCursor(QCursor(pix, 0, pix.height()));
        this->paintArea->setPenColor(w);
        //-------------------------------------------------------------------------------------------------------------------
        this->pencilButton->setPalette(QPalette(w));
        this->eraserButton->setPalette(QPalette(c));
        this->rectangleButton->setPalette(QPalette(w));
        this->ellipseButton->setPalette(QPalette(w));
}

void MainWindow::rectangle() {
//        QPixmap pix = QPixmap("img/rectangle_icon.png");
        this->setCursor(QCursor(Qt::CrossCursor));
        this->paintArea->setPenColor(this->colorButton->palette().button().color());
        //-------------------------------------------------------------------------------------------------------------------
        this->pencilButton->setPalette(QPalette(w));
        this->eraserButton->setPalette(QPalette(w));
        this->rectangleButton->setPalette(QPalette(c));
        this->ellipseButton->setPalette(QPalette(w));
}

void MainWindow::ellipse() {
//        QPixmap pix = QPixmap("img/ellipse_icon.png");
        this->setCursor(QCursor(Qt::CrossCursor));
        this->paintArea->setPenColor(this->colorButton->palette().button().color());
        //-------------------------------------------------------------------------------------------------------------------
        this->pencilButton->setPalette(QPalette(w));
        this->eraserButton->setPalette(QPalette(w));
        this->rectangleButton->setPalette(QPalette(w));
        this->ellipseButton->setPalette(QPalette(c));
}

void MainWindow::about(){
        QMessageBox::about(this, tr("About me"), tr("<p><center> 19120484_Trầm Hữu Đức </center></p>"));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void MainWindow::createActions() {
        openAct = new QAction(tr("Open..."), this);
        openAct->setShortcuts(QKeySequence::Open);

        connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

        foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
                QString text = tr("%1...").arg(QString(format).toUpper());
                QAction *action = new QAction(text, this);

                action->setData(format);
                connect(action, SIGNAL(triggered()), this, SLOT(save()));
                saveAsActs.append(action);
        }

        printAct = new QAction(tr("Print..."), this);
        connect(printAct, SIGNAL(triggered()), paintArea, SLOT(print()));

        exitAct = new QAction(tr("Exit"), this);
        exitAct->setShortcuts(QKeySequence::Quit);
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

        penColorAct = new QAction(tr("Pen Color..."), this);
        connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

        penWidthAct = new QAction(tr("Pen Width..."), this);
        connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

        clearScreenAct = new QAction(tr("Clear Screen"), this);
        clearScreenAct->setShortcut(tr("Ctrl+L"));
        connect(clearScreenAct, SIGNAL(triggered()), paintArea, SLOT(clearImage()));

        aboutAct = new QAction(tr("About"), this);
        connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

        aboutQtAct = new QAction(tr("About Qt"), this);
        connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


void MainWindow::createMenus() {
        saveAsMenu = new QMenu(tr("Save As"), this);
        foreach (QAction *action, saveAsActs) {
        saveAsMenu->addAction(action);
        }
        // File Menu
        fileMenu = new QMenu(tr("File"), this);
        fileMenu->addAction(openAct);
        fileMenu->addMenu(saveAsMenu);
        fileMenu->addAction(printAct);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAct);
        // Option Menu
        optionMenu = new QMenu(tr("Options"), this);
        optionMenu->addAction(penColorAct);
        optionMenu->addAction(penWidthAct);
        optionMenu->addSeparator();
        optionMenu->addAction(clearScreenAct);
        // Help Menu
        helpMenu = new QMenu(tr("Help"), this);
        helpMenu->addAction(aboutAct);
        helpMenu->addAction(aboutQtAct);

        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(optionMenu);
        menuBar()->addMenu(helpMenu);
}

void MainWindow::createToolBar(){
        this->toolbar = new QToolBar("Main Toolbar", NULL);
        //---------------------------------------------------------------------------------------------------------------------
        this->colorButton = new QPushButton("color");
        this->toolbar->addWidget(this->colorButton);
        this->colorButton->setAutoFillBackground(true);
        this->colorButton->setPalette(QPalette(this->paintArea->getPenColor()));
        connect(colorButton, &QPushButton::clicked, this, &MainWindow::penColor);
        colorButton->update();

        this->widthCb = new QComboBox();
        for(int i = 1; i < 51; ++i) {
                this->widthCb->addItem(QString::number(i));
        }
        this->widthCb->setCurrentIndex(this->paintArea->getPenWidth() - 1);
        connect(this->widthCb, SIGNAL(activated(int))  ,this, SLOT(penWidthCb(int)));
        this->toolbar->addWidget(this->widthCb);

        this->toolbar->addSeparator();//----------------------------------------------------------------------
        this->toolbar->addSeparator();//----------------------------------------------------------------------

        this->pencilButton = new QPushButton(QIcon("img/pencil_icon.png"), "pencil", NULL);
        this->toolbar->addWidget(this->pencilButton);
        connect(this->pencilButton, &QPushButton::clicked, this, &MainWindow::pencil);

        this->eraserButton = new QPushButton(QIcon("img/eraser_icon.png"), "eraser", NULL);
        this->toolbar->addWidget(this->eraserButton);
        connect(this->eraserButton, &QPushButton::clicked, this, &MainWindow::eraser);

        this->rectangleButton = new QPushButton(QIcon("img/rectangle_icon.png"), "rectangle", NULL);
        this->toolbar->addWidget(this->rectangleButton);
        connect(this->rectangleButton, &QPushButton::clicked, this, &MainWindow::rectangle);

        this->ellipseButton = new QPushButton(QIcon("img/ellipse_icon.png"), "ellipse", NULL);
        this->toolbar->addWidget(this->ellipseButton);
        connect(this->ellipseButton, &QPushButton::clicked, this, &MainWindow::ellipse);


        //---------------------------------------------------------------------------------------------------------------------
        this->addToolBar(Qt::ToolBarArea::TopToolBarArea, this->toolbar);
        this->toolbar->show();
}


bool MainWindow::maybeSave() {
        if (paintArea->isModified()) {
                QMessageBox::StandardButton ret;

                ret = QMessageBox::warning(this, tr("Scribble"), tr("The image has been modified.\n Do you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                if (ret == QMessageBox::Save) {
                        return saveFile("png");
                }
                else if (ret == QMessageBox::Cancel) {
                        return false;
                }
        }
        return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat) {
        QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath, tr("%1 Files (*.%2);;All Files (*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));
        if (fileName.isEmpty()) {
                return false;
        }
        else {
                return paintArea->saveImage(fileName, fileFormat.constData());
        }
}




















#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "PaintArea.h"

int min(int x, int y){
        return (x < y) ? x : y;
}

int max(int x, int y){
        return (x > y) ? x : y;
}


PaintArea::PaintArea(QWidget *parent) : QWidget(parent) {
        setAttribute(Qt::WA_StaticContents);
        this->setFocusPolicy(Qt::StrongFocus);

        modified = false;
        isdrawing =false;

        holdingW = false;
        holdingS = false;
        holdingA = false;
        holdingD = false;

        isdrawLine = true;
        isdrawRect = false;
        isdrawEllipse = false;

        penWidth = 7;
        penColor = Qt::green;
}

void PaintArea::drawLine(const QPoint &endPoint){
        QPoint target = QPoint(endPoint.x(), endPoint.y());

        if(this->holdingW){
                target.setY(curPoint.y());
        }        
        else if(this->holdingS){
                target.setX(curPoint.x());
        }
        else if(this->holdingA){
                int x = ( target.x() - target.y() + originPoint.x() + originPoint.y() ) / 2;
                int y = ( -target.x() + target.y() + originPoint.x() + originPoint.y() ) / 2 ;
                target.setX(x);
                target.setY(y);
        }
        else if(this->holdingD){
                int x = ( target.x() + target.y() + originPoint.x() - originPoint.y() ) / 2;
                int y = ( target.x() + target.y() - originPoint.x() + originPoint.y() ) / 2 ;
                target.setX(x);
                target.setY(y);
        }

        QPainter painter(&image);
        painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(curPoint, target);
        modified = true;
        int rad = penWidth/2 + 2;
        update(QRect(curPoint, target).normalized().adjusted(-rad, -rad, rad, rad));
        curPoint = target;
}

void PaintArea::drawRect(const QPoint &endPoint){
        QPoint target = QPoint(endPoint.x(), endPoint.y());

        if(this->holdingW){
                target.setY(curPoint.y());
        }
        else if(this->holdingS){
                target.setX(curPoint.x());
        }
        else if(this->holdingA){
                int x = ( target.x() - target.y() + originPoint.x() + originPoint.y() ) / 2;
                int y = ( -target.x() + target.y() + originPoint.x() + originPoint.y() ) / 2 ;
                target.setX(x);
                target.setY(y);
        }
        else if(this->holdingD){
                int x = ( target.x() + target.y() + originPoint.x() - originPoint.y() ) / 2;
                int y = ( target.x() + target.y() - originPoint.x() + originPoint.y() ) / 2 ;
                target.setX(x);
                target.setY(y);
        }

        QPainter painter(&image);
        painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(curPoint, target);
        modified = true;
        int rad = penWidth/2 + 2;
        update(QRect(curPoint, target).normalized().adjusted(-rad, -rad, rad, rad));
        curPoint = target;
}

void PaintArea::drawEllipse(const QPoint &endPoint){
        QPoint target = QPoint(endPoint.x(), endPoint.y());

        if(this->holdingW){
                target.setY(curPoint.y());
        }
        else if(this->holdingS){
                target.setX(curPoint.x());
        }
        else if(this->holdingA){
                int x = ( target.x() - target.y() + originPoint.x() + originPoint.y() ) / 2;
                int y = ( -target.x() + target.y() + originPoint.x() + originPoint.y() ) / 2 ;
                target.setX(x);
                target.setY(y);
        }
        else if(this->holdingD){
                int x = ( target.x() + target.y() + originPoint.x() - originPoint.y() ) / 2;
                int y = ( target.x() + target.y() - originPoint.x() + originPoint.y() ) / 2 ;
                target.setX(x);
                target.setY(y);
        }

        QPainter painter(&image);
        painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(curPoint, target);
        modified = true;
        int rad = penWidth/2 + 2;
        update(QRect(curPoint, target).normalized().adjusted(-rad, -rad, rad, rad));
        curPoint = target;
}

void PaintArea::resizeImage(QImage *image, const QSize &size){
        if(image->size() == size){
                return;
        }

        QImage newImage(size, QImage::Format_RGB32);
        newImage.fill(qRgb(255, 255, 255));
        QPainter painter(&newImage);
        painter.drawImage(QPoint(0, 0), *image);
        *image = newImage;
}

void PaintArea::mousePressEvent(QMouseEvent *event){
        this->originPoint = event->pos();

        if(event->button() == Qt::LeftButton){
                this->curPoint = event->pos();
                this->isdrawing = true;

                if(this->isdrawLine == false){
                        this->selectionTool = new  SelectionTool(QRubberBand::Rectangle, this);
                        this->selectionTool->setGeometry(QRect(this->originPoint, QSize()));
                        this->selectionTool->show();
                }
        }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event){
        if((event->buttons() & Qt::LeftButton) && isdrawing){
                if(this->isdrawLine){
                        this->drawLine(event->pos());
                }
                else{
                        this->selectionTool->setGeometry(QRect(this->originPoint, event->pos()).normalized());
                }
        }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event){
        if(event->button() == Qt::LeftButton && isdrawing){
                if(this->isdrawLine){
                        this->drawLine(event->pos());
                 }
                else if(this->isdrawRect){
                        this->drawRect(event->pos());
                }
                if(this->isdrawEllipse){
                        this->drawEllipse(event->pos());
                }
                if(this->isdrawLine == false){
                        this->selectionTool->close();
                        this->selectionTool = NULL;
                }
                this->isdrawing  = false;
        }
}

void PaintArea::keyPressEvent(QKeyEvent *event){
        if(event->key() == Qt::Key_W){
                holdingW = true;
        }
        else if(event->key() == Qt::Key_S){
                 holdingS = true;
        }
        else if(event->key() == Qt::Key_A){
                 holdingA = true;
        }
        else if(event->key() == Qt::Key_D){
                 holdingD = true;
        }
}


void PaintArea::keyReleaseEvent(QKeyEvent *event){
        if(event->key() == Qt::Key_W){
                holdingW = false;
        }
        else if(event->key() == Qt::Key_S){
                 holdingS = false;
        }
        else if(event->key() == Qt::Key_A){
                 holdingA = false;
        }
        else if(event->key() == Qt::Key_D){
                 holdingD = false;
        }
}


void PaintArea::paintEvent(QPaintEvent *event){
        QPainter painter(this);
        QRect drewRect = event->rect();
        painter.drawImage(drewRect, image, drewRect);
}

void PaintArea::resizeEvent(QResizeEvent *event){
        if(width() > image.width() || height() > image.height()){
                int newWidth = qMax(width() + 128, image.width());
                int newHeight = qMax(height() + 128, image.height());
                resizeImage(&image, QSize(newWidth, newHeight));
                update();
        }
        QWidget::resizeEvent(event);
}

void PaintArea::clearImage(){
        image.fill(qRgb(255, 255, 255));
        modified = false;
        update();
}

void PaintArea::print(){
        #if QT_CONFIG(printdialog)
                QPrinter printer(QPrinter::HighResolution);

                QPrintDialog printDialog(&printer, this);
                if (printDialog.exec() == QDialog::Accepted) {
                        QPainter painter(&printer);
                        QRect rect = painter.viewport();
                        QSize size = image.size();
                        size.scale(rect.size(), Qt::KeepAspectRatio);
                        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                        painter.setWindow(image.rect());
                        painter.drawImage(0, 0, image);
                }
        #endif
}

void PaintArea::setPenColor(const QColor &color){
        this->penColor = color;
}

void PaintArea::setPenWidth(int width){
        this->penWidth = width;
}

bool PaintArea::isModified() const {
        return this->modified;
 }

QColor PaintArea::getPenColor() const {
        return this->penColor;
}

int PaintArea::getPenWidth() const {
        return this->penWidth;
}

bool PaintArea::isDrawLine() const {
    return isdrawLine;
}

bool PaintArea::isDrawRect() const {
    return isdrawRect;
}

bool PaintArea::isDrawEllipse() const {
    return isdrawEllipse;
}

void PaintArea::setDrawLine(bool b){
    this->isdrawLine = b;
}

void PaintArea::setDrawRect(bool b){
    this->isdrawRect = b;
}

void PaintArea::setDrawEllipse(bool b){
    this->isdrawEllipse = b;
}

bool PaintArea::openImage(const QString &fileName){
        QImage loadedImage;

        if(!loadedImage.load(fileName)) {
                return false;
        }

        QSize newSize = loadedImage.size().expandedTo(this->size());
        resizeImage(&loadedImage, newSize);
        image = loadedImage;
        modified = false;
        update();
        return true;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat){
        QImage visibleImage = image;
        resizeImage(&visibleImage, size());

        if(visibleImage.save(fileName, fileFormat)){
                modified = false;
                return true;
        }
        else{
                return false;
        }
}



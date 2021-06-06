#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "PaintArea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent) {
        setAttribute(Qt::WA_StaticContents);
        this->setFocusPolicy(Qt::StrongFocus);

        modified = false;
        isdrawing =false;
        shifting = false;
        penWidth = 7;
        penColor = Qt::green;
}

void PaintArea::drawLine(const QPoint &endPoint){
        QPoint target = QPoint(endPoint.x(), endPoint.y());
        if(this->shifting){
                target.setY(curPoint.y());
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
        if(event->button() == Qt::LeftButton){
                this->curPoint = event->pos();
                this->isdrawing = true;
        }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event){
        if((event->buttons() & Qt::LeftButton) && isdrawing){
                this->drawLine(event->pos());
        }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event){
        if(event->button() == Qt::LeftButton && isdrawing){
                this->drawLine(event->pos());
                this->isdrawing  = false;
        }
}

void PaintArea::keyPressEvent(QKeyEvent *event){
        if(event->key() == Qt::Key_Shift){
                shifting = true;              
        }
}


void PaintArea::keyReleaseEvent(QKeyEvent *event){
        if(event->key() == Qt::Key_Shift){
                shifting = false;
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



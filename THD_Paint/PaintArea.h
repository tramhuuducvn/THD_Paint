#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QtWidgets>
#include <SelectionTool.h>

class PaintArea : public QWidget {
                Q_OBJECT

        private:
                bool modified;
                bool isdrawing;
                bool holdingW;
                bool holdingS;
                bool holdingA;
                bool holdingD;
                bool shifting;

                bool isdrawLine;
                bool isdrawRect;
                bool isdrawEllipse;

                QColor penColor;
                int penWidth;                
                QImage image;
                QPoint curPoint;
                QPoint originPoint;
                QPoint target;
                SelectionTool *selectionTool;

                void drawLine(const QPoint& endPoint);
                void drawRect(const QPoint &beginPoint, const QPoint& endPoint);
                void drawEllipse(const QPoint &beginPoint, const QPoint& endPoint);
                void resizeImage(QImage *image, const QSize &size);

        protected:
                void mousePressEvent(QMouseEvent *event) override;
                void mouseMoveEvent(QMouseEvent *event) override;
                void mouseReleaseEvent(QMouseEvent* event) override;

                void keyPressEvent(QKeyEvent *event) override;
                void keyReleaseEvent(QKeyEvent *event) override;

                void paintEvent(QPaintEvent *event) override;
                void resizeEvent(QResizeEvent *event) override;

        public slots:
                void clearImage();
                void print();

        public:
//                PaintArea();
                PaintArea(QWidget * parent = 0);
                bool openImage(const QString &fileName);
                bool saveImage(const QString &fileName, const char *fileFormat);
                void setPenColor(const QColor &color);
                void setPenWidth(int width);

                bool isModified() const;
                QColor getPenColor() const;
                int getPenWidth() const;

                bool isDrawLine() const;
                bool isDrawRect() const;
                bool isDrawEllipse() const;

                void setDrawLine(bool b);
                void setDrawRect(bool b);
                void setDrawEllipse(bool b);

};

#endif // PAINTAREA_H

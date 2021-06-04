#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class PaintArea : public QWidget {
                Q_OBJECT

        private:
                bool modified;
                QColor penColor;
                bool penWidth;
                bool isdrawing;
                QImage image;
                QPoint curPoint;

                void drawLine(const QPoint& endPoint);
                void resizeImage(QImage *image, const QSize &size);

        protected:
                void mousePressEvent(QMouseEvent *event) override;
                void mouseMoveEvent(QMouseEvent *event) override;
                void mouseReleaseEvent(QMouseEvent* event) override;
                void paintEvent(QPaintEvent *event) override;
                void resizeEvent(QResizeEvent *event) override;

        public slots:
                void clearImage();
                void print();

        public:
                PaintArea(QWidget * parent = 0);
                bool openImage(const QString &fileName);
                bool saveImage(const QString &fileName, const char *fileFormat);
                void setPenColor(const QColor &color);
                void setPenWidth(int width);

                bool isModified() const;
                QColor getPenColor() const;
                int getPenWidth() const;
};

#endif // PAINTAREA_H

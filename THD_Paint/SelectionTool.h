#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QtWidgets>

class SelectionTool : public QRubberBand {
                Q_OBJECT
        protected:
//                void paintEvent(QPaintEvent *event) override;
        public:
                explicit SelectionTool(Shape s, QWidget *parent = 0);
};

#endif // SELECTIONTOOL_H

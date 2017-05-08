#ifndef CANVAS_HXX
#define CANVAS_HXX

#include <QWidget>
#include <QString>
#include <QPainter>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QtMath>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = Q_NULLPTR);

public slots:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
};

#endif // CANVAS_HXX

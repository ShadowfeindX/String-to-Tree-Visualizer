#include "canvas.hxx"

int cols;
Canvas * canvas = nullptr;
QPainter *right = new QPainter, *left = new QPainter;

int index(int x, int y) {
  if (x < 0 || y < 0 || x >= cols || y >= cols)
    return -1;
  return x + y * cols;
}

float map(float value,
          float istart,
          float istop,
          float ostart,
          float ostop) {
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

template <typename T>
void constrain(T & value,
               T min,
               T max) {
  value = value < min ? min : value > max ? max : value;
}

QString str;
QPointF begin(0,0), end(0, 100);
enum Active {
  Left,
  Right,
  Both
} status;

QDialog * menu;

void generate() {
  str = str.replace("A","AAEOEAIAIAUIOIAEAEAU");
  end.ry() *= .5;
}
void reset() {
  str = "A";
  end.ry() = 100;
}

void setup()
{
  canvas->setFixedSize(400,400);
  str = "A";
  status = Both;
  menu = new QDialog(canvas);
  menu->setLayout(new QHBoxLayout);
  QPushButton * button = new QPushButton("Reset");
  QGroupBox * box = new QGroupBox("Style");
  box->setLayout(new QHBoxLayout);
  auto * radio = new QRadioButton("Left");
  canvas->connect(radio, &QRadioButton::pressed, []{status = Left;});
  box->layout()->addWidget(radio);
  radio = new QRadioButton("Right");
  canvas->connect(radio, &QRadioButton::pressed, []{status = Right;});
  box->layout()->addWidget(radio);
  radio = new QRadioButton("Both");
  radio->setChecked(true);
  canvas->connect(radio, &QRadioButton::pressed, []{status = Both;});
  box->layout()->addWidget(radio);
  QLineEdit * line = new QLineEdit(str);
  QObject::connect(line, &QLineEdit::textChanged, [](const QString & s){
    reset();
    str = s;
  });
  QObject::connect(button, &QPushButton::clicked, [line]{
    reset();
    str = line->text();
  });
  menu->layout()->addWidget(line);
  menu->layout()->addWidget(button);
  menu->layout()->addWidget(box);
  menu->show();
}

void draw()
{
  Qt::GlobalColor color = Qt::darkGreen;

  if (status == Right || status == Both) {
  right->begin(canvas);
  right->translate(canvas->width()/2, canvas->height());
  for (int i = 0; i < str.length(); ++i) {
    switch(str.at(i).unicode()) {
      case 'A': {
          right->setPen(color = color == Qt::darkGreen ? Qt::darkYellow : Qt::darkGreen);
          right->drawLine(begin, begin-end);
          right->translate(begin-end);
        } break;
      case 'E': {
          right->rotate(qRadiansToDegrees(M_PI/6));
        } break;
      case 'I': {
          right->rotate(qRadiansToDegrees(-M_PI/6));
        } break;
      case 'O': {
          right->save();
        } break;
      case 'U': {
          right->restore();
        } break;
    }
  }
  right->end();
  } if (status == Left || status == Both) {
  left->begin(canvas);
  left->translate(canvas->width()/2, canvas->height());
  for (int i = 0; i < str.length(); ++i) {
    switch(str.at(i).unicode()) {
      case 'A': {
          left->setPen(color = color == Qt::darkGreen ? Qt::darkYellow : Qt::darkGreen);
          left->drawLine(begin, begin-end);
          left->translate(begin-end);
        } break;
      case 'E': {
          left->rotate(qRadiansToDegrees(-M_PI/6));
        } break;
      case 'I': {
          left->rotate(qRadiansToDegrees(M_PI/6));
        } break;
      case 'O': {
          left->save();
        } break;
      case 'U': {
          left->restore();
        } break;
    }
  }
  left->end();
  }
}

Canvas::Canvas(QWidget *parent)
  : QWidget(parent)
{
  canvas = this;
  this->show();
  setup();
  startTimer(16.67); // Framerate (30fps,33.33ms) / (60fps,16.67ms)
}

void Canvas::mousePressEvent(QMouseEvent *)
{ generate(); }

void Canvas::paintEvent(QPaintEvent *)
{ draw(); }

void Canvas::timerEvent(QTimerEvent *)
{ update(); }

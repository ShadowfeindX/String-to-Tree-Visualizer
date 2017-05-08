#include "canvas.hxx"

int cols;
Canvas * canvas = nullptr;
QPainter *_right = new QPainter, *_left = new QPainter;

QString str;
QPointF begin(0,0), end(0, 100);
enum Active {
  Left,
  Right,
  Both
} status;

QDialog * menu;
QMap<QString,QString> rules {
  {"A", "BWRW"},
  {"E", "CHSX"},
  {"I", "DJNY"},
  {"O", "FKPT"},
  {"U", "GLQV"}
};

QString convert(QString s) {
  s = s.toUpper();
  s.remove(" ");
  foreach (QString key, rules.keys()) {
    foreach (QString val, rules.value(key)) {
      s.replace(val,key);
    }
  } return s;
}

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
    str = convert(s);
    qDebug() << str << s;
  });
  QObject::connect(button, &QPushButton::clicked, [line]{
    reset();
    str = convert(line->text());
  });
  menu->layout()->addWidget(line);
  menu->layout()->addWidget(button);
  menu->layout()->addWidget(box);
  menu->show();
}

void draw()
{
  Qt::GlobalColor color = Qt::darkGreen;
  if (str.count('O') != str.count('U')) {
    _right->begin(canvas);
    _right->setFont(QFont("Arial", 40, 5));
    _right->drawText(40, canvas->height()/2, "Invalid String!");
    _right->end();
    return;
  } if (status == Right || status == Both) {
    _right->begin(canvas);
    _right->translate(canvas->width()/2, canvas->height());
    for (int i = 0; i < str.length(); ++i) {
      switch(str.at(i).unicode()) {
        case 'A': {
            _right->setPen(color = color == Qt::darkGreen ? Qt::darkYellow : Qt::darkGreen);
            _right->drawLine(begin, begin-end);
            _right->translate(begin-end);
          } break;
        case 'E': {
            _right->rotate(qRadiansToDegrees(M_PI/6));
          } break;
        case 'I': {
            _right->rotate(qRadiansToDegrees(-M_PI/6));
          } break;
        case 'O': {
            _right->save();
          } break;
        case 'U': {
            _right->restore();
          } break;
      }
    }
    _right->end();
  } if (status == Left || status == Both) {
    _left->begin(canvas);
    _left->translate(canvas->width()/2, canvas->height());
    for (int i = 0; i < str.length(); ++i) {
      switch(str.at(i).unicode()) {
        case 'A': {
            _left->setPen(color = color == Qt::darkGreen ? Qt::darkYellow : Qt::darkGreen);
            _left->drawLine(begin, begin-end);
            _left->translate(begin-end);
          } break;
        case 'E': {
            _left->rotate(qRadiansToDegrees(-M_PI/6));
          } break;
        case 'I': {
            _left->rotate(qRadiansToDegrees(M_PI/6));
          } break;
        case 'O': {
            _left->save();
          } break;
        case 'U': {
            _left->restore();
          } break;
      }
    }
    _left->end();
  }
}

Canvas::Canvas(QWidget *parent)
  : QWidget(parent)
{
  canvas = this;
  menu = new QDialog(this);
  this->show();
  setup();
  startTimer(250); // Framerate (30fps,33.33ms) / (60fps,16.67ms)
}

void Canvas::mousePressEvent(QMouseEvent *)
{ generate(); }

void Canvas::paintEvent(QPaintEvent *)
{ draw(); }

void Canvas::timerEvent(QTimerEvent *)
{ update(); }

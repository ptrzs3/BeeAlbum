#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Bee");
    this->setWindowIcon(QIcon(":/ico/bee.ico"));
    interval = 1000;
    here = 0;
    timer1 = new QTimer();
    timer2 = new QTimer();
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags( Qt::FramelessWindowHint);
    QList<QScreen*> screenCounter = QGuiApplication::screens();
    this->setGeometry(0, 0, screenCounter[0]->geometry().width(), screenCounter[0]->geometry().height());
    ui->label->setGeometry(0, 0, screenCounter[0]->geometry().width(), screenCounter[0]->geometry().height());
    target = QFileDialog::getExistingDirectory(this,QStringLiteral("选择照片文件夹"),QDir::homePath());
    if(target.isEmpty())
        exit(0);
    filter<<"*.jpg"<<"*.png"<<"*.jpeg";
    findPhotos(target);
    connect(timer1, &QTimer::timeout, this, &Widget::genRandom);
    connect(timer2, &QTimer::timeout, this, &Widget::nextPhoto);
    timer1->start(interval);
//    bool isOK;
//    QString input = QInputDialog::getText(NULL,
//                                         "PhotoAlbum",
//                                         "西北玄天一片云",
//                                         QLineEdit::Password,
//                                         "",
//                                         &isOK);
//    if(isOK && input == "xrj"){
//        timer1->start(interval);
//    } else {
//        exit(0);
//    }
}

Widget::~Widget()
{
    delete ui;
}
void Widget::genRandom()
{
    int i = QRandomGenerator::global()->bounded(jpg.size());
    numberList << i;
    showPixmap(i);
}
void Widget::nextPhoto()
{
    if(here < jpg.size() -1){
        here += 1;
    }
    showPixmap(here);
}
void Widget::showPixmap(int p)
{
    here = p;
    QPixmap pixmap;
    pixmap.load(jpg[p].absoluteFilePath());
    int ScalingFactor = 1;
    pixmap = pixmap.scaled(ui->label->width() * ScalingFactor,ui->label->height() * ScalingFactor,Qt::KeepAspectRatio);
    ui->label->setPixmap(pixmap);
    ui->label->setAlignment(Qt::AlignCenter);
}
QFileInfoList Widget::removeEmptyFolder(QFileInfoList src)
{
    QFileInfo t;
    QFileInfoList dst;
    foreach(t, src)
    {
        if(!QDir(t.absoluteFilePath()).isEmpty())
        {
            dst.append(t);
        }
    }
    return dst;
}

void Widget::findPhotos(QString dir) {
    QDir f;
    f.setPath(dir);
    f.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList t = removeEmptyFolder(f.entryInfoList());
    if(t.size()) {
        for(int i=0; i<t.size(); i++) {
            findPhotos(t[i].absoluteFilePath());
        }
    }
    f.setFilter(QDir::Files);
    f.setNameFilters(filter);
    jpg.append(f.entryInfoList());
}

void Widget::keyPressEvent(QKeyEvent *ev) {
    if(ev->key() == Qt::Key_Escape) {
        exit(0);
    } else if(ev->key() == Qt::Key_Down) {
        interval += 100;
        timer1->setInterval(interval);
    } else if(ev->key() == Qt::Key_Up) {
        interval -= 100;
        if(interval <= 300)
            interval = 300;
        timer1->setInterval(interval);
    } else if(ev->key() == Qt::Key_Space) {
        if(timer1->isActive()) {
            timer1->stop();
            n = numberList.size() - 1;
        } else {
            timer1->start(interval);
            timer2->stop();
        }
    } else if(!timer1->isActive()){
        if(ev->key() == Qt::Key_Left) {
            if(n > 0) {
                n -= 1;
            }
            showPixmap(numberList[n]);
        } else if (ev->key() == Qt::Key_Right) {
            if(n < numberList.size() - 1) {
                n += 1;
            } else {
                int i = QRandomGenerator::global()->bounded(jpg.size());
                numberList << i;
                n = numberList.size() - 1;
            }
            showPixmap(numberList[n]);
        } else if(ev->key() == Qt::Key_N) {
            if(here > 0){
                here -= 1;
            }
            showPixmap(here);
        } else if(ev->key() == Qt::Key_M) {
            if(here < jpg.size() - 1) {
                here += 1;
            }
            showPixmap(here);
        } else if(ev->key() == Qt::Key_B) {
            //  定时器2只有在定时器1停止时才能用
            if(timer2->isActive()) {
                timer2->stop();
            } else {
                timer2->start(interval);
            }
        }
    }
}

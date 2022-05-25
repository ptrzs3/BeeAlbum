#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QList>
#include <QScreen>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QRandomGenerator>
#include <QInputDialog>
#include <QLineEdit>
#include <QFileDialog>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void autoShowPixmap(int);
    void showPixmap(int);
    void getPics(QString);
    QFileInfoList removeEmptyFolder(QFileInfoList);
    QStringList getAlbums(QString);
    void findPhotos(QString);
    QStringList filter;
    QFileInfoList jpg;
    QString target;
    QTimer* timer1;
    QTimer* timer2;
    int interval;
    int here;
    QList<int> numberList;
    int n;
    void genRandom();
    void nextPhoto();

private:
    Ui::Widget *ui;

protected:
    virtual void keyPressEvent(QKeyEvent *ev);
};

#endif // WIDGET_H

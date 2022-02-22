#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QAction>

namespace Ui {
class Widget;
}

class Widget : public QWidget  // 继承QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    int getRandom(int max);//获取随机数
    void topForward(); // 向上移动
    void downForward(); // 向下移动
    void leftForward(); // 向左移动
    void rightForward(); // 向右移动
    void deleteTail(); // 删除last数据
    bool crush(); // 判断是否相撞
    enum Move{Left,Right,Up,Down};
protected slots:
    void timeOut();
    void rewardTimeOut();

private:
    Ui::Widget *ui;
    QList<QRectF> snake;
    int snakeNodeWidth = 10;
    int snakeNodeHeight = 10;
    QTimer *timer;
    QTimer *rewardTimer;
    int time = 100;
    int movefruit = Up;
    bool gameOver = false;
    bool gameStart = false;
    bool has_box = false;
    QList<QRectF> rewardNode;
    int speed = 1;
public slots:
    void start_clicked();
    void diff_clicked();
};

#endif // WIDGET_H

#include "widget.h"
#include "ui_widget.h"
#include <QLayout>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QMediaPlayer>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    QMediaPlayer *player = new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/file_example_WAV_1MG.wav"));
    player->setVolume(100);  //音量
    player->play();

    ui->setupUi(this);
    resize(400,400);
    setStyleSheet("QWidget{background:white}");

    //创建菜单
    auto layout = new QHBoxLayout(this);//创建一个水平布局,且将这个laout manager关联到window
    auto menuBar = new QMenuBar(this);
    QAction *game_start = new QAction("start game");
    QAction *difficulty = new QAction("difficulty");
    layout->setMenuBar(menuBar);//用setMenuBar进行添加
    menuBar->addAction(game_start);
    menuBar->addAction(difficulty);
    connect(game_start,SIGNAL(triggered()),this,SLOT(start_clicked()));
    connect(difficulty,SIGNAL(triggered()),this,SLOT(diff_clicked()));


    //蛇的初始长度为3
    snake.append(QRectF(100,300,snakeNodeWidth,snakeNodeHeight));
    topForward();
    topForward();
    // 在平面内随机位置产生一个果子
    rewardNode.append(QRectF(getRandom(this->width()),getRandom(this->height()),snakeNodeWidth,snakeNodeWidth));

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()),this,SLOT(timeOut()));

    rewardTimer = new QTimer;
    connect(rewardTimer,SIGNAL(timeout()),this,SLOT(rewardTimeOut()));
    this->setWindowTitle("Greed Snake Game");

}

Widget::~Widget()
{
    delete ui;
}

void Widget::timeOut()//计时，每一个时钟内蛇前进1格
{
    int fruit = 1;
    for(int i=0; i<rewardNode.length(); i++){
        //遍历每个果子
        //如果蛇头的中间点到达了果子的区域内
        if(rewardNode.at(i).contains(snake.at(0).topLeft()+QPointF(snakeNodeWidth/2,snakeNodeHeight/2))){
            fruit++; // 吃到了果子
            rewardNode.removeAt(i);//被吃掉的果子消失
            break;
        }
    }
    while(fruit--){//判断蛇正在朝什么方向前进，在前进方向蛇的长度+fruit的长度
        switch (movefruit) {
        case Up:
            topForward();
            break;
        case Down:
            downForward();
            break;
        case Right:
            rightForward();
            break;
        case Left:
            leftForward();
            break;
        default:
            break;
        }
    }
    deleteTail();//删除蛇尾部的方块

    update();//刷新窗口
}
void Widget::rewardTimeOut()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(rewardNode.length() > 5){
        rewardNode.removeAt(qrand()%5);
    }
    rewardNode.append(QRectF(qrand()%(this->width()/20)*20,qrand()%(this->height()/20)*20,snakeNodeWidth,snakeNodeWidth));
}

// 向上移动
void Widget::topForward()
{   //两种情况
    //第一种情况：当蛇头到达顶部时
    if(snake.at(0).y()-snakeNodeHeight < 0){
        //在底部相对应的位置加一个方块，作为头
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),this->height()-snakeNodeHeight),
                              QPointF(snake.at(0).x()+snakeNodeWidth,this->height())));
    }else{
        //蛇头未达到顶部时，在蛇头前面加一个方块，作为头
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeNodeHeight),snake.at(0).topRight()));
    }
}
// 向下移动
void Widget::downForward()
{   //两种情况
    //第一种情况：当蛇头到达底部时
    //在顶部相对应的位置加一个方块，作为头
    //第二种情况：当蛇头没有到达底部时
    //在蛇头下面加一个方块，作为头
    if(snake.at(0).y()+snakeNodeHeight*2 > this->height()){
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),snakeNodeHeight),
                              QPointF(snake.at(0).x()+snakeNodeWidth,0)));
    }else{
        snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeNodeHeight)));
    }
}
// 向左移动
void Widget::leftForward()
{//两种情况
    //第一种情况：当蛇头到达左边界时
    //在右边界相对应的位置加一个方块，作为头
    //第二种情况：当蛇头没有到达左边界时
    //在蛇头右面加一个方块，作为头
    if(snake.at(0).x()-snakeNodeWidth < 0){
        snake.insert(0,QRectF(QPointF(this->width()-snakeNodeWidth,snake.at(0).y()),
                              QPointF(this->width(),snake.at(0).y()+snakeNodeHeight)));
    }else{
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeNodeWidth,0),snake.at(0).bottomLeft()));
    }
}
// 向右移动
void Widget::rightForward()
{//两种情况
    //第一种情况：当蛇头到达右边界时
    //在左边界相对应的位置加一个方块，作为头
    //第二种情况：当蛇头没有到达左边界时
    //在蛇头左面加一个方块，作为头
    if(snake.at(0).x()+snakeNodeWidth*2 > this->width()){
        snake.insert(0,QRectF(QPointF(0,snake.at(0).y()),
                              QPointF(snakeNodeWidth,snake.at(0).y()+snakeNodeHeight)));
    }else{
        snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeNodeWidth,0)));
    }
}
// 删除结尾数据
void Widget::deleteTail()
{
    snake.removeLast();
}

// 绘图
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont fontg ( "Microsoft YaHei", 15);

    painter.setRenderHint(QPainter::Antialiasing);//反锯齿

    pen.setColor(Qt::black);
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    //画出蛇的身体
    for(int i=0; i<snake.length(); i++){
        painter.drawRect(snake.at(i));
    }
    brush.setColor(Qt::blue);
    painter.setBrush(brush);
    //画出果子
    for(int i=0; i<rewardNode.length(); i++){
        painter.drawEllipse(rewardNode.at(i));
    }
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setFont(fontg);
    painter.drawText(300,380,QString("points:")+QString("%1").arg(snake.length()-3));
    //游戏结束

    if(crush()){
        timer->stop();
        rewardTimer->stop();
        gameOver = true;
        painter.drawText(150,200,QString("游戏结束"));
        }

        QWidget::paintEvent(event);

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Up:
        if(movefruit != Down){
            movefruit = Up;
        }
        break;
    case Qt::Key_Down:
        if(movefruit != Up){
            movefruit = Down;
        }
        break;
    case Qt::Key_Right:
        if(movefruit != Left){
            movefruit = Right;
        }
        break;
    case Qt::Key_Left:
        if(movefruit != Right){
            movefruit = Left;
        }
        break;
    case Qt::Key_Space:
        if(!gameStart && !gameOver){
            timer->stop();
            rewardTimer->stop();
            gameStart = true;
        }
        break;
    default:
        break;
    }
}
void Widget::start_clicked(){
    if(!gameOver){
        timer->start(time*2/speed);
        rewardTimer->start(time*30*2/speed);
        gameStart = false;
    }
    else{
        snake.clear();
        rewardNode.clear();
        movefruit = Up;
        snake.append(QRectF(100,300,snakeNodeWidth,snakeNodeHeight));
        topForward();
        topForward();
        // 在平面内随机位置产生一个果子
        rewardNode.append(QRectF(getRandom(this->width()),getRandom(this->height()),snakeNodeWidth,snakeNodeWidth));
        timer->start(time*2/speed);
        rewardTimer->start(time*2*30/speed);
        gameOver = false;
    }

}
void Widget::diff_clicked(){
    bool ok;
    int value1 = QInputDialog::getInt(this,tr("难度设置"),
    tr("请输入难度：1~9"),1,0,9,1,&ok);
    if(ok) speed = value1;



}
// 以O（n^2）的复杂度查看是否相撞
bool Widget::crush()
{
    for(int i=0; i<snake.length(); i++){
        for(int j=i+1; j<snake.length(); j++){
            if(snake.at(i) == snake.at(j)){
                return true;
            }
        }
    }
    return false;
}


int Widget::getRandom(int max)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    int num = qrand()%(max/20)*20;
    return num;
    //qDebug()<<num+min;
}

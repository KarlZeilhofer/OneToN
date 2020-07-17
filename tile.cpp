#include "tile.h"

#include <QPaintEvent>
#include <QPainter>
#include <oneton.h>

Tile::Tile(QWidget *parent) : QWidget(parent)
{
    number = 0;
}

void Tile::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPen penFrame;
    QBrush brushFrame;
    QPen penText;

    switch(state){
    case Tile::Hidden:
        penFrame = QPen(Qt::transparent);
        brushFrame = QBrush(Qt::transparent);
        penText = QPen(Qt::transparent);
        break;
    case Tile::Preview:
        penFrame = QPen(Qt::black);
        brushFrame = QBrush(Qt::white);
        penText = QPen(Qt::black);
        break;
    case Tile::Obscured:
    case Tile::Placed:
        penFrame = QPen(Qt::black);
        brushFrame = QBrush(Qt::white);
        penText = QPen(Qt::transparent);
        break;
    case Tile::CheckedCorrect:
        penFrame = QPen(Qt::black);
        brushFrame = QBrush(Qt::green);
        penText = QPen(Qt::black);
        break;
    case Tile::CheckedWrong:
        penFrame = QPen(Qt::black);
        brushFrame = QBrush(Qt::red);
        penText = QPen(Qt::black);
        break;
    }

    p.setPen(penFrame);
    p.setBrush(brushFrame);
    p.drawRect(QRect(rect().x(), rect().y(), rect().width()-1, rect().height()-1) );

    p.setPen(penText);
    p.setFont(QFont("Arial", int(rect().height()*0.7), 1));
    p.drawText(rect(), QString::number(number), Qt::AlignCenter|Qt::AlignHCenter);
}

void Tile::mousePressEvent(QMouseEvent *event)
{
    OneToN* app = OneToN::app;
    if(state == Obscured && app->counter > 0){
        if(app->counter == number){
            state = CheckedCorrect;
            app->counter++;
            if(app->counter > app -> count){
                app->count++;
                app->timerNewGame->start(2000);
            }
        }else{
            state = CheckedWrong;
            app->timerNewGame->start(4000);
            app->counter = 0;
        }
    }
    update();
}

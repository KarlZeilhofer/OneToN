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
        brushFrame = QBrush(QColor(0x91f0b8)); // green
        penText = QPen(Qt::black);
        break;
    case Tile::CheckedWrong:
        penFrame = QPen(Qt::black);
        brushFrame = QBrush(QColor(0xee8585)); // red
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

    switch(app->mode){
    case OneToN::ModeIntro:
        if(app->state == OneToN::StateWait){
            if(number > 0){
                app->floodWithTiles();
                state = Hidden;
                app->state = OneToN::StateGo;
                app->correct();
            }
        }
        else if(app->state == OneToN::StateGo){
            if(number > 0){ // hit a number
                state = Hidden;
                app->correct();
            }else{ // error
                app->wrong();
            }
        }

        break;
    case OneToN::ModeTraining:
        if(app->state == OneToN::StateWait){
            if(number == 1){
                app->obscureTiles();
                state = CheckedCorrect;
                app->state = OneToN::StateGo;
                app->correct();
            }else{
                state = CheckedWrong;
                app->wrong();
            }
        }
        else if(app->state == OneToN::StateGo){
            if(state == Obscured){
                if(app->counter == number){
                    state = CheckedCorrect;
                    app->correct();
                }else{
                    state = CheckedWrong;
                    app->wrong();
                }
            }
        }

        break;
    case OneToN::ModeChallange:
        if(state == Obscured && app->counter > 0){
            if(app->counter == number){
                state = CheckedCorrect;
                app->correct();
            }else{
                state = CheckedWrong;
                app->wrong();
            }
        }
        break;
    }


    update();
}

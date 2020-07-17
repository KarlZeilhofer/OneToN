#include "oneton.h"
#include "ui_oneton.h"
#include "math.h"

#include <QGridLayout>

OneToN* OneToN::app = 0;

OneToN::OneToN(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OneToN)
{
    ui->setupUi(this);
    app = this;

    timerNewGame = new QTimer(this);
    timerPreviewTiles = new QTimer(this);
    timerObscure = new QTimer(this);


    QGridLayout* grid = new QGridLayout;

    for(int y=0; y<6; y++){
        for(int x=0; x<6; x++){
            if(x == 0){
                tiles.append(new QList<Tile*>);
            }
            Tile* t = new Tile;
            tiles[y]->append(t);
            grid->addWidget(t, y, x, 1, 1);
        }
    }

    QWidget* w = new QWidget();
    w->setLayout(grid);

    setCentralWidget(w);
    srand(time(0));

    timerNewGame->setSingleShot(true);
    connect(timerNewGame, SIGNAL(timeout()), this, SLOT(newGame()));

    timerPreviewTiles->setSingleShot(true);
    connect(timerPreviewTiles, SIGNAL(timeout()), this, SLOT(previewTiles()));

    timerObscure->setSingleShot(true);
    connect(timerObscure, SIGNAL(timeout()), this, SLOT(obscureTiles()));

    count = 2;

    newGame();
}

OneToN::~OneToN()
{
    delete ui;
}



void OneToN::newGame()
{
    counter = 1;

    for(int y=0; y<6; y++){
        for(int x=0; x<6; x++){
            Tile* t = tiles.at(y)->at(x);
            t->state = Tile::Hidden;
            t->number = 0;
            t->update();
        }
    }

    QList<int> list;
    for(int i=0; i<6*6; i++){
        list.append(i);
    }

    for(int i=0; i<count; i++){
        int ii = rand() % list.size();
        int x = list.at(ii);
        Tile* t = tiles.at(x/6)->at(x%6);
        t->state = Tile::Placed;
        t->number = i+1;
        t->update();
        list.removeAt(ii);
    }

    timerPreviewTiles->start(2000);
}

void OneToN::previewTiles()
{
    for(int y=0; y<6; y++){
        for(int x=0; x<6; x++){
            Tile* t = tiles.at(y)->at(x);
            if(t->state == Tile::Placed){
                t->state = Tile::Preview;
                t->update();
            }
        }
    }

    int ms = 1200*count*count/36;
    timerObscure->start(1000>ms?1000:ms);
}

void OneToN::obscureTiles()
{
    for(int y=0; y<6; y++){
        for(int x=0; x<6; x++){
            Tile* t = tiles.at(y)->at(x);
            if(t->state == Tile::Preview){
                t->state = Tile::Obscured;
                t->update();
            }
        }
    }
}

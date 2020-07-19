#include "oneton.h"
#include "ui_oneton.h"
#include "math.h"

#include <QGridLayout>
#include <QSettings>

OneToN* OneToN::app = 0;

OneToN::OneToN(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OneToN)
{
    ui->setupUi(this);
    app = this;
    scoreLabel = new QLabel(this);

    ui->statusBar->addWidget(scoreLabel);

    timerNewRound = new QTimer(this);
    timerPreviewTiles = new QTimer(this);
    timerObscure = new QTimer(this);
    secondsTick = new QTimer(this);


    QGridLayout* grid = new QGridLayout;

    for(int y=0; y<N; y++){
        for(int x=0; x<N; x++){
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

    timerNewRound->setSingleShot(true);
    connect(timerNewRound, SIGNAL(timeout()), this, SLOT(newRound()));

    timerPreviewTiles->setSingleShot(true);
    connect(timerPreviewTiles, SIGNAL(timeout()), this, SLOT(previewTiles()));

    timerObscure->setSingleShot(true);
    connect(timerObscure, SIGNAL(timeout()), this, SLOT(obscureTiles()));

    secondsTick->start(1000);
    connect(secondsTick, SIGNAL(timeout()), this, SLOT(incrementGameTime()));

    level = 2;
    QSettings set;
    highScore = set.value("highScore").toInt();

    updateScore();
    newRound();
}

OneToN::~OneToN()
{
    delete ui;
}



void OneToN::newRound()
{
    counter = 1;

    for(int y=0; y<N; y++){
        for(int x=0; x<N; x++){
            Tile* t = tiles.at(y)->at(x);
            t->state = Tile::Hidden;
            t->number = 0;
            t->update();
        }
    }

    QList<int> list;
    for(int i=0; i<N*N; i++){
        list.append(i);
    }

    for(int i=0; i<level; i++){
        int ii = rand() % list.size();
        int x = list.at(ii);
        Tile* t = tiles.at(x/N)->at(x%N);
        t->state = Tile::Placed;
        t->number = i+1;
        t->update();
        list.removeAt(ii);
    }

    timerPreviewTiles->start(2000);
    levelShown = level;
    updateScore();
}

void OneToN::previewTiles()
{
    for(int y=0; y<N; y++){
        for(int x=0; x<N; x++){
            Tile* t = tiles.at(y)->at(x);
            if(t->state == Tile::Placed){
                t->state = Tile::Preview;
                t->update();
            }
        }
    }

    int ms = 1200*level*level/36;
    timerObscure->start(1000>ms?1000:ms);
}

void OneToN::obscureTiles()
{
    for(int y=0; y<N; y++){
        for(int x=0; x<N; x++){
            Tile* t = tiles.at(y)->at(x);
            if(t->state == Tile::Preview){
                t->state = Tile::Obscured;
                t->update();
            }
        }
    }
}

void OneToN::correct()
{
    if(level > topLevel){
        totalScore += 10;
    }

    counter++;
    if(counter > level){
        topLevel = level;
        level++;
        timerNewRound->start(2000);
        wrongCounter=0;
        fixedScore = totalScore;
    }
    updateScore();
}

void OneToN::wrong()
{
    totalWrongCounter += level-counter;

    timerNewRound->start(4000);
    counter = 0;

    wrongCounter++;
    if(wrongCounter >=3 && level > 2){
        level--;
        wrongCounter=0;
    }

    totalScore = fixedScore; // reset internal counter to fixedScore
    updateScore();
}

void OneToN::updateScore()
{
    double ratio;
    if(totalCorrectCounter+totalWrongCounter == 0){
        ratio = 0;
    }else{
        ratio = 100 * totalCorrectCounter/float(totalCorrectCounter+totalWrongCounter);
    }

    if(totalScore > myHighScore){
        myHighScore = totalScore;
        if(myHighScore > highScore){
            highScore = myHighScore;
            QSettings set;
            set.setValue("highScore", highScore);
        }
    }

    this->scoreLabel->setText(QString("Level %1, %2 Punkte, Highscore: %3      %4").
                              arg(levelShown).arg(myHighScore).arg(highScore).arg(gameTime/60 + (gameTime%60)/100.0, 0, 'f', 2));
}

void OneToN::incrementGameTime()
{
    gameTime++;
    updateScore();
}

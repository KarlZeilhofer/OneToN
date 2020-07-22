#include "oneton.h"
#include "ui_oneton.h"
#include "math.h"

#include <QGridLayout>
#include <QSettings>
#include <QtMultimedia/QSound>

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

    mode = ModeChallange;

    QActionGroup* ag = new QActionGroup(this);
    ag->addAction(ui->actionModeIntro);
    ag->addAction(ui->actionModeTraining);
    ag->addAction(ui->actionModeChallange);



    QGridLayout* grid = new QGridLayout;

    for(int y=0; y<Ny; y++){
        for(int x=0; x<Nx; x++){
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

    QSettings set;
    highScore = set.value("highScore").toInt();
    mode = GameMode(set.value("mode").toInt());

    switch (mode) {
    case OneToN::ModeIntro:
        ui->actionModeIntro->setChecked(true);
        break;
    case OneToN::ModeTraining:
        ui->actionModeTraining->setChecked(true);
        break;
    case OneToN::ModeChallange:
        ui->actionModeChallange->setChecked(true);
        break;
    }

    newGame();
    showMaximized();
}

OneToN::~OneToN()
{
    delete ui;
}

void OneToN::newGame()
{
    timerNewRound->stop();
    timerObscure->stop();
    timerPreviewTiles->stop();

    myHighScore = 0;
    totalScore = 0;
    fixedScore = 0;
    topLevel = 0;
    level = 2;

    wrongCounter=0;
    correctCounter=0;
    gameTime = 0;

    updateScore();
    newRound();
}



void OneToN::newRound()
{
    counter = 1;
    state = StateWait;

    hideTiles();

    QList<int> list;
    for(int i=0; i<Nx*Ny; i++){
        list.append(i);
    }

    for(int i=0; i<level; i++){
        int ii = rand() % list.size();
        int k = list.at(ii);
        Tile* t = tiles.at(k/Nx)->at(k%Nx);
        t->state = Tile::Placed;
        t->number = i+1;
        t->update();
        list.removeAt(ii);
    }

    levelShown = level;
    updateScore();

    switch (mode) {
    case OneToN::ModeIntro:
        previewTiles();
        break;
    case OneToN::ModeTraining:
        previewTiles();
        break;
    case OneToN::ModeChallange:
        timerPreviewTiles->start(2000);
        break;
    }


}

void OneToN::hideTiles()
{
    for(int y=0; y<Ny; y++){
        for(int x=0; x<Nx; x++){
            Tile* t = tiles.at(y)->at(x);
            t->state = Tile::Hidden;
            t->number = 0;
            t->update();
        }
    }
}

void OneToN::previewTiles()
{
    for(int y=0; y<Ny; y++){
        for(int x=0; x<Nx; x++){
            Tile* t = tiles.at(y)->at(x);
            if(t->state == Tile::Placed){
                t->state = Tile::Preview;
                t->update();
            }
        }
    }

    switch(mode){
    case ModeIntro:
        // wait until any number-tile is clicked
        // then flood with obscured tiles
    break;
    case ModeTraining:
        // wait until tile one is clicked
        // then obscure all number-tiles
    break;
    case ModeChallange:
        int ms = 1200*level*level/36;
        timerObscure->start(1000>ms?1000:ms);
    break;
    }

}

void OneToN::obscureTiles()
{
    for(int y=0; y<Ny; y++){
        for(int x=0; x<Nx; x++){
            Tile* t = tiles.at(y)->at(x);
            if(t->state == Tile::Preview){
                t->state = Tile::Obscured;
                t->update();
            }
        }
    }
}

void OneToN::floodWithTiles()
{
    for(int y=0; y<Ny; y++){
        for(int x=0; x<Nx; x++){
            Tile* t = tiles.at(y)->at(x);
            t->state = Tile::Obscured;
            t->update();
        }
    }
}

/// correct tile clicked
void OneToN::correct()
{
    switch(mode){
    case OneToN::ModeTraining:
    case OneToN::ModeIntro:
    case OneToN::ModeChallange:
        if(level > topLevel){
            totalScore += 10;
        }

        counter++;
        QSound::play(":/sounds/click.wav");
        if(counter > level){ // if clicked all numbers in this round
            QSound::play(":/sounds/success.wav");
            topLevel = level;
            correctCounter++;
            if(correctCounter >= 3){
                correctCounter = 0;
                level++;
                wrongCounter=0;
            }
            timerNewRound->start(2000);
            fixedScore = totalScore;
            state = StateWait;
        }else{

        }
        break;
    }

    updateScore();
}

/// wrong tile clicked
void OneToN::wrong()
{
    state = StateFailed;
    QSound::play(":/sounds/fail.wav");
    // TODO: sound!!!
    switch(mode){
    case OneToN::ModeIntro:
        hideTiles();
        counter = 0;
        timerNewRound->start(1000);
        totalScore = fixedScore; // reset internal counter to fixedScore
        break;
    case OneToN::ModeTraining:
    case OneToN::ModeChallange:
        totalWrongCounter += level-counter;

        timerNewRound->start(4000);
        counter = 0;

        wrongCounter++;
        correctCounter = 0;
        if(wrongCounter >=3 && level > 2){
            level--;
            wrongCounter=0;
        }

        totalScore = fixedScore; // reset internal counter to fixedScore
        break;
    }
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

void OneToN::on_actionModeIntro_triggered()
{
    mode = ModeIntro;
    saveMode();
    newGame();
}

void OneToN::on_actionModeTraining_triggered()
{
    mode = ModeTraining;
    saveMode();
    newGame();
}

void OneToN::on_actionModeChallange_triggered()
{
    mode = ModeChallange;
    saveMode();
    newGame();
}

void OneToN::on_actionNew_Game_triggered()
{
    newGame();
}

void OneToN::on_actionNew_Round_triggered()
{
    newRound();
}

void OneToN::saveMode()
{
    QSettings set;
    set.setValue("mode", int(mode));
}

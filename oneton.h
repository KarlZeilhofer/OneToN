#ifndef ONETON_H
#define ONETON_H

#include "tile.h"

#include <QLabel>
#include <QMainWindow>
#include <QTimer>

namespace Ui {
class OneToN;
}

class OneToN : public QMainWindow
{
    Q_OBJECT

public:
    explicit OneToN(QWidget *parent = 0);
    ~OneToN();

    static OneToN* app;
    int counter; // count for correct tiles
    int level; // count of total tiles in current round
    int levelShown = 2;

public slots:
    void newRound();
    void previewTiles();
    void obscureTiles();

    void correct();
    void wrong();

    void updateScore();

    void incrementGameTime();


private:
    Ui::OneToN *ui;
    QList< QList<Tile*>* > tiles;
    int wrongCounter = 0; // reduce count after 3 lost games

    int totalWrongCounter; // for statistics
    int totalCorrectCounter = 0; // for statistics

    int myHighScore=0; // shows maximum of totalScore
    int totalScore=0; // resets on each start of a round
    int highScore=0;
    int fixedScore=0;
    int topLevel=0; // top completed level

    int gameTime=0;
    int N=4; // game board size NxN

public:
    QTimer* timerNewRound;
    QTimer* timerPreviewTiles;
    QTimer* timerObscure;
    QLabel* scoreLabel;
    QTimer* secondsTick;
};

#endif // ONETON_H

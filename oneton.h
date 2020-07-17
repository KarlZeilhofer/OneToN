#ifndef ONETON_H
#define ONETON_H

#include "tile.h"

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
    int count; // count of total tiles in current round

public slots:
    void newGame();
    void previewTiles();
    void obscureTiles();


private:
    Ui::OneToN *ui;
    QList< QList<Tile*>* > tiles;

public:
    QTimer* timerNewGame;
    QTimer* timerPreviewTiles;
    QTimer* timerObscure;

};

#endif // ONETON_H

#ifndef TILE_H
#define TILE_H

#include <QWidget>

class Tile : public QWidget
{
    Q_OBJECT
public:
    explicit Tile(QWidget *parent = nullptr);

signals:

public slots:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    int number;
    enum State{Hidden, Placed, Preview, Obscured, CheckedCorrect, CheckedWrong};
    State state = Hidden;
};

#endif // TILE_H

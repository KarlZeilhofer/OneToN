One To N
========

A very simple, but addictive game in remembering number positions. 

Please have a look at the very awesome demonstration with a shimpanse: https://www.youtube.com/watch?v=GgtzZnagJiM

Written with Qt 5.12, tested on Linux Mint 20.0

# Gameplay
Tiles with numbers are shown for a very short time.
![Preview for Level 10](screenshot-10-preview.png)

Try to remember theire positions. When the numbers are hidden, click
on the white tiles starting from 1, counting upwards.

If you get all right, you enter the next level, with one extra tile.

If you failed 3 times in a row, you drop to the previous level.
![Failed, didn't remember number 7!](screenshot-10-failed.png)

# Compile
```
git clone https://github.com/KarlZeilhofer/OneToN
mkdir build
cd build
qmake ../OneToN
make
```


# License
Public Domain

#include "oneton.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("KarlZeilhofer");
    a.setApplicationName("One To N");
    a.setApplicationVersion("0.0.4");

    OneToN w;
    w.show();

    return a.exec();
}

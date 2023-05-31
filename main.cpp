#include <QApplication>
#include "gl-ctrls/precise_landing_assist_ctrl.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget wgt;
    wgt.resize(600, 400);
    PreciseLandingAssistCtrl ctrl(&wgt);
    wgt.show();

    return app.exec();
}

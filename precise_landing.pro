QT += widgets core opengl

CONFIG += c++11

LIBS += \


HEADERS +=  \
    gl-ctrls/gl_utils.h     \
    gl-ctrls/precise_landing_assist_ctrl.h
#    gl-ctrls/precise_landing_assist_card.h


SOURCES +=  \
    gl-ctrls/gl_utils.cpp     \
    gl-ctrls/precise_landing_assist_ctrl.cpp    \
#    gl-ctrls/precise_landing_assist_card.cpp
    main.cpp


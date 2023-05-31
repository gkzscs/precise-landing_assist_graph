#ifndef PreciseLandingAssistCtrl_H
#define PreciseLandingAssistCtrl_H

#include <QOpenGLWidget>
#include <QMutex>

#include "gl_utils.h"


class PreciseLandingAssistCtrl : public QOpenGLWidget, public GLFuncUtils
{
public:
    PreciseLandingAssistCtrl(QWidget *parent = nullptr);
    ~PreciseLandingAssistCtrl() override;

    void set_direction(double d);
    double direction() const;

    void set_distance(double d);
    double distance() const;

    void set_uav_angle(double d);
    double uav_angle() const;

    void update_ui();

public:
    void set_radius_range(double min, double max);
    double min_radius() const;
    double max_radius() const;

    void set_radius(double d);
    double radius() const;

    void set_radius_scale_step(double d);
    double radius_scale_step() const;

private:
    void init_members();
    void init_ui();
    void init_signal_slots();

    void test();

private:
    void calc_members();
    void calc_uav_pos();
    void calc_distance_mark_points();
    void calc_distance_mark_text();

protected:
    void wheelEvent(QWheelEvent *e) override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void draw_bg();
    void draw_axis();
    void draw_tgt();
    void draw_uav();
    void draw_distance_mark();

private:
    void draw_text(const QString &txt, const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight, bool bold = true,
                   int pixelSz = 12, const QColor &cl = Qt::white, int flags = Qt::AlignCenter);

private:
    double      _direction;
    double      _distance;
    double      _uav_angle;

private:
    // assist vars
    bool        _uav_is_inside;

    double      _radius;
    double      _min_radius;
    double      _max_radius;
    double      _radius_scale_step;

    QString     _str_distance;

    GLPoint2f   _uav_pos;

    QVector<GLPoint2f>      _vec_axis_pts;
    QVector<GLPoint2f>      _vec_uav_triangle_pts;
    QVector<GLPoint2f>      _vec_uav_outside_triangle_pts;
    QVector<GLPoint2f>      _vec_distance_lines_pts;
    QVector<GLPoint2f>      _vec_distance_txt_pts;

private:
    GLColor3f   _cl_gray;
    GLColor3f   _cl_dark_blue;
    GLColor3f   _cl_blue;
    GLColor3f   _cl_red;
    GLColor3f   _cl_yellow;
    GLColor3f   _cl_white;

private:
    QMutex      _mtx;

};

#endif // PreciseLandingAssistCtrl_H

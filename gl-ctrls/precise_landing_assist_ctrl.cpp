#include "precise_landing_assist_ctrl.h"

#include <QDebug>
#include <QTimer>
#include <QWheelEvent>
#include <QMutexLocker>


static const double PI = 3.1415926;

static const int font_pixel_size = 12;
static const float circle_f = 0.6f;
static const GLPoint2f pt_center(0, 0);
static const GLPoint2f pt_top_left(-1*circle_f, 1*circle_f);
static const GLPoint2f pt_bottom_right(1*circle_f, -1*circle_f);


PreciseLandingAssistCtrl::PreciseLandingAssistCtrl(QWidget *parent)
    : QOpenGLWidget(parent)
{
    init_members();
    init_ui();
    init_signal_slots();

    test();
}

PreciseLandingAssistCtrl::~PreciseLandingAssistCtrl()
{

}

void PreciseLandingAssistCtrl::set_direction(double d)
{
    _direction = d;
}

double PreciseLandingAssistCtrl::direction() const
{
    return _direction;
}

void PreciseLandingAssistCtrl::set_distance(double d)
{
    if (d < 0) return;

    _distance = d;
}

double PreciseLandingAssistCtrl::distance() const
{
    return _distance;
}

void PreciseLandingAssistCtrl::set_uav_angle(double d)
{
    _uav_angle = d;
}

double PreciseLandingAssistCtrl::uav_angle() const
{
    return _uav_angle;
}

void PreciseLandingAssistCtrl::update_ui()
{
    QMutexLocker locker(&_mtx);

    calc_members();

    update();
}

void PreciseLandingAssistCtrl::set_radius_range(double min, double max)
{
    if (min < 0 || max < 0 || min > max) return;

    _min_radius = min;
    _max_radius = max;
}

double PreciseLandingAssistCtrl::min_radius() const
{
    return _min_radius;
}

double PreciseLandingAssistCtrl::max_radius() const
{
    return _max_radius;
}

void PreciseLandingAssistCtrl::set_radius(double d)
{
    if (d < _min_radius)
    {
        d = _min_radius;
    }
    else if (d > _max_radius)
    {
        d = _max_radius;
    }

    _radius = d;
}

double PreciseLandingAssistCtrl::radius() const
{
    return _radius;
}

void PreciseLandingAssistCtrl::set_radius_scale_step(double d)
{
    if (d < 0) return;

    _radius_scale_step = d;
}

double PreciseLandingAssistCtrl::radius_scale_step() const
{
    return _radius_scale_step;
}

void PreciseLandingAssistCtrl::init_members()
{
    _direction      = 0;
    _distance       = 0;
    _uav_angle      = 0;

    _min_radius     = 50;
    _max_radius     = 2000;
    _radius         = 500;
    _radius_scale_step  = 25;

    {
        const float f = 0.8f;
        _vec_axis_pts.push_back(GLPoint2f(-1*f, 0));
        _vec_axis_pts.push_back(GLPoint2f(1*f, 0));
        _vec_axis_pts.push_back(GLPoint2f(0, -1*f));
        _vec_axis_pts.push_back(GLPoint2f(0, 1*f));
    }

    {
        const float f = 0.03f;
        _vec_uav_triangle_pts.push_back(GLPoint2f(1*f, -2*f));
        _vec_uav_triangle_pts.push_back(GLPoint2f(-1*f, -2*f));
        _vec_uav_triangle_pts.push_back(GLPoint2f(0, 2*f));
    }

    {
        const float f = 0.03f;
        _vec_uav_outside_triangle_pts.push_back(GLPoint2f(-1*f, -1*f));
        _vec_uav_outside_triangle_pts.push_back(GLPoint2f(1*f, -1*f));
        _vec_uav_outside_triangle_pts.push_back(GLPoint2f(0, 0));
    }

    // colors
    {
        _cl_gray = qcolor_2_gl_color3f(QColor(79, 91, 104));
        _cl_dark_blue = qcolor_2_gl_color3f(QColor(5, 27, 50));
        _cl_blue = qcolor_2_gl_color3f(QColor(8, 47, 88));
        _cl_red = qcolor_2_gl_color3f(QColor(243, 4, 4));
        _cl_yellow = qcolor_2_gl_color3f(QColor(246, 238, 7));
        _cl_white = qcolor_2_gl_color3f(QColor(255, 255, 255));
    }
}

void PreciseLandingAssistCtrl::init_ui()
{
    resize(400, 400);

    auto f = font();
    f.setPixelSize(font_pixel_size);
    f.setFamily("Microsoft YaHei");
    setFont(f);

    // pay attention to the position of initialization
    QSurfaceFormat fmt = format();
    fmt.setSamples(18);
    setFormat(fmt);
}

void PreciseLandingAssistCtrl::init_signal_slots()
{

}

void PreciseLandingAssistCtrl::test()
{
    QTimer *tm = new QTimer();
    tm->setInterval(1000 / 10);
    connect(tm, &QTimer::timeout, [=]()
    {
        static double dis = 600;
        static double direct = 60;
        static double angle = 30;

        dis = dis - 1.23;
        direct = direct + 0.5;
        angle = angle - 0.5;

        set_distance(dis);
        set_direction(direct * PI / 180);
        set_uav_angle(angle * PI / 180);

        update_ui();
    });
    tm->start();
}

void PreciseLandingAssistCtrl::calc_members()
{
    calc_uav_pos();
    calc_distance_mark_points();
    calc_distance_mark_text();
}

void PreciseLandingAssistCtrl::calc_uav_pos()
{
    _uav_is_inside = (_distance < _radius);

    float ratio = static_cast<float>(_distance / _radius);
    float r = circle_f * (ratio < 1 ? ratio : 1);
    float x = r * static_cast<float>(cos(_direction + PI/2));
    float y = r * static_cast<float>(sin(_direction + PI/2));
    _uav_pos = GLPoint2f(x, y);
}

void PreciseLandingAssistCtrl::calc_distance_mark_points()
{
    static const float h_line_w = 0.4f;
    float hLineXOffset = h_line_w * (_uav_pos.x < 0 ? -1 : 1);
    GLPoint2f ptUav = (_uav_is_inside ? _uav_pos : scale_gl_pos(_uav_pos, 1.2f));
    GLPoint2f ptEnd = GLPoint2f(ptUav.x + hLineXOffset, ptUav.y);

    // line points
    {
        _vec_distance_lines_pts.clear();
        _vec_distance_lines_pts.push_back(pt_center);
        _vec_distance_lines_pts.push_back(ptUav);
        _vec_distance_lines_pts.push_back(ptEnd);
    }

    // txt points
    {
        static const float txt_h = 0.1f;
        GLPoint2f pt1, pt2;

        if (_uav_pos.x < 0)
        {
            pt1 = GLPoint2f(ptEnd.x, ptEnd.y + txt_h);
            pt2 = GLPoint2f(ptUav);
        }
        else
        {
            pt1 = GLPoint2f(ptUav.x, ptUav.y + txt_h);
            pt2 = GLPoint2f(ptEnd);
        }

        _vec_distance_txt_pts.clear();
        _vec_distance_txt_pts.push_back(pt1);
        _vec_distance_txt_pts.push_back(pt2);
    }
}

void PreciseLandingAssistCtrl::calc_distance_mark_text()
{
    if (_uav_is_inside)
    {
        _str_distance = QString("%1m").arg(_distance, 0, 'f', 2);
    }
    else
    {
        _str_distance = QString(">%1m").arg(_radius);
    }
}

void PreciseLandingAssistCtrl::wheelEvent(QWheelEvent *e)
{
    auto diff = _radius_scale_step * (e->delta() > 0 ? 1 : -1);
    auto r = _radius + diff;
    set_radius(r);

    qDebug() << "radius:" << min_radius() << max_radius() << radius();
}

void PreciseLandingAssistCtrl::initializeGL()
{
    if (!initializeOpenGLFunctions())
    {
        qDebug() << "init opengl functions failed";
    }

    reset_color();
}

void PreciseLandingAssistCtrl::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);

    glViewport(0, 0, w, h);
}

void PreciseLandingAssistCtrl::paintGL()
{
    QOpenGLWidget::paintGL();

    // draw graph
    draw_bg();
    draw_axis();
    draw_distance_mark();
    draw_tgt();
    draw_uav();
}

void PreciseLandingAssistCtrl::draw_bg()
{
    gl_clear_color3f(_cl_dark_blue);

    gl_color3f(_cl_blue);
    draw_ellipse(pt_top_left, pt_bottom_right);

    gl_color3f(_cl_gray);
    draw_ellipse(pt_top_left, pt_bottom_right, GL_LINE_LOOP);
}

void PreciseLandingAssistCtrl::draw_axis()
{
    static const QString str_n = "N";
    static const float axis_radius = 0.8f;
    static const float txt_w = 0.1f;
    static const float txt_h = 0.2f;
    static const GLPoint2f pt_me_top_left = GLPoint2f(0, axis_radius);
    static const GLPoint2f pt_me_bottom_right = GLPoint2f(txt_w, axis_radius-txt_h);

    gl_color3f(_cl_gray);
    draw_lines(_vec_axis_pts, GL_LINES);

    draw_text(str_n, pt_me_top_left, pt_me_bottom_right);
}

void PreciseLandingAssistCtrl::draw_tgt()
{
    static const float radius = 0.03f;
    static const QString str_h = "H";
    static const GLPoint2f pt_me_top_left = GLPoint2f(-radius, radius);
    static const GLPoint2f pt_me_bottom_right = GLPoint2f(radius, -radius);

    gl_color3f(_cl_red);
    draw_ellipse(pt_center, radius, radius);
    draw_text(str_h, pt_me_top_left, pt_me_bottom_right);
}

void PreciseLandingAssistCtrl::draw_uav()
{

    glPushMatrix();
    gl_color3f(_cl_yellow);
    glTranslatef(_uav_pos.x, _uav_pos.y, 0);

    if (_uav_is_inside)
    {
        const float angle = static_cast<float>(_uav_angle * 180 / PI);
        glRotatef(angle, 0, 0, 1);
        draw_triangle(_vec_uav_triangle_pts);
    }
    else
    {
        const float angle = static_cast<float>(_direction * 180 / PI);
        glRotatef(angle, 0, 0, 1);
        draw_triangle(_vec_uav_outside_triangle_pts);
    }

    glPopMatrix();
}

void PreciseLandingAssistCtrl::draw_distance_mark()
{
    gl_color3f(_cl_gray);
    draw_lines(_vec_distance_lines_pts, GL_LINE_STRIP);

    if (_vec_distance_txt_pts.size() == 2)
    {
        draw_text(_str_distance, _vec_distance_txt_pts.at(0), _vec_distance_txt_pts.at(1));
    }
}

void PreciseLandingAssistCtrl::draw_text(const QString &txt, const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight, bool bold,
                                         int pixelSz, const QColor &cl, int flags)
{
    QPainter p(this);
    {
        auto f = font();
        f.setBold(bold);
        f.setPixelSize(pixelSz);

        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(cl);
        p.setFont(f);
    }

    GLFuncUtils::draw_text(p, rect(), ptTopLeft, ptBottomRight, flags, txt);
}



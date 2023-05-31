#include "gl_utils.h"



static const float PI = 3.1415926f;


/**
 * @brief GLFuncUtils::gl_point_2_qpointf
 * pay attention to the differences of coordinates
 * @param pt
 * @param rcViewPort
 * @return
 */
QPointF GLFuncUtils::gl_point_2_qpointf(const GLPoint2f &pt, const QRect &rcViewPort)
{
    // add 1
    auto ptCenter = rect_center(rcViewPort);
    double disX = static_cast<double>(pt.x * rcViewPort.width() / 2);
    double disY = static_cast<double>(pt.y * rcViewPort.height() / 2);

    return QPointF(ptCenter.x()+disX, ptCenter.y()-disY);
}

GLPoint2f GLFuncUtils::qpointf_2_gl_point(const QPointF &pt, const QRect &rcViewPort)
{
    auto ptCenter = rect_center(rcViewPort);
    float x = static_cast<float>((pt.x() - ptCenter.x()) / rcViewPort.width());
    float y = static_cast<float>(-(pt.y() - ptCenter.y()) / rcViewPort.height());

    return GLPoint2f(x, y);
}

QColor GLFuncUtils::gl_color4f_2_qcolor(const GLColor4f &cl)
{
    int r = static_cast<int>(cl.r * 255);
    int g = static_cast<int>(cl.g * 255);
    int b = static_cast<int>(cl.b * 255);
    int a = static_cast<int>(cl.a * 255);

    return QColor(r, g, b, a);
}

GLColor4f GLFuncUtils::qcolor_2_gl_color4f(const QColor &cl)
{
    float r = cl.red() / 255.0f;
    float g = cl.green() / 255.0f;
    float b = cl.blue() / 255.0f;
    float a = cl.alpha() / 255.0f;

    return GLColor4f(r, g, b, a);
}

QColor GLFuncUtils::gl_color3f_2_qcolor(const GLColor3f &cl)
{
    int r = static_cast<int>(cl.r * 255);
    int g = static_cast<int>(cl.g * 255);
    int b = static_cast<int>(cl.b * 255);

    return QColor(r, g, b);
}

GLColor3f GLFuncUtils::qcolor_2_gl_color3f(const QColor &cl)
{
    float r = cl.red() / 255.0f;
    float g = cl.green() / 255.0f;
    float b = cl.blue() / 255.0f;

    return GLColor3f(r, g, b);
}

GLfloat GLFuncUtils::texture_coord_2_gl(GLfloat a)
{
    return (a * 2 - 1);
}

GLfloat GLFuncUtils::gl_coord_2_texture(GLfloat a)
{
    return (a + 1) / 2;
}

GLPoint2f GLFuncUtils::scale_gl_pos(const GLPoint2f &pt, float f)
{
    return GLPoint2f(pt.x * f, pt.y * f);
}

QPointF GLFuncUtils::rect_center(const QRect &rc)
{
    double centerX = (rc.left() + rc.right() + 1) / 2.0;
    double centerY = (rc.top() + rc.bottom() + 1) / 2.0;

    return QPointF(centerX, centerY);
}

void GLFuncUtils::gl_point2f(const GLPoint2f &pt)
{
    glVertex2f(pt.x, pt.y);
}

void GLFuncUtils::gl_color3f(const GLColor3f &cl)
{
    glColor3f(cl.r, cl.g, cl.b);
}

void GLFuncUtils::gl_color4f(const GLColor4f &cl)
{
    glColor4f(cl.r, cl.g, cl.b, cl.a);
}

void GLFuncUtils::gl_clear_qcolor(const QColor &cl)
{
    auto glCl = qcolor_2_gl_color4f(cl);
    gl_clear_color4f(glCl);
}

void GLFuncUtils::gl_clear_color3f(const GLColor3f &cl)
{
    glClearColor(cl.r, cl.g, cl.b, 1.0f);
}

void GLFuncUtils::gl_clear_color4f(const GLColor4f &cl)
{
    glClearColor(cl.r, cl.g, cl.b, cl.a);
}

void GLFuncUtils::draw_point(const GLPoint2f &pt)
{
    glBegin(GL_POINTS);
    {
        gl_point2f(pt);
    }
    glEnd();
}

void GLFuncUtils::draw_line(const GLPoint2f &pt1, const GLPoint2f &pt2)
{
    glBegin(GL_LINES);
    {
        gl_point2f(pt1);
        gl_point2f(pt2);
    }
    glEnd();
}

void GLFuncUtils::draw_lines(const QVector<GLPoint2f> &vecPts, GLenum mode)
{
    glBegin(mode);
    {
        for (auto pt : vecPts)
        {
            gl_point2f(pt);
        }
    }
    glEnd();
}

void GLFuncUtils::draw_triangle(const QVector<GLPoint2f> &vecPts)
{
    assert (vecPts.length() == 3);

    glBegin(GL_TRIANGLES);
    {
        for (auto pt : vecPts)
        {
            gl_point2f(pt);
        }
    }
    glEnd();
}

void GLFuncUtils::draw_rect(const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight)
{
    GLPoint2f ptTopRight(ptBottomRight.x, ptTopLeft.y);
    GLPoint2f ptBottomLeft(ptTopLeft.x, ptBottomRight.y);

    glBegin(GL_QUADS);
    {
        gl_point2f(ptTopLeft);
        gl_point2f(ptTopRight);
        gl_point2f(ptBottomRight);
        gl_point2f(ptBottomLeft);
    }
    glEnd();
}

void GLFuncUtils::draw_polygon(const QVector<GLPoint2f> &vecPts)
{
    glBegin(GL_POLYGON);
    {
        for (auto pt : vecPts)
        {
            gl_point2f(pt);
        }
    }
    glEnd();
}

void GLFuncUtils::draw_ellipse(const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight, GLenum mode)
{
    auto x = (ptTopLeft.x + ptBottomRight.x) / 2;
    auto y = (ptTopLeft.y + ptBottomRight.y) / 2;
    auto ptCenter = GLPoint2f(x, y);

    auto rx = (ptBottomRight.x - ptTopLeft.x) / 2;
    auto ry = (ptTopLeft.y - ptBottomRight.y) / 2;

    draw_ellipse(ptCenter, rx, ry, mode);
}

/**
 * @brief GLFuncUtils::draw_ellipse
 * @param ptCenter: center of the ellipse
 * @param rx: long axis
 * @param ry: short axis
 */
void GLFuncUtils::draw_ellipse(const GLPoint2f &ptCenter, GLfloat rx, GLfloat ry, GLenum mode)
{
    static const int count = 360;
    static const float angle_unit = 2 * PI / count;

    glBegin(mode);
    {
        for (int i = 0; i < count; ++i)
        {
            const float angle = angle_unit * i;
            GLfloat x = ptCenter.x + rx * cos(angle);
            GLfloat y = ptCenter.y + ry * sin(angle);

            glVertex2f(x, y);
        }
    }
    glEnd();
}

/**
 * @brief GLFuncUtils::draw_img, the texture is bound outside, before calling this method
 * @param
 * vecPts: vector of polygon points
 */
void GLFuncUtils::draw_img(const QVector<GLPoint2f> &vecPts)
{
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    {
        for (auto pt : vecPts)
        {
            glTexCoord2f(gl_coord_2_texture(pt.x), gl_coord_2_texture(pt.y));
            gl_point2f(pt);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void GLFuncUtils::draw_img(const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight)
{
    GLPoint2f ptTopRight(ptBottomRight.x, ptTopLeft.y);
    GLPoint2f ptBottomLeft(ptTopLeft.x, ptBottomRight.y);
    auto vecPts = {ptTopLeft, ptTopRight, ptBottomRight, ptBottomLeft};

    draw_img(vecPts);
}

void GLFuncUtils::draw_text(QPainter &p, const QRect &rcViewPort, const GLPoint2f &ptTopLeft, const QString &text)
{
    auto pt = gl_point_2_qpointf(ptTopLeft, rcViewPort);

    glDisable(GL_DEPTH_TEST);
    p.save();
    {
        p.drawText(pt, text);
    }
    p.restore();
    glEnable(GL_DEPTH_TEST);
}

void GLFuncUtils::draw_text(QPainter &p, const QRect &rcViewPort, const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight,
                            int flags, const QString &text)
{
    auto topLeft = gl_point_2_qpointf(ptTopLeft, rcViewPort);
    auto bottomRight = gl_point_2_qpointf(ptBottomRight, rcViewPort);
    auto rc = QRect(topLeft.toPoint(), bottomRight.toPoint());

    glDisable(GL_DEPTH_TEST);
    p.save();
    {
        p.drawText(rc, flags, text);
    }
    p.restore();
    glEnable(GL_DEPTH_TEST);
}

void GLFuncUtils::reset_color()
{
    glColor4f(1, 1, 1, 0);
}




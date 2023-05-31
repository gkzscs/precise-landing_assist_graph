#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <QVector>
#include <QColor>
#include <QPointF>
#include <QRectF>
#include <QPainter>

// pay attention to the sequence of `<gl/GL.h>`
#include <qopenglfunctions_4_5_compatibility.h>
#include <gl/GL.h>
#include <QOpenGLTexture>


#define DROP_ABNORMAL_DATA(data)   do { if (abs(data) > 100000) return; } while (0)


/**
 * @brief structs
 */

/**
 * @brief The GLPoint2f struct
 * @param
 * x: range of [-1, 1]
 * y: range of [-1, 1]
 */
struct GLPoint2f
{
    GLfloat x;
    GLfloat y;

    GLPoint2f(GLfloat tmpX = 0, GLfloat tmpY = 0)
        : x(tmpX), y(tmpY)
    {}
};

struct GLPoint3f
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

    GLPoint3f(GLfloat tmpX = 0, GLfloat tmpY = 0, GLfloat tmpZ = 0)
        : x(tmpX), y(tmpY), z(tmpZ)
    {}
};

/**
 * @brief The GLColor3f struct
 * @param
 * r: range of [0, 1]
 * g: range of [0, 1]
 * b: range of [0, 1]
 */
struct GLColor3f
{
    GLfloat r;
    GLfloat g;
    GLfloat b;

    GLColor3f(GLfloat tmpR = 0, GLfloat tmpG = 0, GLfloat tmpB = 0)
        : r(tmpR), g(tmpG), b(tmpB)
    {}
};

struct GLColor4f
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;

    GLColor4f(GLfloat tmpR = 0, GLfloat tmpG = 0, GLfloat tmpB = 0, GLfloat tmpA = 1)
        : r(tmpR), g(tmpG), b(tmpB), a(tmpA)
    {}
};

/**
 * @brief classes
 */
class GLFuncUtils : protected QOpenGLFunctions_4_5_Compatibility
{
public:
    QPointF gl_point_2_qpointf(const GLPoint2f &pt, const QRect &rcViewPort);
    GLPoint2f qpointf_2_gl_point(const QPointF &pt, const QRect &rcViewPort);

    QColor gl_color4f_2_qcolor(const GLColor4f &cl);
    GLColor4f qcolor_2_gl_color4f(const QColor &cl);

    QColor gl_color3f_2_qcolor(const GLColor3f &cl);
    GLColor3f qcolor_2_gl_color3f(const QColor &cl);

    GLfloat texture_coord_2_gl(GLfloat a);
    GLfloat gl_coord_2_texture(GLfloat a);

public:
    GLPoint2f scale_gl_pos(const GLPoint2f &pt, float f);

public:
    QPointF rect_center(const QRect &rc);

public:
    void gl_point2f(const GLPoint2f &pt);
    void gl_color3f(const GLColor3f &cl);
    void gl_color4f(const GLColor4f &cl);

    void gl_clear_qcolor(const QColor &cl);
    void gl_clear_color3f(const GLColor3f &cl);
    void gl_clear_color4f(const GLColor4f &cl);

public:
    void draw_point(const GLPoint2f &pt);
    void draw_line(const GLPoint2f &pt1, const GLPoint2f &pt2);
    void draw_lines(const QVector<GLPoint2f> &vecPts, GLenum mode);
    void draw_triangle(const QVector<GLPoint2f> &vecPts);
    void draw_rect(const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight);
    void draw_polygon(const QVector<GLPoint2f> &vecPts);
    void draw_ellipse(const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight, GLenum mode = GL_POLYGON);
    void draw_ellipse(const GLPoint2f &ptCenter, GLfloat rx, GLfloat ry, GLenum mode = GL_POLYGON);
    void draw_img(const QVector<GLPoint2f> &vecPts);
    void draw_img(const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight);
    void draw_text(QPainter &p, const QRect &rcViewPort, const GLPoint2f &ptTopLeft, const QString &text);
    void draw_text(QPainter &p, const QRect &rcViewPort, const GLPoint2f &ptTopLeft, const GLPoint2f &ptBottomRight,
                   int flags, const QString &text);

public:
    void reset_color();

};

#endif // GL_UTILS_H

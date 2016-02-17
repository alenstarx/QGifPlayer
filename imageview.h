#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <GL/glu.h>
#include <QtOpenGL>
#include <QGLWidget>

class ImageView : public QGLWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);

    ~ImageView();
    void AddImage(const char *frame, int w, int h, QImage::Format format);

protected:
    void resizeGL(int w, int h);

    void initializeGL();

    void paintGL();
#if 0
    //void keyPressEvent(QKeyEvent *event);

    //void timerEvent(QTimerEvent *event);
private:
    void loadGLTexture();

    void buildFont(); // 创建我们的字符显示列表

    void glPrint(GLint x, GLint y, char *string, int set);
#endif
private:
    void DrawImage(GLint tex, int x, int y, int w, int h);
    bool m_show_full_screen;

    GLuint m_base;
    GLuint m_texture[2];
    GLfloat m_cnt1;
    GLfloat m_cnt2;

    int _frame_count;
    int _frame_width;
    int _frame_height;
signals:

public slots:

};

#endif // IMAGEVIEW_H

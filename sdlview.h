#ifndef SDLVIEW_H
#define SDLVIEW_H

#include <QWidget>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SDLView : public QWidget
{
    Q_OBJECT
public:
    ~SDLView();
    explicit SDLView(QWidget *parent = 0);
    static bool initSDL();
    static void uninitSDL();

    void AddImage(const char *frame, int w, int h, QImage::Format format);

    bool bindSDL(void* wid);
    static bool startSDL();
    static bool stopSDL();


signals:

public slots:

protected:
    void resizeSDL(int w, int h);
    void paintSDL();

    void paintEvent(QPaintEvent * ev);

private:
    QWidget* _sdl;
    void DrawImage(int x, int y, int w, int h);
    //SDL_Window* _win;
    //SDL_Surface* _surface;
    int _frame_width;
    int _frame_height;
    int _frame_format;
    //char* _frame;
    //SDL_Surface* _frame;
};

#endif // SDLVIEW_H

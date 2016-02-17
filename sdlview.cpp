#include "sdlview.h"
#include "logdef.h"
#include <thread>
#include <chrono>

    static SDL_Surface* _frame = NULL;
    static SDL_Window* _win = NULL;
    static SDL_Surface* _surface = NULL;

SDLView::~SDLView()
{
    if(_win){
        SDL_DestroyWindow(_win);
    }
}

SDLView::SDLView(QWidget *parent) :
    QWidget(parent)
{
#if 0
    std::thread t([this](){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    LOGD("Create WindowFrom ID %p", this->winId());
    LOGD("Create WindowFrom ID");
    _win = SDL_CreateWindowFrom((void*)this->winId());
    if (! _win) {
        LOGE("SDL Error: '%s'\n", SDL_GetError());
    } else {
        _surface = SDL_GetWindowSurface(_win);
        if (_surface){
            startSDL();
        } else {
            LOGE("SDL Error: '%s'\n", SDL_GetError());
        }
    }
    });
    t.detach();
#endif
    _frame_width = 0;
    _frame_height = 0;
    _frame_format = 0;
    _frame = SDL_LoadBMP("button.bmp");
    if (!_frame){
        LOGE("SDL Error: '%s'\n", SDL_GetError());
    }
}

static int __sdl_done = SDL_TRUE;
bool SDLView::startSDL()
{
    if (__sdl_done){
        std::thread t([](){
            SDL_Event event;
            __sdl_done = SDL_FALSE;
            //while(!__sdl_done){
            while(1){
            while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                LOGD("SDL_KEYDOWN");
                break;
            case SDL_WINDOWEVENT:
                LOGD("SDL_WINDOWEVENT");
                switch (event.window.event) {
                case SDL_WINDOWEVENT_EXPOSED:
                    //DrawImage(0, 0, _frame_width, _frame_height);
                        if(_frame){
        SDL_Rect dstRect = {0, 0, _frame->w, _frame->h};
        SDL_BlitSurface(_frame, NULL, _surface, NULL);

        if (SDL_UpdateWindowSurfaceRects(_win, NULL, 1) != 0){
            LOGE("SDL Error: %s", SDL_GetError());
        }
    }

                    break;
                default:
                    break;
                }
                break;
            case SDL_QUIT:
                __sdl_done = SDL_TRUE;
            LOGD("SDL_QUIT");
                break;
            }
            }
            }
            LOGD("SDL loop end");
        });
        t.detach();
    }
    return true;
}

bool SDLView::stopSDL()
{
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

void SDLView::resizeSDL(int w, int h)
{
    SDL_Event event;
    event.type = SDL_WINDOWEVENT;
    event.window.event = SDL_WINDOWEVENT_RESIZED;
    event.window.windowID = SDL_GetWindowID(_win);
    event.window.data1 = w;
    event.window.data2 = h;
    SDL_PushEvent(&event);
}

void SDLView::paintSDL()
{
    SDL_Event event;
    event.type = SDL_WINDOWEVENT;
    event.window.event = SDL_WINDOWEVENT_EXPOSED;
    event.window.windowID = SDL_GetWindowID(_win);
    SDL_PushEvent(&event);
}

void SDLView::paintEvent(QPaintEvent *ev)
{
    paintSDL();
}

void SDLView::DrawImage(int x, int y, int w, int h)
{
    if(_surface){
        SDL_Rect dstRect = {x, y, w, h};
        SDL_BlitSurface(_frame, NULL, _surface, &dstRect);

        if (SDL_UpdateWindowSurfaceRects(_win, &dstRect, 1) != 0){

        }
    }
}

bool SDLView::initSDL()
{
    int rc = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE);
    if (rc != 0)
    {
        LOGE("SDL Error: '%s'\n", SDL_GetError());
        return false;
    }
    return true;
}

void SDLView::uninitSDL()
{
    SDL_Quit();
}

void SDLView::AddImage(const char *frame, int w, int h, QImage::Format format)
{
    _frame_width = w;
    _frame_height = h;
    _frame_format = (int)format;
    //_frame = (char*)frame;
    _frame = SDL_CreateRGBSurfaceFrom(_frame, w, h, 32, w * 4, 0xff000000, 0x00ff0000, 0x0000ff00, 0x00);
    if (!_frame){
        LOGE("SDL Error: '%s'\n", SDL_GetError());
    }
}

bool SDLView::bindSDL(void *wid)
{
     LOGD("Create WindowFrom ID %p", wid);
    LOGD("Create WindowFrom ID");
    _win = SDL_CreateWindowFrom(wid);
    if (! _win) {
        LOGE("SDL Error: '%s'\n", SDL_GetError());
    } else {
        _surface = SDL_GetWindowSurface(_win);
        if (_surface){
            startSDL();
        } else {
            LOGE("SDL Error: '%s'\n", SDL_GetError());
        }
    }
    return true;
}

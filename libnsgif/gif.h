#ifndef GIF_H
#define GIF_H

#include <vector>
#include <iostream>

class GifFrame
{
public:
    GifFrame(unsigned char* ptr, int delay, bool display = true);
    const char* data(){return (const char*)_frame;}
    bool display(){return _display;}
    int delay(){return _delay;}
private:
    unsigned char* _frame;
    bool _display;
    int _delay;
};

class Gif
{
public:
    Gif(const char* file);
    ~Gif();
    bool load(const char* file);
    bool load(std::string& file);
    int status();

    int width;
    int height;
    int frame_count;
    int position;
    int delay;
    int loop_count;

    GifFrame* get(int idx = 0);
private:
    std::string _file;
    unsigned char* _buffer;
    size_t _buffer_size;
    int _state;

    std::vector<GifFrame*> _frames;
};

#endif // GIF_H

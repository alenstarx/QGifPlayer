#include "gif.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "libnsgif/libnsgif.h"
#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef DEBUG
#if 0 //def WIN32
#define MAXDUGBUFF 512
#include <debugapi.h>
#define D(fmt, ...)    do { char buf[MAXDUGBUFF]; memset(buf, 0x00, MAXDUGBUFF); sprintf(buf, "%s (%d) DEBUG: ", __FILE__, __LINE__); OutputDebugStringA(buf); memset(buf, 0x00, MAXDUGBUFF); sprintf(buf, fmt ## "\r\n", ##__VA_ARGS__); OutputDebugStringA(buf); }while (0)
#else
#define D(fmt, ...) do{printf("%s (%d) DEBUG: ", __FILE__, __LINE__);printf(fmt , ## __VA_ARGS__);printf("\n");}while(0)
#endif
#else
#define D(fmt, ...)
#endif


unsigned char *load_file(const char *path, size_t *data_size)
{
    FILE *fd;
    struct stat sb;
    unsigned char *buffer;
    size_t size;
    size_t n;

    fd = fopen(path, "rb");
    if (!fd) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    if (stat(path, &sb)) {
        perror(path);
        exit(EXIT_FAILURE);
    }
    size = sb.st_size;

    buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        fprintf(stderr, "Unable to allocate %lld bytes\n",
            (long long)size);
        exit(EXIT_FAILURE);
    }

    n = fread(buffer, 1, size, fd);
    if (n != size) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    fclose(fd);

    *data_size = size;
    return buffer;
}

void warning(const char *context, gif_result code)
{
    D("%s failed: ", context);
    switch (code)
    {
    case GIF_INSUFFICIENT_FRAME_DATA:
        D("GIF_INSUFFICIENT_FRAME_DATA");
        break;
    case GIF_FRAME_DATA_ERROR:
        D("GIF_FRAME_DATA_ERROR");
        break;
    case GIF_INSUFFICIENT_DATA:
        D("GIF_INSUFFICIENT_DATA");
        break;
    case GIF_DATA_ERROR:
        D("GIF_DATA_ERROR");
        break;
    case GIF_INSUFFICIENT_MEMORY:
        D("GIF_INSUFFICIENT_MEMORY");
        break;
    default:
        D("unknown code %i", code);
        break;
    }
}


void *bitmap_create(int width, int height)
{
    return calloc(width * height, 4);
}


void bitmap_set_opaque(void *bitmap, bool opaque)
{
    (void)opaque;  /* unused */
    assert(bitmap);
}


bool bitmap_test_opaque(void *bitmap)
{
    assert(bitmap);
    return false;
}


unsigned char *bitmap_get_buffer(void *bitmap)
{
    assert(bitmap);
    return (unsigned char*)bitmap;
}


void bitmap_destroy(void *bitmap)
{
    assert(bitmap);
    free(bitmap);
}


void bitmap_modified(void *bitmap)
{
    assert(bitmap);
    return;
}



Gif::Gif(const char *file) : _buffer(NULL), _buffer_size(0)
{
    _frames.clear();
    if(file){
        _file = std::string(file);
    }
    if(! load(_file)){
        D("load %s error", _file.c_str());
        _state = -1;
    } else {
       _state = 0;
    }
}

Gif::~Gif()
{
    for(std::vector<GifFrame*>::iterator it = this->_frames.begin(); it != this->_frames.end(); it++){
        delete *it;
    }
    if (_buffer){
       free(_buffer);
    }
    _frames.clear();
}

bool Gif::load(const char* file)
{
    this->_file = std::string(file);
    return this->load(this->_file);
}

bool Gif::load(std::string &file)
{
    gif_bitmap_callback_vt bitmap_callbacks = {
        bitmap_create,
        bitmap_destroy,
        bitmap_get_buffer,
        bitmap_set_opaque,
        bitmap_test_opaque,
        bitmap_modified
    };
    if(file.empty()){
        return false;
    }

    gif_animation* gif = (gif_animation*)calloc(1, sizeof(gif_animation));
    size_t size;
    gif_result code;

    /* create our gif animation */
    gif_create(gif, &bitmap_callbacks);

    /* load file into memory */
    unsigned char *data = load_file(file.c_str(), &size);

    /* begin decoding */
    do {
        code = gif_initialise(gif, size, data);
        if (code != GIF_OK && code != GIF_WORKING) {
            warning("gif_initialise", code);
            free(gif);
            free(data);
            return false;
        }
    } while (code != GIF_OK);

    D("# width                %u \n", gif->width);
    D("# height               %u \n", gif->height);
    D("# frame_count          %u \n", gif->frame_count);
    D("# frame_count_partial  %u \n", gif->frame_count_partial);
    D("# loop_count           %u \n", gif->loop_count);
    D("# buffer_size          %u \n", gif->buffer_size);

    if (_buffer && (_buffer_size < (gif->width * gif->height * gif->frame_count * 4))){
        _buffer = (unsigned char*)realloc(_buffer, gif->width * gif->height * gif->frame_count * 4);
        _buffer_size = gif->width * gif->height * gif->frame_count * 4;
    } else {
        _buffer = (unsigned char*)malloc(gif->width * gif->height * gif->frame_count * 4);
        _buffer_size = gif->width * gif->height * gif->frame_count * 4;
    }
    this->width = gif->width;
    this->height = gif->height;
    this->frame_count = gif->frame_count;
    this->loop_count = gif->loop_count;

    /* decode the frames */
    for (int i = 0; i != gif->frame_count; i++) {
        unsigned char *image;

        code = gif_decode_frame(gif, i);
        if (code != GIF_OK)
            warning("gif_decode_frame", code);

        D("# frame %u: delay %d\n", i, gif->frames[i].frame_delay * 10);
        image = (unsigned char *)gif->frame_image;
#if 0
        unsigned int row, col;
        unsigned char* buf = (unsigned char*)malloc(gif.width * gif.height * 3);
        argb2rgb(image, gif.width * gif.height * 4, buf, gif.width * gif.height * 3);
        char bmp_name[128] = { 0x00 };
        sprintf(bmp_name, "gif%03d.bmp", i);
        save_bmp(bmp_name, buf, gif.width, gif.height, 24);
        free(buf);
#else
        memcpy(_buffer + i * gif->width * gif->height * 4, image, gif->width * gif->height * 4);
        GifFrame* f = new GifFrame(_buffer + i * gif->width * gif->height * 4, gif->frames[i].frame_delay * 10, gif->frames[i].display);
        this->_frames.push_back(f);
#endif
    }

    /* clean up */
    gif_finalise(gif);
    free(data);
    free(gif);

    _state = 0;
    return true;
}

int Gif::status()
{
    return _state;
}

GifFrame *Gif::get(int idx)
{
    if(idx < _frames.size()){
        return _frames.at(idx);
    }
    return NULL;
}


GifFrame::GifFrame(unsigned char *ptr, int delay, bool display) : _frame(ptr), _delay(delay), _display(display)
{
}

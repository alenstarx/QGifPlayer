#ifndef LOGDEF_H
#define LOGDEF_H
#include <stdio.h>
#ifdef DEBUG
#define LOGE(fmt, ...) do{fprintf(stderr, "%s(%3d) [E]: ", __FILE__, __LINE__);fprintf(stderr, fmt,  ##__VA_ARGS__), fprintf(stderr, "\n");}while(0)
#define LOGW(fmt, ...) do{fprintf(stderr, "%s(%3d) [W]: ", __FILE__, __LINE__);fprintf(stderr, fmt,  ##__VA_ARGS__), fprintf(stderr, "\n");}while(0)
#define LOGI(fmt, ...) do{fprintf(stderr, "%s(%3d) [I]: ", __FILE__, __LINE__); fprintf(stderr, fmt,  ##__VA_ARGS__), fprintf(stderr, "\n");}while(0)
#define LOGD(fmt, ...) do{fprintf(stderr, "%s(%3d) [D]: ", __FILE__, __LINE__); fprintf(stderr, fmt,  ##__VA_ARGS__), fprintf(stderr, "\n");}while(0)
#else
#define LOGE(fmt, ...) do{fprintf(stderr, "%s(%3d) [E]: ", __FILE__, __LINE__);fprintf(stderr, fmt,  ##__VA_ARGS__), fprintf(stderr, "\n");}while(0)
#define LOGW(fmt, ...) do{fprintf(stderr, "%s(%3d) [W]: ", __FILE__, __LINE__);fprintf(stderr, fmt,  ##__VA_ARGS__), fprintf(stderr, "\n");}while(0)
#define LOGI(fmt, ...) do{fprintf(stderr, "%s(%3d) [I]: ", __FILE__, __LINE__); fprintf(stderr, fmt,  ##__VA_ARGS__), fprintf(stderr, "\n");}while(0)
#define LOGD(fmt, ...) do{}while(0)
#endif


#endif // LOGDEF_H

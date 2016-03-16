#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "libnsgif/gif.h"
#include "imageview.h"
#include "sdlview.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_file_btn_clicked();

private:
    Ui::Widget *ui;
    Gif* _gif;
    ImageView* _image;
    //SDLView* _image;
};

#endif // WIDGET_H

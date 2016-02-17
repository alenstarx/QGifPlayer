#include "widget.h"
#include "ui_widget.h"
#include "logdef.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //SDLView::initSDL();
    ui->setupUi(this);
    _gif = new Gif(NULL);
    //_image = new ImageView(this);
    _image = new SDLView(this);
    this->ui->main_layout->addWidget(this->_image);
    int wid = _image->winId();
    SDLView::initSDL();
    _image->bindSDL((void*)_image->window()->winId());
    LOGD("WinId %p", this->_image->winId());
}

Widget::~Widget()
{
    delete ui;
    //delete _image;
    delete _gif;
    SDLView::stopSDL();
    SDLView::uninitSDL();
}

void Widget::on_file_btn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Gif"), qgetenv("HOME"), tr("Gif Files(*.gif)"));
    if(path.length() < 1){
        return;
    } else {
        if (QFile::exists(path)){
            if(! _gif->load(path.toUtf8().data())){
                LOGE("gif load error");
            } else {
                LOGD("gif load success: %d, %d x %d", _gif->frame_count, _gif->width, _gif->height);
                //QSize s(_gif->width, _gif->height);
                //this->_image->setMinimumSize(s);
                this->_image->AddImage(_gif->get(16)->data(), _gif->width, _gif->height, QImage::Format_RGB888);
            }
        }
    }
}

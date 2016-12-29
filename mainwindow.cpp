#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QMatrix>
#include <QDebug>
#include <QMovie>
#include <QString>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), curPath("./"), curDir("./"),
    i(0), model(new QFileSystemModel),
    level(1.0)
{
    ui->setupUi(this);

    //获得屏幕尺寸
    QDesktopWidget* desktopWidget = QApplication::desktop();
    screenRect = desktopWidget->screenGeometry();

    //过滤器
    QStringList filterList;
    filterList << "*.gif" << "*.jpg" << "*.png" << "*.bmp";

    //过滤掉其他东西，除了文件
    curDir.setFilter(QDir::Files);
    curDir.setNameFilters(filterList);
    fileInfoList = curDir.entryInfoList();
    //不为空则将当前文件设为列表第一个文件
    if ( !fileInfoList.isEmpty() ) {
        curFileName = fileInfoList.at(0).fileName();
    }


    model->setNameFilterDisables(false);
    model->setFilter(QDir::Dirs|QDir::Drives|QDir::Files|QDir::NoDotAndDotDot);
    //禁止用户移动文件
    ui->listView->setMovement(QListView::Static);
    //以缩略图的形式展现当前可以访问的文件
    ui->listView->setViewMode(QListView::IconMode);
    //设置列表大小
    ui->listView->setGridSize(QSize(100,100));
    //当窗口改变的时候，文件自适应
    ui->listView->setResizeMode(QListView::Adjust);
    ui->listView->setModel(model);

    //设置过滤器
    model->setNameFilters(filterList);
    ui->listView->setRootIndex(model->setRootPath(curPath));

    //居中显示
    ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    //将label装入scrollarea
    //ui->scrollArea->setWidget(ui->label);

    //连接信号和槽
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(fileChanged(QModelIndex)));

    showImg();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::showImg
 * 显示图片
 */
void MainWindow::showImg()
{
    QStringList style = curFileName.split(".");

    QPixmap fitpixmap;
    double bili;    //图片比例因子

    QMatrix matrix;

    //当前文件不存在
    if (curFileName == "") {
        return;
    }
    //图片为动图，用类似于视频的方式播放
    else if (style.last().toLower() == "gif") {
        QMovie *movie = new QMovie(curPath + "/" + curFileName);
        ui->label->resize(level*500, level*300);
        ui->label->move((ui->widget->width() - ui->label->width()) / 2,
                        (ui->widget->height() - ui->label->height()) / 2);
        ui->label->setMovie(movie);
        movie->start();

    } else {
        //加载图像
        pix.load(curPath + "/" + curFileName);

        //保存长宽比例
        bili = (double)pix.width()/pix.height();

        //图像和组件的大小关系
        if (pix.width() <= ui->widget->width() &&
                pix.height() <= ui->widget->height()) {
            fitpixmap=pix.scaled(level*pix.width(), level*pix.height(),
                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        } else if (pix.width() > ui->widget->width() &&
                   pix.height() <= ui->widget->height()) {
            //图片自适应widget宽度
            fitpixmap=pix.scaled(level*ui->widget->width(),
                                 level*ui->widget->width()/bili,
                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        } else if (pix.width() <= ui->widget->width() &&
                   pix.height() > ui->widget->height()) {
            //图片自适应height高度
            fitpixmap=pix.scaled(level*ui->widget->height()*bili,
                                         level*ui->widget->height(),
                                         Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        }
        //图像自身长宽的比例
        else if (pix.width() < pix.height()) {
            fitpixmap=pix.scaled(level*ui->widget->height()*bili,
                                         level*ui->widget->height(),
                                         Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        } else if (pix.width() > pix.height()) {
            fitpixmap=pix.scaled(level*ui->widget->width(),
                                         level*ui->widget->width()/bili,
                                         Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        }
        //重设label的大小
        ui->label->resize(fitpixmap.width(), fitpixmap.height());
        //平移label
        ui->label->move((ui->widget->width() - ui->label->width()) / 2,
                        (ui->widget->height() - ui->label->height()) / 2);
        fitpixmap.transformed(matrix, Qt::SmoothTransformation);
        ui->label->setPixmap(fitpixmap);
    }
}

/**
 * @brief MainWindow::getIndex
 * @param string 图片名称
 * @return 图片所指向的下标
 */
int MainWindow::getIndex(QString string)
{
    if (fileInfoList.size() <= 0) {
        return 0;
    }
    for (int j = 0; j < fileInfoList.size(); j++) {
        if (fileInfoList.at(j).fileName() == string) {
            return j;
        }
    }

    return 0;
}

/**
 * @brief MainWindow::resizeEvent
 * @param event
 * 重写窗口改变事件
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    showImg();
}

void MainWindow::fileChanged(const QModelIndex &index)
{
    //获得文件名
    curFileName = index.data(Qt::DisplayRole).toString();
    i = getIndex(curFileName);

    showImg();
}

void MainWindow::on_closeWin_action_triggered()
{
    this->close();
}


/*
 * 通过自动关联，当点击打开文件的时候会自动打开filedialog，
 * 然后用户选择一张图片，同时我们会获取此图片所在的绝对路径curPath
 * 以及图片的名称fileName，然后我们会搜索当前路径下的所有图片文件
 *
 */
void MainWindow::on_open_action_triggered()
{
    //获取图片的路径
    curFileName = QFileDialog::getOpenFileName(this, tr("打开图片"), curPath,
                                             tr("图片文件(*.png *.jpg *.bmp *.gif)"));

    if (curFileName == NULL) {
        return;
    }

    //将图片的相关路径信息存储到fileInfo中
    QFileInfo fileInfo = QFileInfo(curFileName);
    //将图片名称赋给filename
    curFileName = fileInfo.fileName();
    //将图片路径赋给curpath
    curPath = fileInfo.absolutePath();
    //并重新设置路径
    curDir.setPath(curPath);
    //图片信息
    fileInfoList = curDir.entryInfoList();
    //获得图片位置
    i = getIndex(curFileName);

    ui->listView->setRootIndex(model->setRootPath(curPath));

    showImg();
}

void MainWindow::on_closePic_action_triggered()
{
    ui->listView->setRootIndex(model->setRootPath(NULL));
    pix.load(":/pic/background.png");
    //showImg();
    ui->label->resize(pix.width(), pix.height());
    ui->label->move((ui->widget->width() - ui->label->width()) / 2,
                    (ui->widget->height() - ui->label->height()) / 2);
    ui->label->setPixmap(pix);

    fileInfoList.clear();
}

void MainWindow::on_up_triggered()
{
    if (fileInfoList.isEmpty()) {
        return;
    }

    if (i == 0) {
        i = fileInfoList.size()-1;
    } else {
        i--;
    }

    curFileName = fileInfoList.at(i).fileName();
    showImg();
}
void MainWindow::on_down_triggered()
{
    if (fileInfoList.isEmpty()) {
        return;
    }

    if (i == fileInfoList.size()-1) {
        i = 0;
    } else {
        i++;
    }

    curFileName = fileInfoList.at(i).fileName();
    showImg();
}

void MainWindow::on_zoomin_triggered()
{
    level += 0.05;

    showImg();
}

void MainWindow::on_zoomout_triggered()
{
    level -= 0.05;
    showImg();
}

void MainWindow::on_restore_triggered()
{
    level = 1.0;
    showImg();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QLabel>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QPixmap>
#include <QRect>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void showImg();

    int getIndex(QString string);

protected:
    void resizeEvent(QResizeEvent * event);

    //槽
private slots:
    //关闭窗口
    void on_closeWin_action_triggered();
    //打开图片
    void on_open_action_triggered();
    //文件改变
    void fileChanged(const QModelIndex &index);
    //关闭图片
    void on_closePic_action_triggered();
    //放大
    void on_zoomin_triggered();
    //缩小
    void on_zoomout_triggered();
    //上一张
    void on_up_triggered();
    //下一张
    void on_down_triggered();
    //还原
    void on_restore_triggered();

private:
    Ui::MainWindow *ui;


    int i; //用来判断第几张


    QString curPath; //当前路径
    QString curFileName; //当前文件名
    QDir curDir; //当前文件夹
    QFileInfoList fileInfoList;  //图像列表
    QFileSystemModel *model; //给listview使用的过滤器
    QPixmap pix;   //图像


    double level;//放大系数

    QRect screenRect; //屏幕尺寸

};

#endif // MAINWINDOW_H

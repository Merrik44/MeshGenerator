#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void progressBarUpdate(int Value);
    
private slots:
    void on_subdivisionSlider_valueChanged(int value);



    void on_loadObjButton_clicked();

    void on_loadLstButton_clicked();

    void on_generateMeshButton_clicked();

    void on_countSlider_valueChanged(int value);

    void on_exportObjButton_clicked();

    void on_texCheckBox_toggled(bool checked);

    void on_subDivPlus_clicked();

    void on_subDivPlus_2_clicked();

    void on_meshCheckBox_2_toggled(bool checked);

    void on_meshCheckBox_toggled(bool checked);

    void on_wireFrameCheckBox_toggled(bool checked);

    void on_segmentSlider_valueChanged(int value);

    void on_meshCheckBox_3_toggled(bool checked);

    void on_simplifyCheckBox_toggled(bool checked);

    void on_simplifyCheckBox_2_toggled(bool checked);

    void on_loadLstButton_2_clicked();

    void on_loadLstButton_3_clicked();

    void on_segmentSlider_2_valueChanged(int value);

    void on_simplifyCheckBox_3_clicked();

    void on_simplifyCheckBox_3_toggled(bool checked);

    void on_simplifyCheckBox_4_toggled(bool checked);

    void on_simplifyCheckBox_5_toggled(bool checked);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    GLWidget* glWidget;
};

#endif // MAINWINDOW_H

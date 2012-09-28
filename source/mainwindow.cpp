#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sstream"
#include "qfiledialog.h"
#include "meshgenerator.h"

extern MainWindow* mainWindowRef;
extern int Count;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mainWindowRef = this;
    ui->setupUi(this);

    glWidget = new GLWidget;
    ui->glLayout->addWidget( glWidget );

    Count = ui->countSlider->value();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_subdivisionSlider_valueChanged(int value)
{

     glWidget->setSubdivisionLevel(value );

    std::stringstream ss;
    ss << value;
    ui->subdLevelLabel->setText(ss.str().c_str());

    std::stringstream ss2;
    int faces = glWidget->model->GetFaceCount();
    ss2 << faces;
    ui->faceCountLabel->setText(ss2.str().c_str());
}

void MainWindow::on_loadObjButton_clicked()
{
     std::string fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../obj models",  tr("Files (*.obj*)") ).toStdString();
     if( fileName != "")
        glWidget->LoadModel(fileName);
   // std::cout << fileName.toStdString() << std::endl;

}

void MainWindow::on_loadLstButton_clicked()
{
    QFileDialog loader;
    std::string fileName = loader.getOpenFileName(this, tr("Open File"), "../lst files",  tr("Files (*.lst*)") ).toStdString();
    this->repaint();
    loader.close();
    if( fileName != "")
       glWidget->LoadLST(fileName);

    glWidget->GenerateMeshFromLST();
}

void MainWindow::on_generateMeshButton_clicked()
{
    glWidget->GenerateMeshFromLST();
}


void MainWindow::on_countSlider_valueChanged(int value)
{
    std::stringstream ss;
    ss << value;
    ui->countLabel->setText(ss.str().c_str());

      DebugClear();
    Count = value;
    glWidget->GenerateMeshFromLST();



}


void MainWindow::progressBarUpdate(int Value)
 {
     ui->progressBar->setValue( Value );
     if( Value >= 100 )
         ui->progressBar->setVisible(false);
     else
          ui->progressBar->setVisible(true);
 }

void MainWindow::on_exportObjButton_clicked()
{
    std::string fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "../obj models",  tr("Files (*.obj*)") ).toStdString();
    if( fileName != "")
        glWidget->ExportModelAsObj(fileName+".obj");
}


extern bool display4Texture;
void MainWindow::on_texCheckBox_toggled(bool checked)
{
    display4Texture = checked;
    glWidget->updateGL();
}

void MainWindow::on_subDivPlus_clicked()
{
    ui->subdivisionSlider->setValue(ui->subdivisionSlider->value()+ 1);
}

void MainWindow::on_subDivPlus_2_clicked()
{
    ui->subdivisionSlider->setValue(ui->subdivisionSlider->value()- 1);
}

extern bool displayGraph;
void MainWindow::on_meshCheckBox_2_toggled(bool checked)
{
    displayGraph = checked;
    glWidget->updateGL();
}


extern bool displayMesh;
void MainWindow::on_meshCheckBox_toggled(bool checked)
{
    displayMesh = checked;
    glWidget->updateGL();
}

extern bool displayWireFrame;
void MainWindow::on_wireFrameCheckBox_toggled(bool checked)
{
    displayWireFrame = checked;
    glWidget->updateGL();
}

extern float SubDivControl;
void MainWindow::on_segmentSlider_valueChanged(int value)
{
    SubDivControl = 1.0f - (float)value/(float)ui->segmentSlider->maximum();
    SubDivControl /=2.0f;

    glWidget->GenerateMeshFromLST();
    glWidget->updateGL();
}

extern bool tightJoints;
void MainWindow::on_meshCheckBox_3_toggled(bool checked)
{
    tightJoints = checked;
    glWidget->GenerateMeshFromLST();
    glWidget->updateGL();
}

extern bool simplifyGraph;
void MainWindow::on_simplifyCheckBox_toggled(bool checked)
{
    simplifyGraph = checked;
    glWidget->reloadLST();
    glWidget->GenerateMeshFromLST();
    glWidget->updateGL();
}

extern bool enableProgressControl;
void MainWindow::on_simplifyCheckBox_2_toggled(bool checked)
{
    enableProgressControl = checked;
    glWidget->GenerateMeshFromLST();

}

void MainWindow::on_loadLstButton_2_clicked()
{
    glWidget->listIndex++;
    glWidget->listIndex %= glWidget->lstFileList.count();
    QString name = glWidget->lstFileList.value(glWidget->listIndex);

    glWidget->LoadLST( "../lst files/"+name.toStdString() );
    glWidget->GenerateMeshFromLST();
    glWidget->updateGL();
}

void MainWindow::on_loadLstButton_3_clicked()
{
    glWidget->listIndex--;
     glWidget->listIndex += glWidget->lstFileList.count();
    glWidget->listIndex %= glWidget->lstFileList.count();
    QString name = glWidget->lstFileList.value(glWidget->listIndex);
    this->setWindowTitle(name);
    glWidget->LoadLST( "../lst files/"+name.toStdString() );
    glWidget->GenerateMeshFromLST();
    glWidget->updateGL();
}

extern int noOfSides;
void MainWindow::on_segmentSlider_2_valueChanged(int value)
{
    noOfSides = value;
    glWidget->GenerateMeshFromLST();
    glWidget->updateGL();
}


void MainWindow::on_simplifyCheckBox_3_clicked()
{

}

extern bool retriangulate;

void MainWindow::on_simplifyCheckBox_3_toggled(bool checked)
{
    retriangulate = checked;
     glWidget->GenerateMeshFromLST();
}

extern bool alternatingTriangulation;
void MainWindow::on_simplifyCheckBox_4_toggled(bool checked)
{
    alternatingTriangulation = checked;
    glWidget->GenerateMeshFromLST();

}

extern bool smoothShading;
void MainWindow::on_simplifyCheckBox_5_toggled(bool checked)
{
    smoothShading = checked;
    glWidget->updateGL();
}

void MainWindow::on_pushButton_clicked()
{
    glWidget->ResetCamera();
    glWidget->updateGL();
}

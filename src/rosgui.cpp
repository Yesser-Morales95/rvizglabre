#include <QMessageBox>
#include <QMdiArea>
#include <QtGui>
#include <QMainWindow>
#include <QTreeWidget>
#include <QStyledItemDelegate>
#include <QItemDelegate>
#include <QList>
#include <QProcess>
#include <QVector>
#include <QString>
#include <QTemporaryDir>
#include <QCheckBox>
#include <QTextEdit>
#include <QTextStream>
#include <QTextCursor>
#include <QFileDialog>
#include <QFile>
#include <QTableWidget>
#include <QTableWidgetItem>




#include "rosgui.h"
//#include "tuning_ui_add.h"
#include <ui_rosgui.h>

//#include "include/ui_rosgui.h"
//#include "rvizg.h"


//#include "include/secondwindow.h"
////#include "include/ui_secondwindow.h"
//#include "include/thirdwindow.h"
////#include "include/ui_thirdwindow.h"
//#include "include/fourth.h"
////#include "include/ui_fourth.h"
////#include "secondwindow.h"


#include <ros/ros.h>


////robot editor
#include <cstdlib>
#include <fstream>

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <XmlRpcValue.h>
#include <sensor_msgs/JointState.h>
#include <kdl_parser/kdl_parser.hpp>
#include <robot_state_publisher/robot_state_publisher.h>

#include <sstream>
////robot editor




#include "std_msgs/MultiArrayLayout.h"
//#include "std_msgs/Int16MultiArray"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Int32MultiArray.h"
const double FACTOR = 1;
double ToG    = 57.295779513;
const double resetv  = 0;
static bool Ws = false;
bool DHb = false;


ROSGUI::ROSGUI(QWidget *parent)
  :QMainWindow(parent),
  main_window_ui_(new Ui::ROSGUI)

{
// proc = new QProcess();     //   <QtGui>
//  proc->start("gnome-terminal --geometry=50x10-0-10 -x bash -c \"roscore\" ");


    main_window_ui_->setupUi(this);

   // qnode->on_init();

    mRviz = new MyViz(main_window_ui_->mdiArea);

    main_window_ui_->mdiArea->addSubWindow(mRviz, Qt::FramelessWindowHint); //mdiarea various screen

   // mRviz->adjustSize();
//    mRviz->setMaximumHeight();

    mRviz->showMaximized();

//    showModel = new rvizMain(main_window_ui_->mdiArea);
    // Add rviz to mdiArea as a subwindow and maximize it
//    main_window_ui_->mdiArea->addSubWindow(showModel, Qt::FramelessWindowHint); // FramelessWindowHint removes close, minimize and maximize title bar
//    showModel->showMaximized();

    QPixmap pix(":/images/img/Uni.jpg");
    

    main_window_ui_->label_3->setPixmap(pix.scaled(main_window_ui_->label_3->size(),Qt::KeepAspectRatio));
    main_window_ui_->statusBar->showMessage(tr("Listo, Esperando Modelo a Analizar"));
//    main_window_ui_->groupBox->setEnabled(false);

    // readJntLimitsFromROSParamURDF(&n);

    //Mostrando ventanas emergentes//

    //CD
//    secwindow = new QMainWindow();
//    Ui::SecondWindow SecondWindowUI;
//    SecondWindowUI.setupUi(secwindow);
//    QPixmap pix2(":/images/img/CD.png");
//    SecondWindowUI.label->setPixmap(pix2);
//    //CI
//    thwindow = new QMainWindow();
//    Ui::thirdwindow thWindowUI;
//    thWindowUI.setupUi(thwindow);
//    QPixmap pix3(":/images/img/CI.png");
//    thWindowUI.label_2->setPixmap(pix3);
//    //DH
//    fourwindow = new QMainWindow();
//    Ui::fourth frWindowUI;
//    frWindowUI.setupUi(fourwindow);
//    QPixmap pix4(":/images/img/dhparameter.png");
//    frWindowUI.label_2->setPixmap(pix4);

    //SIGNAL and SLOTS
    //Acciones de la GUI
//    QObject::connect(this, SIGNAL(statusTool(int )),showModel, SLOT(setTool(int )));
    connect(main_window_ui_->actionOpen,   SIGNAL(triggered()), this, SLOT(on_actionOpen_triggered()));
    connect(main_window_ui_->pushButton,   SIGNAL(triggered()),   this, SLOT(on_pushButton_clicked()));
//    connect(main_window_ui_.pushButton_2, SIGNAL(clicked()),   this, SLOT(on_pushButton_2_clicked()));
//    connect(main_window_ui_.pushButton_4, SIGNAL(clicked()),   this, SLOT(on_pushButton_4_clicked()));
//    connect(SecondWindowUI.pushButton,    SIGNAL(clicked()),   this, SLOT(on_pushButton_SW_clicked()));
//    connect(thWindowUI.pushButton,        SIGNAL(clicked()),   this, SLOT(on_pushButton_SW_clicked()));
//    connect(frWindowUI.pushButton,        SIGNAL(clicked()),   this, SLOT(on_pushButton_SW_clicked()));

    //Cinematica Inversa
    //Execute IK
    connect(main_window_ui_->pushIK,     SIGNAL(clicked()),SLOT(executeIK()));
    //Execute IK
    connect(main_window_ui_->xSlider,      SIGNAL(valueChanged(int)), SLOT(updateSpinboxes()));
    connect(main_window_ui_->ySlider,      SIGNAL(valueChanged(int)), SLOT(updateSpinboxes()));
    connect(main_window_ui_->zSlider,      SIGNAL(valueChanged(int)), SLOT(updateSpinboxes()));
    connect(main_window_ui_->rollSlider,   SIGNAL(valueChanged(int)), SLOT(updateSpinboxes()));
    connect(main_window_ui_->pitchSlider,  SIGNAL(valueChanged(int)), SLOT(updateSpinboxes()));
    connect(main_window_ui_->yawSlider,    SIGNAL(valueChanged(int)), SLOT(updateSpinboxes()));

    connect(main_window_ui_->xBox,         SIGNAL(valueChanged(double)), SLOT(updateSlider()));
    connect(main_window_ui_->yBox,         SIGNAL(valueChanged(double)), SLOT(updateSlider()));
    connect(main_window_ui_->zBox,         SIGNAL(valueChanged(double)), SLOT(updateSlider()));
    connect(main_window_ui_->rollBox,      SIGNAL(valueChanged(double)), SLOT(updateSlider()));
    connect(main_window_ui_->pitchBox,     SIGNAL(valueChanged(double)), SLOT(updateSlider()));
    connect(main_window_ui_->yawBox,       SIGNAL(valueChanged(double)), SLOT(updateSlider()));
    //Cinematica Inversa

    //Control de dialer y spinbox activos

        //Basic Joints
 //   connect(main_window_ui_->checkBox6DOFs_3, SIGNAL(toggled(bool)), SLOT(on_1DOF()));   //airplane
    connect(main_window_ui_->checkBoxPrismatic, SIGNAL(toggled(bool)), SLOT(on_1DOF()));
 //   connect(main_window_ui_->checkBoxRev3D, SIGNAL(toggled(bool)), SLOT(on_1DOF()));   //cube


    // Basic robots
    connect(main_window_ui_->checkBox2DOFs,     SIGNAL(toggled(bool)), SLOT(on_2DOF()));
    connect(main_window_ui_->checkBox3DOFs,     SIGNAL(toggled(bool)), SLOT(on_3DOF()));
    connect(main_window_ui_->checkBox4DOFs,     SIGNAL(toggled(bool)), SLOT(on_4DOF()));
    connect(main_window_ui_->checkBox5DOFs,     SIGNAL(toggled(bool)), SLOT(on_5DOF()));
    connect(main_window_ui_->checkBox6DOFs,     SIGNAL(toggled(bool)), SLOT(on_6DOF()));

    //Classic Robots
    connect(main_window_ui_->checkBox1Cl, SIGNAL(toggled(bool)), SLOT(on_5DOF()));
    connect(main_window_ui_->checkBox2Cl, SIGNAL(toggled(bool)), SLOT(on_4DOF()));
    connect(main_window_ui_->checkBox3Cl, SIGNAL(toggled(bool)), SLOT(on_3DOF()));
    connect(main_window_ui_->checkBox4Cl, SIGNAL(toggled(bool)), SLOT(on_3DOF()));

    //Industrial Robots (1)
    connect(main_window_ui_->checkBoxKuka1,     SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxKuka2,     SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxKuka3,     SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxKuka4,     SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxFanuc1,    SIGNAL(toggled(bool)), SLOT(on_6DOF()));

    connect(main_window_ui_->checkBoxFanuc2,    SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxFanuc3,    SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxABB1,      SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxABB2,      SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxABB3,      SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxABB4,      SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxMotoman1,  SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxMotoman2,  SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxKatana,    SIGNAL(toggled(bool)), SLOT(on_6DOF()));
    connect(main_window_ui_->checkBoxUR5,       SIGNAL(toggled(bool)), SLOT(on_6DOF()));

    connect(main_window_ui_->actionOpen,         SIGNAL(triggered()),   SLOT(on_6DOF()));






    //Cinematica Directa
    //Execute FK
    connect(main_window_ui_->PushTrajectory,     SIGNAL(clicked()),SLOT(executeFK()));
    //Execute FK
    connect(main_window_ui_->dial1DOF, SIGNAL(valueChanged(int)), SLOT(updateSpinboxesD()));
    connect(main_window_ui_->dial2DOF, SIGNAL(valueChanged(int)), SLOT(updateSpinboxesD()));
    connect(main_window_ui_->dial3DOF, SIGNAL(valueChanged(int)), SLOT(updateSpinboxesD()));
    connect(main_window_ui_->dial4DOF, SIGNAL(valueChanged(int)), SLOT(updateSpinboxesD()));
    connect(main_window_ui_->dial5DOF, SIGNAL(valueChanged(int)), SLOT(updateSpinboxesD()));
    connect(main_window_ui_->dial6DOF, SIGNAL(valueChanged(int)), SLOT(updateSpinboxesD()));

    connect(main_window_ui_->spinBox1DOF, SIGNAL(valueChanged(double)), SLOT(updateDialer()));
    connect(main_window_ui_->spinBox2DOF, SIGNAL(valueChanged(double)), SLOT(updateDialer()));
    connect(main_window_ui_->spinBox3DOF, SIGNAL(valueChanged(double)), SLOT(updateDialer()));
    connect(main_window_ui_->spinBox4DOF, SIGNAL(valueChanged(double)), SLOT(updateDialer()));
    connect(main_window_ui_->spinBox5DOF, SIGNAL(valueChanged(double)), SLOT(updateDialer()));
    connect(main_window_ui_->spinBox6DOF, SIGNAL(valueChanged(double)), SLOT(updateDialer()));
    //Cinematica Directa

    //Matemática del robot
    //Euler
    connect(main_window_ui_->doubleSpinBox_MYaw,   SIGNAL(valueChanged(double)), SLOT(updateMatEuler()));
    connect(main_window_ui_->doubleSpinBox_MRoll,  SIGNAL(valueChanged(double)), SLOT(updateMatEuler()));
    connect(main_window_ui_->doubleSpinBox_MPitch, SIGNAL(valueChanged(double)), SLOT(updateMatEuler()));
    connect(main_window_ui_->doubleSpinBox_MX,     SIGNAL(valueChanged(double)), SLOT(updateMatEuler()));
    connect(main_window_ui_->doubleSpinBox_MY,     SIGNAL(valueChanged(double)), SLOT(updateMatEuler()));
    connect(main_window_ui_->doubleSpinBox_MZ,     SIGNAL(valueChanged(double)), SLOT(updateMatEuler()));
    //Rot
    //Quaternions
 //   connect(main_window_ui_->doubleSpinBox_qW, SIGNAL(valueChanged(double)), SLOT(updateMatQuat()));
//    connect(main_window_ui_->doubleSpinBox_qX, SIGNAL(valueChanged(double)), SLOT(updateMatQuat()));
//    connect(main_window_ui_->doubleSpinBox_qY, SIGNAL(valueChanged(double)), SLOT(updateMatQuat()));
//    connect(main_window_ui_->doubleSpinBox_qZ, SIGNAL(valueChanged(double)), SLOT(updateMatQuat()));

    // Spherical
    connect(main_window_ui_->doubleSpinBox_rSph,    SIGNAL(valueChanged(double)), SLOT(updateMatSph()));
    connect(main_window_ui_->doubleSpinBox_phiSph,  SIGNAL(valueChanged(double)), SLOT(updateMatSph()));
    connect(main_window_ui_->doubleSpinBox_thSph,   SIGNAL(valueChanged(double)), SLOT(updateMatSph()));

    // Cylindrical
    connect(main_window_ui_->doubleSpinBox_ZCy,     SIGNAL(valueChanged(double)), SLOT(updateMatCyl()));
    connect(main_window_ui_->doubleSpinBox_roCy,    SIGNAL(valueChanged(double)), SLOT(updateMatCyl()));
    connect(main_window_ui_->doubleSpinBox_phiCy,   SIGNAL(valueChanged(double)), SLOT(updateMatCyl()));

    connect(main_window_ui_->spinBox_2,   SIGNAL(valueChanged(int)), SLOT(updateWS(int)));




    //Matemática del robot


    //Industrial Robots (1)
    connect(main_window_ui_->checkBoxKuka1, SIGNAL(clicked()), SLOT(onKUKA1_URDF()));
    connect(main_window_ui_->checkBoxKuka2, SIGNAL(clicked()), SLOT(onKUKA2_URDF()));
    connect(main_window_ui_->checkBoxKuka3, SIGNAL(toggled(bool)), SLOT(onKUKA3_URDF()));
    connect(main_window_ui_->checkBoxKuka4, SIGNAL(toggled(bool)), SLOT(onKUKA4_URDF()));
    connect(main_window_ui_->checkBoxFanuc1, SIGNAL(toggled(bool)), SLOT(onFANUC1_URDF()));

    //Industrial Robots (2)
    connect(main_window_ui_->checkBoxFanuc2, SIGNAL(toggled(bool)), SLOT(onFANUC2_URDF()));
    connect(main_window_ui_->checkBoxFanuc3, SIGNAL(toggled(bool)), SLOT(onFANUC3_URDF()));
    connect(main_window_ui_->checkBoxABB1, SIGNAL(toggled(bool)), SLOT(onABB1_URDF()));
    connect(main_window_ui_->checkBoxABB2, SIGNAL(toggled(bool)), SLOT(onABB2_URDF()));
    connect(main_window_ui_->checkBoxABB3, SIGNAL(toggled(bool)), SLOT(onABB3_URDF()));

    //Industrial Robots (3)
    connect(main_window_ui_->checkBoxABB4, SIGNAL(toggled(bool)), SLOT(onABB4_URDF()));
    connect(main_window_ui_->checkBoxMotoman1, SIGNAL(toggled(bool)), SLOT(onMOTOM1_URDF()));
    connect(main_window_ui_->checkBoxMotoman2, SIGNAL(toggled(bool)), SLOT(onMOTOM2_URDF()));
    connect(main_window_ui_->checkBoxKatana, SIGNAL(toggled(bool)), SLOT(onKATANA_URDF()));
    connect(main_window_ui_->checkBoxUR5, SIGNAL(toggled(bool)), SLOT(onUR5_URDF()));


    //Simple Robots
    connect(main_window_ui_->checkBox2DOFs, SIGNAL(toggled(bool)), SLOT(on2DOFs_URDF()));
    connect(main_window_ui_->checkBox3DOFs, SIGNAL(toggled(bool)), SLOT(on3DOFs_URDF()));
    connect(main_window_ui_->checkBox4DOFs, SIGNAL(toggled(bool)), SLOT(on4DOFs_URDF()));
    connect(main_window_ui_->checkBox5DOFs, SIGNAL(toggled(bool)), SLOT(on5DOFs_URDF()));
    connect(main_window_ui_->checkBox6DOFs, SIGNAL(toggled(bool)), SLOT(on6DOFs_URDF()));

    //Classic Robots
    connect(main_window_ui_->checkBox1Cl, SIGNAL(toggled(bool)), SLOT(on_Puma_URDF()));
    connect(main_window_ui_->checkBox2Cl, SIGNAL(toggled(bool)), SLOT(on_Scara_URDF()));
    connect(main_window_ui_->checkBox3Cl, SIGNAL(toggled(bool)), SLOT(onCartesian_URDF()));
    connect(main_window_ui_->checkBox4Cl, SIGNAL(toggled(bool)), SLOT(onCylindrical_URDF()));

    //Basic Joints
    connect(main_window_ui_->checkBoxRevolute,  SIGNAL(toggled(bool)), SLOT(onRevol_URDF()));
    connect(main_window_ui_->checkBoxPrismatic, SIGNAL(toggled(bool)), SLOT(onPrism_URDF()));
    connect(main_window_ui_->checkBoxRev_Pris,  SIGNAL(toggled(bool)), SLOT(onPris_Rev_URDF()));
    connect(main_window_ui_->checkBoxRev3D,     SIGNAL(toggled(bool)), SLOT(on3DOF_URDF()));     //cube
    connect(main_window_ui_->checkBox6DOFs_3, SIGNAL(toggled(bool)), SLOT(onMat1()));            //airplane


    //Denavith Select parameters to load
    connect(main_window_ui_->spinBox,          SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_valueChanged(int)));
    connect(main_window_ui_->checkBox_DH,     SIGNAL(toggled(bool)), SLOT(on_pushButton_3_toggled()));
    connect(main_window_ui_->checkBox_DH,     SIGNAL(toggled(bool)), SLOT(on_checkBox_DH_toggled()));
    //KeySecuence
//    main_window_ui_->checkBox4DOFI->setShortcut( QKeySequence( QString( "Ctrl+1" )));

//    //ToolTip
//    main_window_ui_->checkBox4DOFI->setToolTip("Robot - Kuka Modelo KR2100, Ctrl+1");
//    main_window_ui_->comboBox->setToolTip("Elige los perfiles de /nvisualización de tu modelo");



   connect(main_window_ui_->checkBox_3,    SIGNAL(toggled(bool)), this, SLOT(on_checkBox_3_toggled(bool)));
//   QObject::connect(main_window_ui_.checkBox_2,    SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_2_toggled(int)));
 //  QObject::connect(main_window_ui_.comboBox,      SIGNAL(activated(int)), this, SLOT(on_comboBox_activated(int)));
   connect(main_window_ui_->comboBox,      SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_currentIndexChanged(int)));
   connect(main_window_ui_->comboBox_2,    SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_2_currentIndexChanged(int)));
   connect(main_window_ui_->comboBox_5,    SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_5_currentIndexChanged(int)));
//    main_window_ui_->tableWidget_3->setRowCount(4);
//    main_window_ui_->tableWidget_3->setColumnCount(4);
////    main_window_ui_->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    QTableWidgetItem *newItem = new QTableWidgetItem();
//    newItem->setText(QString("4"));
////    newItem2->setText("2");
//    main_window_ui_->tableWidget_3->setItem(1,0,newItem);//item(0, 0)->setText("0");
//    main_window_ui_->tableWidget_3->setItem(1,1,newItem);
////    main_window_ui_->tableWidget_3->setItem(2,0,newItem2);
////    main_window_ui_->tableWidget_3->setItem(3,0,newItem2);
////    main_window_ui_->tableWidget_3->item(1, 0)->;
//    main_window_ui_->tableWidget_3->item(2, 0)->setText(QString("0"));


//   main_window_ui_->tableWidget->item(0, 0)->
    startTime = ros::Time::now();
    joint_1_plot = 0.0;
    joint_2_plot = 0.0;
    joint_3_plot = 0.0;
    joint_4_plot = 0.0;
    joint_5_plot = 0.0;
    joint_6_plot = 0.0;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGraph()));
    timer->start(5);
    this->initializeGraph();
    connect(main_window_ui_->graph_canvas, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoved(QMouseEvent*)));
    connect(main_window_ui_->graph_canvas, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(main_window_ui_->pushButton_4, SIGNAL(clicked()), this, SLOT(removeAllGraphs()));
    connect(main_window_ui_->actionOpen, SIGNAL(triggered()), this, SLOT(openTrigger()));

//offWidgets();
std::string filePath = ros::package::getPath("rvizglabre") + "/modelos/uni.urdf";
std::ifstream selected_file(filePath.c_str());
std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
this->updateURDF(file_contents);


//filemap = ros::package::getPath("rvizglabre") + "/maps/fanuc_r1000ia80f.h5";
//filePathmap = QString::fromUtf8(filemap.c_str());

// proc->start("gnome-terminal --geometry=50x10-0-10 -x bash -c \"rosrun map_creator load_reachability_map\" \filePathmap\ ");
//proc->start("bash",QStringList() << "-i" << "-c" << \
//                     "rosrun map_creator load_reachability_map" + filePathmap );

//main_window_ui_.dial1DOF->setMinimum(0);
//main_window_ui_.dial1DOF->setMaximum(10);

//     mRviz->datameasure(dataM);
//     main_window_ui_->label_60->setText(dataM);

//filemap = ros::package::getPath("rvizglabre") + "/maps/fanuc_r1000ia80f.h5";
//filePathmap = QString::fromUtf8(filemap.c_str());


//proc->start("gnome-terminal",QStringList() << "-i" << "-c" << \
//            "rosrun map_creator load_reachability_map" + filePathmap);
//proc->waitForStarted();

        //d_=ros::Duration(0);
        ////itTime_ =d_;
        publisher_thread_ = new boost::thread(boost::bind(&ROSGUI::publishJointStates, this));

            joint_pub            = nh_.advertise<trajectory_msgs::JointTrajectory>("set_joint_trajectory", 10);
            joint_value_pub      = nh_.advertise<std_msgs::Float32MultiArray>("joint_limits", 10);
            joint_sub            = nh_.subscribe("/set_joint_trajectory_delay",10,&ROSGUI::trajectoryCallback,this);
            arduinopub           = nh_.advertise<std_msgs::Int32MultiArray>("arduino_joints", 10);

            //Son pasados los valores via Suscripcion a la función
            //Valores de posicion de MoveIt
            //robot_state_vis_pub_ = nh_.advertise<moveit_msgs::DisplayRobotState>("/my_lab_uni/robot_state",1, true);
            map_reuleaux         = nh_.advertise<std_msgs::String>("/my_lab_uni/map_reuleaux", 10);

          odom_trans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(0,0,0);



}

ROSGUI::~ROSGUI()
{
  delete mRviz;
  if(publisher_thread_ != NULL)
    {
      publisher_thread_->interrupt();
      publisher_thread_->join();

      delete publisher_thread_;
    }

  if(robot_tree_ != NULL)
   robot_tree_.reset();
  if(robot_state_pub_ != NULL)
   robot_state_pub_.reset();
  delete main_window_ui_;
//  delete showModel;
}

//Funcionalidades Ventana principal
//Ventanas EMERGENTES TUTORIALES

//void ROSGUI::show()
//{
//  main_window_.show();
//}

//void ROSGUI::openDialoginfo()
//{
//   /* QMessageBox::StandardButton reply;
//    reply =*/ QMessageBox::information(this, tr("Teoria de Cinematica"), tr("<p>Se encuentra toda la informacion de cinematica directa, " \
//                                                                                "y Cinematica inversa en la pagina principal de este laboratorio virtual." \
//                                                                                "<p>Sigue las instrucciones de la guia del uso y ejemplos  " \
//                                                                                "Para el adecuado uso de esta ventana de practicas.")
//                                    );
////    if (reply == QMessageBox::Ok)
////        informationLabel->setText(tr("OK"));
////    else
////        informationLabel->setText(tr("Escape"));
//}

//void ROSGUI::openCI()
//{
//   thwindow->show();
//}

//void ROSGUI::openDH()
//{
//  fourwindow->show();
//}

void ROSGUI::on_pushButton_clicked()
{

//  ROSGUI::openDialoginfo();
  QMessageBox::information(this, tr("Teoria de Cinematica"), tr("<p>Se encuentra toda la informacion de cinematica directa, " \
                                                                                  "y Cinematica inversa en la pagina principal de este laboratorio virtual." \
                                                                                  "<p>Sigue las instrucciones de la guia del uso y ejemplos  " \
                                                                                  "Para el adecuado uso de esta ventana de practicas."),QMessageBox::Ok);

}


//void ROSGUI::on_pushButton_2_clicked()
//{

// openCI();

//}
//void ROSGUI::on_pushButton_4_clicked()
//{
//    openDH();
//}

//void ROSGUI::on_pushButton_SW_clicked()
//{
//  secwindow->close();
//  thwindow->close();
//  fourwindow->close();

//}

void ROSGUI::on_actionOpen_triggered()
{
//    file_name_ = QFileDialog::getOpenFileName(0, tr("Open URDF File Laboratory"), "/home/yesser/ros_qtc_plugin/src/urdf/", tr("URDF Files (*.urdf *.xml)"));
//                                                  //Nombre de ventana           //path de archivos                           //tipos de archivos
//    if(file_name_.isEmpty())
//        return;
//    printf("file selected: %s\n", qPrintable(file_name_));  //manda la informacion del archivo seleccionado

//    // convert the file to a string
//      std::ifstream selected_file(file_name_.toStdString().c_str());
//      std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
//      this->updateURDF(file_contents);
}

void ROSGUI::on_actionSave_triggered()
{

}

void ROSGUI::on_actionSave_as_triggered()
{

}

void ROSGUI::initializeGraph() {
    //Make legend visible
    //ui.graph_canvas->legend->setVisible(true);
    //ui.graph_canvas->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);

    //Add the graphs
    main_window_ui_->graph_canvas->addGraph();
    main_window_ui_->graph_canvas->graph(0)->setName("Joint_1");
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setStyle(Qt::DotLine);
    blueDotPen.setWidthF(4);
    main_window_ui_->graph_canvas->graph(0)->setPen(blueDotPen);

    main_window_ui_->graph_canvas->addGraph();
    main_window_ui_->graph_canvas->graph(1)->setName("Joint_2");
    QPen redDotPen;
    redDotPen.setColor(Qt::red);
    redDotPen.setStyle(Qt::DotLine);
    redDotPen.setWidthF(4);
    main_window_ui_->graph_canvas->graph(1)->setPen(redDotPen);

    main_window_ui_->graph_canvas->addGraph();
    main_window_ui_->graph_canvas->graph(2)->setName("Joint_3");
    QPen yellowDotPen;
    yellowDotPen.setColor(Qt::yellow);
    yellowDotPen.setStyle(Qt::DotLine);
    yellowDotPen.setWidthF(4);
    main_window_ui_->graph_canvas->graph(2)->setPen(yellowDotPen);

    main_window_ui_->graph_canvas->addGraph();
    main_window_ui_->graph_canvas->graph(3)->setName("Joint_4");
    QPen blackDotPen;
    blackDotPen.setColor(Qt::black);
    blackDotPen.setStyle(Qt::DotLine);
    blackDotPen.setWidthF(4);
    main_window_ui_->graph_canvas->graph(3)->setPen(blackDotPen);

    main_window_ui_->graph_canvas->addGraph();
    main_window_ui_->graph_canvas->graph(4)->setName("Joint_5");
    QPen greenDotPen;
    greenDotPen.setColor(Qt::green);
    greenDotPen.setStyle(Qt::DotLine);
    greenDotPen.setWidthF(4);
    main_window_ui_->graph_canvas->graph(4)->setPen(greenDotPen);

    main_window_ui_->graph_canvas->addGraph();
    main_window_ui_->graph_canvas->graph(5)->setName("Joint_6");
    QPen otherDotPen;
    otherDotPen.setColor(QColor(115,182,209));
    otherDotPen.setStyle(Qt::DotLine);
    otherDotPen.setWidthF(4);
    main_window_ui_->graph_canvas->graph(5)->setPen(otherDotPen);

    // give the axes some labels:
    main_window_ui_->graph_canvas->xAxis->setLabel("Time (s)");
    main_window_ui_->graph_canvas->yAxis->setLabel("Joint move (°)");
    main_window_ui_->graph_canvas->legend->setVisible(true);


    //For user interaction
    // main_window_ui_->graph_canvas->setInteraction(QCP::iRangeDrag, true);
    // main_window_ui_->graph_canvas->setInteraction(QCP::iRangeZoom, true);
    main_window_ui_->graph_canvas->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);

    connect(main_window_ui_->graph_canvas, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoved(QMouseEvent*)));

    x_org = 0;

    //Plot the graph
    main_window_ui_->graph_canvas->replot();
}

void ROSGUI::updateGraph() {
    double x_val = (ros::Time::now() - startTime).toSec();

    // double x_org1 = x_org;
    // if (x_val - x_org1 > 20) {
    //     main_window_ui_->graph_canvas->graph(0)->removeDataBefore(x_org1 + 1);
    //     main_window_ui_->graph_canvas->graph(1)->removeDataBefore(x_org1 + 1);
    //     x_org++;
    // }

    main_window_ui_->graph_canvas->graph(0)->addData(x_val, joint_1_plot);//Set Point
    main_window_ui_->graph_canvas->graph(1)->addData(x_val, joint_2_plot);//Output
    main_window_ui_->graph_canvas->graph(2)->addData(x_val, joint_3_plot);
    main_window_ui_->graph_canvas->graph(3)->addData(x_val, joint_4_plot);
    main_window_ui_->graph_canvas->graph(4)->addData(x_val, joint_5_plot);
    main_window_ui_->graph_canvas->graph(5)->addData(x_val, joint_6_plot);
    main_window_ui_->graph_canvas->rescaleAxes();
    main_window_ui_->graph_canvas->replot();
}

void ROSGUI::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
  if (main_window_ui_->graph_canvas->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    main_window_ui_->graph_canvas->axisRect()->setRangeZoom(main_window_ui_->graph_canvas->xAxis->orientation());
  else if (main_window_ui_->graph_canvas->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    main_window_ui_->graph_canvas->axisRect()->setRangeZoom(main_window_ui_->graph_canvas->yAxis->orientation());
  else
    main_window_ui_->graph_canvas->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void ROSGUI::removeAllGraphs()
{
   // main_window_ui_->graph_canvas->clearGraphs();
   main_window_ui_->graph_canvas->graph(0)->data()->clear();
   main_window_ui_->graph_canvas->graph(1)->data()->clear();
   main_window_ui_->graph_canvas->graph(2)->data()->clear();
   main_window_ui_->graph_canvas->graph(3)->data()->clear();
   main_window_ui_->graph_canvas->graph(4)->data()->clear();
   main_window_ui_->graph_canvas->graph(5)->data()->clear();

   main_window_ui_->graph_canvas->replot();
}

void ROSGUI::on_comboBox_5_currentIndexChanged(int index=0)
{
  switch (index){

    case 0:
 {
    main_window_ui_->graph_canvas->graph(0)->setVisible(true);
    main_window_ui_->graph_canvas->graph(1)->setVisible(true);
    main_window_ui_->graph_canvas->graph(2)->setVisible(true);
    main_window_ui_->graph_canvas->graph(3)->setVisible(true);
    main_window_ui_->graph_canvas->graph(4)->setVisible(true);
    main_window_ui_->graph_canvas->graph(5)->setVisible(true);
   break;
 }

   case 1:
  {
    main_window_ui_->graph_canvas->graph(0)->setVisible(true);
    main_window_ui_->graph_canvas->graph(1)->setVisible(false);
    main_window_ui_->graph_canvas->graph(2)->setVisible(false);
    main_window_ui_->graph_canvas->graph(3)->setVisible(false);
    main_window_ui_->graph_canvas->graph(4)->setVisible(false);
    main_window_ui_->graph_canvas->graph(5)->setVisible(false);

   break;
  }
   case 2:
 {
    main_window_ui_->graph_canvas->graph(0)->setVisible(false);
    main_window_ui_->graph_canvas->graph(1)->setVisible(true);
    main_window_ui_->graph_canvas->graph(2)->setVisible(false);
    main_window_ui_->graph_canvas->graph(3)->setVisible(false);
    main_window_ui_->graph_canvas->graph(4)->setVisible(false);
    main_window_ui_->graph_canvas->graph(5)->setVisible(false);
   break;
 }

    case 3:
 {
    main_window_ui_->graph_canvas->graph(0)->setVisible(false);
    main_window_ui_->graph_canvas->graph(1)->setVisible(false);
    main_window_ui_->graph_canvas->graph(2)->setVisible(true);
    main_window_ui_->graph_canvas->graph(3)->setVisible(false);
    main_window_ui_->graph_canvas->graph(4)->setVisible(false);
    main_window_ui_->graph_canvas->graph(5)->setVisible(false);
   break;
 }

    case 4:
 {
    main_window_ui_->graph_canvas->graph(0)->setVisible(false);
    main_window_ui_->graph_canvas->graph(1)->setVisible(false);
    main_window_ui_->graph_canvas->graph(2)->setVisible(false);
    main_window_ui_->graph_canvas->graph(3)->setVisible(true);
    main_window_ui_->graph_canvas->graph(4)->setVisible(false);
    main_window_ui_->graph_canvas->graph(5)->setVisible(false);
   break;
 }
    case 5:
 {
    main_window_ui_->graph_canvas->graph(0)->setVisible(false);
    main_window_ui_->graph_canvas->graph(1)->setVisible(false);
    main_window_ui_->graph_canvas->graph(2)->setVisible(false);
    main_window_ui_->graph_canvas->graph(3)->setVisible(false);
    main_window_ui_->graph_canvas->graph(4)->setVisible(true);
    main_window_ui_->graph_canvas->graph(5)->setVisible(false);
   break;
 }
    case 6:
 {
    main_window_ui_->graph_canvas->graph(0)->setVisible(false);
    main_window_ui_->graph_canvas->graph(1)->setVisible(false);
    main_window_ui_->graph_canvas->graph(2)->setVisible(false);
    main_window_ui_->graph_canvas->graph(3)->setVisible(false);
    main_window_ui_->graph_canvas->graph(4)->setVisible(false);
    main_window_ui_->graph_canvas->graph(5)->setVisible(true);
   break;
 }

  }
}

//void ROSGUI::on_actionExit_triggered()
//{
// main_window_.close();
//}

void ROSGUI::openTrigger() {
    file_name_ = QFileDialog::getOpenFileName(0, tr("Open URDF File"), ".", tr("XML Files (*.urdf)"));

    // this will be true if the user cancels
    if(file_name_.isEmpty())
        return;

    // debugging print for now
    printf("file selected: %s\n", qPrintable(file_name_));

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");


    // convert the file to a string
    std::ifstream selected_file(file_name_.toStdString().c_str());
    std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());

    // fill the text editor with this string
    this->updateURDF(file_contents);
    updatetoURDF();
    main_window_ui_->statusBar->showMessage(tr("Modelo proveido por el usuario"));

}



void ROSGUI::onMat1()
{
//  delete robot_state_pub_;
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  std::string filePath = ros::package::getPath("rvizglabre") + "/modelos/Math/airplane.urdf";
  std::ifstream selected_file(filePath.c_str());
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Cubo, para analisis de matemática del Robot"));
}

void ROSGUI::on3DOF_URDF()
{

  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  std::string filePath = ros::package::getPath("rvizglabre") + "/modelos/Math/Cubo.urdf";
  std::ifstream selected_file(filePath.c_str());
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Cubo, para analisis de matemática del Robot"));

}
void ROSGUI::onRevol_URDF(){

}


void ROSGUI::onPris_Rev_URDF(){

}

void ROSGUI::onPrism_URDF(){
  ToG =10; //Meter ->Centimeter
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("Cm");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/prismatic.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Prismatico DOF"));

}

void ROSGUI::on2DOFs_URDF()
{
//  delete robot_state_pub_;
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");

//  QTemporaryDir temporaryDir2;

//  QFile::copy(":/robots/URDF/modelos/two_link_yz.urdf", temporaryDir2.path() + "/two_link_planarxy.urdf");



  //    file_name_ = "/home/yesser/ros_qtc_plugin/src/rvizglabre/modelos/irb120_3_58.urdf";
  std::string filePath = ros::package::getPath("rvizglabre") + "/modelos/two_link_yz.urdf";
  std::ifstream selected_file(filePath.c_str());
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo 2 DOF"));

    //   <QtGui>
  // proc.start("gnome-terminal --geometry=50x10-0-10 -x bash -c \"roscore\" ");
  //QProcess *proc = new QProcess();
//  proc->start("gnome-terminal --geometry=50x10-0-10 -x bash -c \"rosrun rvizglabre talker\"");

}
void ROSGUI::on3DOFs_URDF()
{
//  delete robot_state_pub_;
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  std::string filePath = ros::package::getPath("rvizglabre") + "/modelos/3dof.urdf";
  std::ifstream selected_file(filePath.c_str());
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo 3 DOF"));


}
void ROSGUI::on4DOFs_URDF(){
  ToG    = 57.295779513;
  main_window_ui_->statusBar->showMessage(tr("Modelo 4 UNI"));
 

}
void ROSGUI::on5DOFs_URDF(){
  ToG    = 57.295779513;
  main_window_ui_->statusBar->showMessage(tr("Modelo 5 DOF UNI"));
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  std::string filePath = ros::package::getPath("rvizglabre") + "/modelos/armUNI.urdf";
  std::ifstream selected_file(filePath.c_str());
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
}

void ROSGUI::on6DOFs_URDF(){
  ToG    = 57.295779513;
  main_window_ui_->statusBar->showMessage(tr("Modelo 6 DOF"));

}



void ROSGUI::on_Puma_URDF(){
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/puma560.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
//  map.data =  ros::package::getPath("rvizglabre") + "/maps/scara.h5";
//  ROS_INFO("%s", map.data.c_str());
//  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Puma 560 DOF"));

}

void ROSGUI::on_Scara_URDF(){
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/scara.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/scara.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Scara"));

}

void ROSGUI::onCartesian_URDF(){
  ToG =10; //Meter ->Centimeter
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("Cm");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");

  filePath= ros::package::getPath("rvizglabre") + "/modelos/cartesian.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Cartesiano"));
}

void ROSGUI::onCylindrical_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");  
  filePath= ros::package::getPath("rvizglabre") + "/modelos/cylindrical.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Cilindrico"));

}

void ROSGUI::onKUKA1_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/kr210l150.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/kr210l150.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial KUKA KR210L150"));

}

void ROSGUI::onKUKA2_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/kr16_2.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial KUKA KR16-2"));


}

void ROSGUI::onKUKA3_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/kr120r2500.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial KUKA KR120R2500"));

}

void ROSGUI::onKUKA4_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/lbr_iiwa_14.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/lbr_iiwa_14.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial KUKA LBR-IIWA-14-R820"));


}

void ROSGUI::onFANUC1_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/fanuc_lrmate200ic.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/fanuc_lrmate200ic.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial FANUC LRMATE200Ic"));


}

void ROSGUI::onFANUC2_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/fanuc_m10ia.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/fanuc_m10ia.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial FANUC M10IA"));


}

void ROSGUI::onFANUC3_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/r1000ia80f.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/fanuc_r1000ia80f.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial FANUC R1000IA80F"));

   //  proc->start("bash",QStringList() << "-i" << "-c" << \
   //                       "rosrun map_creator load_reachability_map" + filePathmap );
}

void ROSGUI::onABB1_URDF()
{
  ToG    = 57.295779513;

//  // SIN COPIA DE ARCHIVO

//    file_name_ = "/home/udp/ros_qtc_plugin/src/rvizglabre/modelos/irb120_3_58.urdf";


//    std::ifstream selected_file(file_name_.toStdString().c_str());


//  // SIN COPIA DE ARCHIVO

// CON COPIA DE ARCHIVO
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
   filePath= ros::package::getPath("rvizglabre") + "/modelos/irb120_3_58.urdf";
   std::ifstream selected_file(filePath);
// CON COPIA DE ARCHIVO

  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());

//  msg->name = {"joint_1","joint_2","joint_3","joint_4","joint_5","joint_6"};
//  msg->position = {0.0, 0.05235092341899872, 0.0, 1.518426775932312, 0.0, 0.9599822759628296, 0.0};
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/irb120_3_58.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial ABB IRB120"));



}

void ROSGUI::onABB2_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/irb2600.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/irb2600.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial ABB IRB2600"));

}

void ROSGUI::onABB3_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/irb6640.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/irb6640.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial ABB IRB6640"));

}

void ROSGUI::onABB4_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");

  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/irb5400.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial ABB IRB5400"));

}


void ROSGUI::onMOTOM1_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","link_6");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/mh5.urdf";

  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/motoman_mh5.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
//  proc->start("gnome-terminal --geometry=50x10-0-10 -x bash -c \"rosrun map_creator load_reachability_map fanuc_r1000ia80f.h5\"");
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial Yaskawa MOTOMAN-MH5"));


}

void ROSGUI::onMOTOM2_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/sia20d.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/sia20d.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial Yaskawa MOTOMAN-SIA20D"));

}

void ROSGUI::onKATANA_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/katana_robot.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/katana.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial Neuronics KATANA-450-6M90A"));

}

void ROSGUI::onUR5_URDF()
{
  ToG    = 57.295779513;
  main_window_ui_->comboBox->setCurrentIndex(0); // Shwo All Options Robot Arrows TF
  QFont f( "Sans Serif", 9, QFont::Normal);
  main_window_ui_->label_15->setFont(f);
  main_window_ui_->label_15->setText("°");
  resetvalue();
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","base_link");
  nh_.setParam("tip_link","tool0");
  filePath= ros::package::getPath("rvizglabre") + "/modelos/ur5_robot.urdf";
  std::ifstream selected_file(filePath);
  std::string file_contents((std::istreambuf_iterator<char>(selected_file)), std::istreambuf_iterator<char>());
  this->updateURDF(file_contents);
  updatetoURDF();
  map.data =  ros::package::getPath("rvizglabre") + "/maps/ur5.h5";
  ROS_INFO("%s", map.data.c_str());
  map_reuleaux.publish(map);
  main_window_ui_->statusBar->showMessage(tr("Modelo Industrial Universal Robots UR5"));

}




void ROSGUI::updateMatEuler(){

    KDL::Rotation rot;
    std::vector<double> angleRot(6);
    double phiSph,rSph,thSph;
    double phiC,roC;
    double X,Y,Z;

    main_window_ui_->doubleSpinBox_rSph->      blockSignals(true);
    main_window_ui_->doubleSpinBox_phiSph->    blockSignals(true);
    main_window_ui_->doubleSpinBox_thSph->     blockSignals(true);

    main_window_ui_->doubleSpinBox_ZCy->       blockSignals(true);
    main_window_ui_->doubleSpinBox_roCy->      blockSignals(true);
    main_window_ui_->doubleSpinBox_phiCy->     blockSignals(true);

    angleRot[0] = main_window_ui_->doubleSpinBox_MRoll-> value()/ToG;
    angleRot[1] = main_window_ui_->doubleSpinBox_MPitch->value()/ToG;
    angleRot[2] = main_window_ui_->doubleSpinBox_MYaw->  value()/ToG;

    angleRot[3] = main_window_ui_->doubleSpinBox_MX->value();
    angleRot[4] = main_window_ui_->doubleSpinBox_MY->value();
    angleRot[5] = main_window_ui_->doubleSpinBox_MZ->value();

    // tf::Quaternion quat = tf::createQuaternionFromRPY( angleRot[0],  angleRot[1],  angleRot[2]);

    // main_window_ui_->qW->setText(QString::number(quat.getW()));
    // main_window_ui_->qX->setText(QString::number(quat.getX()));
    // main_window_ui_->qY->setText(QString::number(quat.getY()));
    // main_window_ui_->qZ->setText(QString::number(quat.getZ()));

    //Calculos Coordenadas Esfericas

    phiSph = atan (angleRot[4]/angleRot[3]);

    thSph = atan ( 
                  (sqrt 
                        ( pow (angleRot[3], 2.0) + pow (angleRot[4], 2.0)   ) 
                   ) / angleRot[5] 
               );

    rSph= sqrt 
             ( pow (angleRot[3], 2.0) + pow (angleRot[4], 2.0) + pow (angleRot[5], 2.0) ) ;


    //Calculos Coordenadas Cilindricas

    roC = sqrt 
             ( pow (angleRot[3], 2.0) + pow (angleRot[4], 2.0) ); 

    phiC= phiSph;



    main_window_ui_->doubleSpinBox_rSph   ->setValue(rSph);
    main_window_ui_->doubleSpinBox_phiSph ->setValue(phiSph*ToG);
    main_window_ui_->doubleSpinBox_thSph  ->setValue(thSph*ToG);

    main_window_ui_->doubleSpinBox_ZCy    ->setValue(angleRot[5]);
    main_window_ui_->doubleSpinBox_roCy   ->setValue(roC);
    main_window_ui_->doubleSpinBox_phiCy  ->setValue(phiC*ToG);

    // tf::quaternionTFToKDL(quat,rot);


    // main_window_ui_->label_mat1x1->setText(QString::number(rot.UnitX().x()));
    // main_window_ui_->label_mat1x2->setText(QString::number(rot.UnitX().y()));
    // main_window_ui_->label_mat1x3->setText(QString::number(rot.UnitX().z()));
    // main_window_ui_->label_mat1x4->setText(QString::number(angleRot[3]));

    // main_window_ui_->label_mat2x1->setText(QString::number(rot.UnitY().x()));
    // main_window_ui_->label_mat2x2->setText(QString::number(rot.UnitY().y()));
    // main_window_ui_->label_mat2x3->setText(QString::number(rot.UnitY().z()));
    // main_window_ui_->label_mat2x4->setText(QString::number(angleRot[4]));

    // main_window_ui_->label_mat3x1->setText(QString::number(rot.UnitZ().x()));
    // main_window_ui_->label_mat3x2->setText(QString::number(rot.UnitZ().y()));
    // main_window_ui_->label_mat3x3->setText(QString::number(rot.UnitZ().z()));
    // main_window_ui_->label_mat3x4->setText(QString::number(angleRot[5]));

    // main_window_ui_->label_mat4x1->setText(QString("0"));
    // main_window_ui_->label_mat4x2->setText(QString("0"));
    // main_window_ui_->label_mat4x3->setText(QString("0"));
    // main_window_ui_->label_mat4x4->setText(QString("1"));


    // Z = rSph * cos(thSph);
    // Y = rSph * sin(thSph) * sin(phiSph);
    // X = rSph * sin(thSph) * cos(phiSph);

    // main_window_ui_->doubleSpinBox_MX->setValue(X);
    // main_window_ui_->doubleSpinBox_MY->setValue(Y);
    // main_window_ui_->doubleSpinBox_MZ->setValue(Z);



 
    // main_window_ui_->doubleSpinBox_qX->setValue(quat.getX());
    // main_window_ui_->doubleSpinBox_qY->setValue(quat.getY());
    // main_window_ui_->doubleSpinBox_qZ->setValue(quat.getZ());

    odom_trans.transform.translation.x =  angleRot[3];
    odom_trans.transform.translation.y =  angleRot[4];
    odom_trans.transform.translation.z =  angleRot[5];

    odom_trans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(angleRot[0],angleRot[1],angleRot[2] );

//    std::cout << m <<std::endl;



    main_window_ui_->doubleSpinBox_rSph->      blockSignals(false);
    main_window_ui_->doubleSpinBox_phiSph->    blockSignals(false);
    main_window_ui_->doubleSpinBox_thSph->     blockSignals(false);

    main_window_ui_->doubleSpinBox_ZCy->       blockSignals(false);
    main_window_ui_->doubleSpinBox_roCy->      blockSignals(false);
    main_window_ui_->doubleSpinBox_phiCy->     blockSignals(false);

}


void ROSGUI::updateMatQuat(){
//      std::vector<double> angleRot(1);
//      double roll, pitch, yaw;
//      angleRot[0] = main_window_ui_->doubleSpinBox_qW->value();


// //     angleRot[1] = main_window_ui_->doubleSpinBox_qX->value();
// //     angleRot[2] = main_window_ui_->doubleSpinBox_qY->value();
// //     angleRot[3] = main_window_ui_->doubleSpinBox_qZ->value();

//      tf::Quaternion q;
//      //(
// //     angleRot[0],
// //     angleRot[1],
// //     angleRot[2],
// //     angleRot[3] );
//      q.setW(angleRot[0]);
//      q.normalize();
//      q.normalized();
     
//      main_window_ui_->doubleSpinBox_qX->setValue(q.getX());
//      main_window_ui_->doubleSpinBox_qY->setValue(q.getY());
//      main_window_ui_->doubleSpinBox_qZ->setValue(q.getZ());

//       //odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(89);

//      tf::Matrix3x3 m(q);
//      m.getRPY(roll, pitch, yaw);
//      main_window_ui_->spinBox_MRoll->setValue(roll*ToG);
//      main_window_ui_->spinBox_MPitch->setValue(pitch*ToG);
//      main_window_ui_->spinBox_MYaw->setValue(yaw*ToG);


//      odom_trans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(roll,pitch,yaw);

//      std::cout << angleRot[0] <<std::endl;


}


void ROSGUI::updateMatSph()  {

    main_window_ui_->doubleSpinBox_MX->    blockSignals(true);
    main_window_ui_->doubleSpinBox_MY->    blockSignals(true);
    main_window_ui_->doubleSpinBox_MZ->    blockSignals(true);

    main_window_ui_->doubleSpinBox_ZCy->       blockSignals(true);
    main_window_ui_->doubleSpinBox_roCy->      blockSignals(true);
    main_window_ui_->doubleSpinBox_phiCy->     blockSignals(true);


    double phiSph,rSph,thSph;
    double phiC,roC;
    double X,Y,Z;

    rSph   = main_window_ui_->doubleSpinBox_rSph    -> value();
    phiSph = main_window_ui_->doubleSpinBox_phiSph  -> value()/ToG;
    thSph  = main_window_ui_->doubleSpinBox_thSph   -> value()/ToG;

    //Cartesian
    Z = rSph * cos(thSph);
    Y = rSph * sin(thSph) * sin(phiSph);
    X = rSph * sin(thSph) * cos(phiSph);

    phiC = phiSph;
    roC  = rSph * sin(thSph);

    main_window_ui_->doubleSpinBox_MX->setValue(X);
    main_window_ui_->doubleSpinBox_MY->setValue(Y);
    main_window_ui_->doubleSpinBox_MZ->setValue(Z);

    main_window_ui_->doubleSpinBox_phiCy->setValue(phiC*ToG);
    main_window_ui_->doubleSpinBox_roCy ->setValue(roC);
    main_window_ui_->doubleSpinBox_ZCy  ->setValue(Z);


    main_window_ui_->label_mat1x4->setText(QString::number(X));
    main_window_ui_->label_mat2x4->setText(QString::number(Y));
    main_window_ui_->label_mat3x4->setText(QString::number(Z));

    odom_trans.transform.translation.x =  X;
    odom_trans.transform.translation.y =  Y;
    odom_trans.transform.translation.z =  Z;

    main_window_ui_->doubleSpinBox_MX->    blockSignals(false);
    main_window_ui_->doubleSpinBox_MY->    blockSignals(false);
    main_window_ui_->doubleSpinBox_MZ->    blockSignals(false);

    main_window_ui_->doubleSpinBox_ZCy->       blockSignals(false);
    main_window_ui_->doubleSpinBox_roCy->      blockSignals(false);
    main_window_ui_->doubleSpinBox_phiCy->     blockSignals(false);
 
}

//cc280120190210

void ROSGUI::updateMatCyl(){

    main_window_ui_->doubleSpinBox_MX->    blockSignals(true);
    main_window_ui_->doubleSpinBox_MY->    blockSignals(true);
    main_window_ui_->doubleSpinBox_MZ->    blockSignals(true);

    main_window_ui_->doubleSpinBox_rSph->      blockSignals(true);
    main_window_ui_->doubleSpinBox_phiSph->    blockSignals(true);
    main_window_ui_->doubleSpinBox_thSph->     blockSignals(true);

    double phiSph,rSph,thSph;
    double phiC,roC;
    double X,Y,Z;


    Z     =  main_window_ui_->doubleSpinBox_ZCy   -> value();
    roC   =  main_window_ui_->doubleSpinBox_roCy  -> value();
    phiC  =  main_window_ui_->doubleSpinBox_phiCy -> value()/ToG;

    Y = roC  * sin(phiC);
    X = roC  * cos(phiC);

    rSph   = sqrt 
                ( pow (roC, 2.0) + pow (Z, 2.0) ); 

    thSph  = atan (roC/Z);

    phiSph = phiC;
    
    main_window_ui_->doubleSpinBox_MX->setValue(X);
    main_window_ui_->doubleSpinBox_MY->setValue(Y);
    main_window_ui_->doubleSpinBox_MZ->setValue(Z);

    main_window_ui_->label_mat1x4->setText(QString::number(X));
    main_window_ui_->label_mat2x4->setText(QString::number(Y));
    main_window_ui_->label_mat3x4->setText(QString::number(Z));

    main_window_ui_->doubleSpinBox_rSph   ->setValue(rSph);
    main_window_ui_->doubleSpinBox_phiSph ->setValue(phiSph*ToG);
    main_window_ui_->doubleSpinBox_thSph  ->setValue(thSph*ToG);

    odom_trans.transform.translation.x =  X;
    odom_trans.transform.translation.y =  Y;
    odom_trans.transform.translation.z =  Z;

    // main_window_ui_->doubleSpinBox_ZCy,    
    // main_window_ui_->doubleSpinBox_roCy,    
    // main_window_ui_->doubleSpinBox_phiCy, 




    main_window_ui_->doubleSpinBox_MX->    blockSignals(false);
    main_window_ui_->doubleSpinBox_MY->    blockSignals(false);
    main_window_ui_->doubleSpinBox_MZ->    blockSignals(false);

    main_window_ui_->doubleSpinBox_rSph->      blockSignals(false);
    main_window_ui_->doubleSpinBox_phiSph->    blockSignals(false);
    main_window_ui_->doubleSpinBox_thSph->     blockSignals(false);
}


void ROSGUI::updateWS(int dataWS){
 //int dataWS =0;
 //dataWS = main_window_ui_->spinBox_2->value();
 mRviz->updateWs(dataWS);

}





void ROSGUI::updateURDF(const std::string& urdf)
{

//  std::string param_nameR = "robot_description";
//  std::string full_param_name;
//  std::string xml_string;
//  urdf::Model urdf_model;
//  nh_.searchParam(param_nameR, full_param_name);
//    if (nh_.hasParam(full_param_name)){nh_.getParam(full_param_name.c_str(), xml_string);}
//    if (!urdf_model.initString(xml_string))    //Si se abre desde una direccion de ruta de archivo .initFile
//    {
//        ROS_ERROR("Failed to parse urdf file in model param");
//        nh_.shutdown();
//        //return false;
//    }

//  XmlRpc::XmlRpcValue robot_description(xml_string);
//  nh_.setParam("my_lab_uni/robot_description", robot_description);   // nh_. Node handle publicador de los parametros urdf del robot
////  boost::mutex::scoped_lock state_pub_lock(state_pub_mutex_);
///
  XmlRpc::XmlRpcValue robot_description(urdf);
    nh_.setParam("my_lab_uni/robot_description", robot_description);   // nh_. Node handle publicador de los parametros urdf del robot
  //  boost::mutex::scoped_lock state_pub_lock(state_pub_mutex_);

//    if(robot_tree_ != NULL)
//      delete robot_tree_;
//    if(robot_state_pub_ != NULL)
//      delete robot_state_pub_;

    robot_tree_.reset(new KDL::Tree());
    if(!kdl_parser::treeFromString(urdf, *robot_tree_))
    {
      ROS_ERROR("Failed to construct KDL tree");
      return;
    }




//  if(robot_tree_ != NULL)
//    delete robot_tree_;
//  if(robot_state_pub_ != NULL)
//    delete robot_state_pub_;

//  robot_tree_ = new KDL::Tree();
//  if(!kdl_parser::treeFromString(xml_string, *robot_tree_))
//  {
//    ROS_ERROR("Failed to construct KDL tree");
//    return;
//  }

  // create a robot state publisher from the tree
  robot_state_pub_.reset( new robot_state_publisher::RobotStatePublisher(*robot_tree_));

  // now create a map with joint name and positions
  joint_positions_.clear();
  const std::map<std::string, KDL::TreeElement>& segments = robot_tree_->getSegments();
  for(std::map<std::string, KDL::TreeElement>::const_iterator it=segments.begin();
    it != segments.end(); it++)
  {
    joint_positions_[it->second.segment.getJoint().getName()] = 0.0;
 std::cout << it->second.segment.getJoint().getName() <<std::endl;
  }

  // refresh the preview
  mRviz->refresh("my_lab_world");///"+ robot_tree_->getRootSegment()->first);

 // mRviz->subscribeTopics("joint_states");

}


void ROSGUI::updatetreeforDH(KDL::Tree modelU){
//  if(robot_state_pub_ != NULL)
//    delete robot_state_pub_;

//robot_tree_.reset(new KDL::Tree());
///robot_tree_=modelU;
  robot_state_pub_.reset( new robot_state_publisher::RobotStatePublisher(modelU));


 // robot_state_pub_.reset();

  //robot_state_pub2_ = new robot_state_publisher::RobotStatePublisher(modelU);
  // new robot_state_publisher::RobotStatePublisher(modelU));
  joint_positions_.clear();
  const std::map<std::string, KDL::TreeElement>& segments = modelU.getSegments();
  for(std::map<std::string, KDL::TreeElement>::const_iterator it=segments.begin();
    it != segments.end(); it++)
  {
    joint_positions_[it->second.segment.getJoint().getName()] = 0.0;
    std::cout << "Joints"<< it->second.segment.getJoint().getName() <<std::endl;
  }

  // refresh the preview
  mRviz->refreshDH("my_lab_world/" + modelU.getRootSegment()->first);
  main_window_ui_->statusBar->showMessage(tr("Modelo Usuario DH"));

}

void ROSGUI::publishJointStates(/*const trajectory_msgs::JointTrajectory &trajectory*/)
{



//  trajectory_msgs::JointTrajectoryPoint* point =  trajectory.points;
////  std::vector<trajectory_msgs::JointTrajectoryPoint> point(6);
////  point =;
////  point[0].positions.resize(1);
//  joint_positions_["joint_1"]= point[0].positions[0];
//  joint_positions_["joint_2"]= main_window_ui_->spinBox2DOF->value()/ToG;
//  joint_positions_["joint_3"]= main_window_ui_->spinBox3DOF->value()/ToG;
//  joint_positions_["joint_4"]= main_window_ui_->spinBox4DOF->value()/ToG;
//  joint_positions_["joint_5"]= main_window_ui_->spinBox5DOF->value()/ToG;
//  joint_positions_["joint_6"]= main_window_ui_->spinBox6DOF->value()/ToG;



  ros::Rate loop_rate(5);

  while(true)
  {
    { // lock the state publisher objects and run
      boost::mutex::scoped_lock state_pub_lock(state_pub_mutex_);
          odom_trans.header.frame_id = "my_lab_world";
          odom_trans.child_frame_id = "my_lab_world/base_link";
          odom_trans.header.stamp = ros::Time::now();

          //odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(89);
          broadcaster.sendTransform(odom_trans);



      if(robot_state_pub_ != NULL)
      {
         robot_state_pub_->publishTransforms(joint_positions_, ros::Time::now()/*+d_*/, "my_lab_world");
         robot_state_pub_->publishFixedTransforms("my_lab_world");

      }


//      if(robot_state_pubDH_ != NULL)
//      {
//         robot_state_pubDH_->publishTransforms(joint_positionsDH_, ros::Time::now(), "my_lab_uni");
//         robot_state_pubDH_->publishFixedTransforms("my_lab_uni");
//      }
         //         auto it2 = joint_positions_.rbegin();
//         std::cout << it2->first << " : " << it2->second << std::endl;
//         it2++;
//         std::cout << it2->first << " : " << it2->second << std::endl;
//         it2++;
//         std::cout << it2->first << " : " << it2->second << std::endl;
//         it2++;
//         std::cout << it2->first << " : " << it2->second << std::endl;
//         it2++;
//         std::cout << it2->first << " : " << it2->second << std::endl;
//         it2++;
//         std::cout << it2->first << " : " << it2->second << std::endl;
//         it2++;


//         for(int i,)
         //ROS_INFO_STREAM(joint_positions_.size());
        //  ROS_INFO(joint_positions_);
        // sensor_msgs::JointState::ConstPtr& msg;
        // msg->fl;
         //msg->name = {"joint_1","joint_2","joint_3","joint_4","joint_5","joint_6"};

         //msg->position = {0.0, 0.05235092341899872, 0.0, 1.518426775932312, 0.0, 0.9599822759628296, 0.0};
       //  ROS_INFO("Joint1=%f Joint2=%f", msg, msg);
        // ROS_INFO("Published joint state info");

        // ROS_INFO(" Lower %f \n Upper %f",ROSGUI::lower_limits[1], ROSGUI::upper_limits[1]);

    }
    try {
      boost::this_thread::interruption_point();
    } catch(const boost::thread_interrupted& o) {
      break; // quit the thread's loop
    }


    loop_rate.sleep();
  }
//return true;
}

void ROSGUI::updateSpinboxes()
{
        main_window_ui_->yBox->    blockSignals(true);
        main_window_ui_->zBox->    blockSignals(true);
        main_window_ui_->rollBox-> blockSignals(true);
        main_window_ui_->pitchBox->blockSignals(true);
        main_window_ui_->yawBox->  blockSignals(true);

        main_window_ui_->xBox->    setValue((main_window_ui_->xSlider->value() * FACTOR));
        main_window_ui_->yBox->    setValue((main_window_ui_->ySlider->value() * FACTOR));
        main_window_ui_->zBox->    setValue((main_window_ui_->zSlider->value() * FACTOR));
        main_window_ui_->rollBox-> setValue((main_window_ui_->rollSlider->value() * FACTOR));
        main_window_ui_->pitchBox->setValue((main_window_ui_->pitchSlider->value() * FACTOR));
        main_window_ui_->yawBox->  setValue((main_window_ui_->yawSlider->value() * FACTOR));


        main_window_ui_->xBox->    blockSignals(false);
        main_window_ui_->yBox->    blockSignals(false);
        main_window_ui_->zBox->    blockSignals(false);
        main_window_ui_->rollBox-> blockSignals(false);
        main_window_ui_->pitchBox->blockSignals(false);
        main_window_ui_->yawBox->  blockSignals(false);

}
void ROSGUI::updateSlider()
{
        main_window_ui_->xSlider->    blockSignals(true);
        main_window_ui_->ySlider->    blockSignals(true);
        main_window_ui_->zSlider->    blockSignals(true);
        main_window_ui_->rollSlider-> blockSignals(true);
        main_window_ui_->pitchSlider->blockSignals(true);
        main_window_ui_->yawSlider->  blockSignals(true);

        main_window_ui_->xSlider->    setValue((main_window_ui_->xBox->value() / FACTOR));
        main_window_ui_->ySlider->    setValue((main_window_ui_->yBox->value() / FACTOR));
        main_window_ui_->zSlider->    setValue((main_window_ui_->zBox->value() / FACTOR));
        main_window_ui_->rollSlider-> setValue((main_window_ui_->rollBox->value() / FACTOR));
        main_window_ui_->pitchSlider->setValue((main_window_ui_->pitchBox->value() / FACTOR));
        main_window_ui_->yawSlider->  setValue((main_window_ui_->yawBox->value() / FACTOR));



        main_window_ui_->xSlider->    blockSignals(false);
        main_window_ui_->ySlider->    blockSignals(false);
        main_window_ui_->zSlider->    blockSignals(false);
        main_window_ui_->rollSlider-> blockSignals(false);
        main_window_ui_->pitchSlider->blockSignals(false);
        main_window_ui_->yawSlider->  blockSignals(false);

}

void ROSGUI::updateDialer()
{


  main_window_ui_->dial1DOF->    blockSignals(true);
  main_window_ui_->dial2DOF->    blockSignals(true);
  main_window_ui_->dial3DOF->    blockSignals(true);
  main_window_ui_->dial4DOF->    blockSignals(true);
  main_window_ui_->dial5DOF->    blockSignals(true);
  main_window_ui_->dial6DOF->    blockSignals(true);

  main_window_ui_->dial1DOF->    setValue((main_window_ui_->spinBox1DOF->value() / FACTOR));
  main_window_ui_->dial2DOF->    setValue((main_window_ui_->spinBox2DOF->value() / FACTOR));
  main_window_ui_->dial3DOF->    setValue((main_window_ui_->spinBox3DOF->value() / FACTOR));
  main_window_ui_->dial4DOF->    setValue((main_window_ui_->spinBox4DOF->value() / FACTOR));
  main_window_ui_->dial5DOF->    setValue((main_window_ui_->spinBox5DOF->value() / FACTOR));
  main_window_ui_->dial6DOF->    setValue((main_window_ui_->spinBox6DOF->value() / FACTOR));

//  joint_positions_["joint_1"]= main_window_ui_.spinBox1DOF->value()/ToG;
//  joint_positions_["joint_2"]= main_window_ui_.spinBox2DOF->value()/ToG;
//  joint_positions_["joint_3"]= main_window_ui_.spinBox3DOF->value()/ToG;
//  joint_positions_["joint_4"]= main_window_ui_.spinBox4DOF->value()/ToG;
//  joint_positions_["joint_5"]= main_window_ui_.spinBox5DOF->value()/ToG;
//  joint_positions_["joint_6"]= main_window_ui_.spinBox6DOF->value()/ToG;


  main_window_ui_->dial1DOF->    blockSignals(false);
  main_window_ui_->dial2DOF->    blockSignals(false);
  main_window_ui_->dial3DOF->    blockSignals(false);
  main_window_ui_->dial4DOF->    blockSignals(false);
  main_window_ui_->dial5DOF->    blockSignals(false);
  main_window_ui_->dial6DOF->    blockSignals(false);

//  j(nj)=0;
//  j(0) = main_window_ui_.spinBox1DOF->value()/ToG;
//  j(1) = main_window_ui_.spinBox2DOF->value()/ToG;
//  j(2) = main_window_ui_.spinBox3DOF->value()/ToG;
//  j(3) = main_window_ui_.spinBox4DOF->value()/ToG;
//  j(4) = main_window_ui_.spinBox5DOF->value()/ToG;
//  j(5) = main_window_ui_.spinBox6DOF->value()/ToG;

//  if(!jointsv->ForwardK(pos_mat, j, nj))
//  {
//           std::cerr << "Error at Publish Joint for FKinematics" <<std::endl;
//  }
//  stringX = QString::number(pos_mat.p.x()); //Convert Double to String
//  stringY = QString::number(pos_mat.p.y());
//  stringZ = QString::number(pos_mat.p.z());

//  pos_mat.M.GetRPY(roll, pitch, yaw);
//  stringYaw   = QString::number(yaw); //Convert Double to String
//  stringPitch = QString::number(pitch);
//  stringRoll  = QString::number(roll);

//   main_window_ui_.fkX->setText(stringX);
//   main_window_ui_.fkY->setText(stringY);
//   main_window_ui_.fkZ->setText(stringZ);

//   main_window_ui_.fkYaw->setText(stringYaw);
//   main_window_ui_.fkPitch->setText(stringPitch);
//   main_window_ui_.fkRoll->setText(stringRoll);


}

void ROSGUI::updateSpinboxesD()
{

  arduino.data.resize(6);

    main_window_ui_->spinBox1DOF->    blockSignals(true);
    main_window_ui_->spinBox2DOF->    blockSignals(true);
    main_window_ui_->spinBox3DOF->    blockSignals(true);
    main_window_ui_->spinBox4DOF->    blockSignals(true);
    main_window_ui_->spinBox5DOF->    blockSignals(true);
    main_window_ui_->spinBox6DOF->    blockSignals(true);

    main_window_ui_->spinBox1DOF->    setValue((main_window_ui_->dial1DOF->value() * FACTOR));
    main_window_ui_->spinBox2DOF->    setValue((main_window_ui_->dial2DOF->value() * FACTOR));
    main_window_ui_->spinBox3DOF->    setValue((main_window_ui_->dial3DOF->value() * FACTOR));
    main_window_ui_->spinBox4DOF->    setValue((main_window_ui_->dial4DOF->value() * FACTOR));
    main_window_ui_->spinBox5DOF->    setValue((main_window_ui_->dial5DOF->value() * FACTOR));
    main_window_ui_->spinBox6DOF->    setValue((main_window_ui_->dial6DOF->value() * FACTOR));

    joint_positions_["joint_1"]= main_window_ui_->dial1DOF->value()/ToG;
    joint_positions_["joint_2"]= main_window_ui_->dial2DOF->value()/ToG;
    joint_positions_["joint_3"]= main_window_ui_->dial3DOF->value()/ToG;
    joint_positions_["joint_4"]= main_window_ui_->dial4DOF->value()/ToG;
    joint_positions_["joint_5"]= main_window_ui_->dial5DOF->value()/ToG;
    joint_positions_["joint_6"]= main_window_ui_->dial6DOF->value()/ToG;



    main_window_ui_->spinBox1DOF->    blockSignals(false);
    main_window_ui_->spinBox2DOF->    blockSignals(false);
    main_window_ui_->spinBox3DOF->    blockSignals(false);
    main_window_ui_->spinBox4DOF->    blockSignals(false);
    main_window_ui_->spinBox5DOF->    blockSignals(false);
    main_window_ui_->spinBox6DOF->    blockSignals(false);

    j(nj)=0;
    j(0) = main_window_ui_->dial1DOF->value()/ToG;
    j(1) = main_window_ui_->dial2DOF->value()/ToG;
    j(2) = main_window_ui_->dial3DOF->value()/ToG;
    j(3) = main_window_ui_->dial4DOF->value()/ToG;
    j(4) = main_window_ui_->dial5DOF->value()/ToG;
    j(5) = main_window_ui_->dial6DOF->value()/ToG;


//Vigir
     jointV.resize(6);
     jointV[0] = main_window_ui_->spinBox1DOF->value()/ToG;
     jointV[1] = main_window_ui_->spinBox2DOF->value()/ToG;
     jointV[2] = main_window_ui_->spinBox3DOF->value()/ToG;
     jointV[3] = main_window_ui_->spinBox4DOF->value()/ToG;
     jointV[4] = main_window_ui_->spinBox5DOF->value()/ToG;
     jointV[5] = main_window_ui_->spinBox6DOF->value()/ToG;
  arduino.data[0]=  jointV[0] ;
  arduino.data[1]=  jointV[1] ;
  arduino.data[2] = jointV[2] ;
  arduino.data[3] = jointV[3] ;
  arduino.data[4] = jointV[4] ;
  arduino.data[5] = jointV[5] ;

     //Datos de Joints a MoveIT
//     mRviz->refreshJoint(jointV);
     this->FKdata(j);
     arduinopub.publish(arduino);

}


void ROSGUI::on_1DOF()
{
        main_window_ui_->dial1DOF->   setEnabled(true);
        main_window_ui_->dial2DOF->   setEnabled(false);
        main_window_ui_->dial3DOF->   setEnabled(false);
        main_window_ui_->dial4DOF->   setEnabled(false);
        main_window_ui_->dial5DOF->   setEnabled(false);
        main_window_ui_->dial6DOF->   setEnabled(false);
        main_window_ui_->spinBox1DOF->setEnabled(true);
        main_window_ui_->spinBox2DOF->setEnabled(false);
        main_window_ui_->spinBox3DOF->setEnabled(false);
        main_window_ui_->spinBox4DOF->setEnabled(false);
        main_window_ui_->spinBox5DOF->setEnabled(false);
        main_window_ui_->spinBox6DOF->setEnabled(false);
}


void ROSGUI::on_2DOF()
{
        main_window_ui_->dial1DOF->   setEnabled(true);
        main_window_ui_->dial2DOF->   setEnabled(true);
        main_window_ui_->dial3DOF->   setEnabled(false);
        main_window_ui_->dial4DOF->   setEnabled(false);
        main_window_ui_->dial5DOF->   setEnabled(false);
        main_window_ui_->dial6DOF->   setEnabled(false);
        main_window_ui_->spinBox1DOF->setEnabled(true);
        main_window_ui_->spinBox2DOF->setEnabled(true);
        main_window_ui_->spinBox3DOF->setEnabled(false);
        main_window_ui_->spinBox4DOF->setEnabled(false);
        main_window_ui_->spinBox5DOF->setEnabled(false);
        main_window_ui_->spinBox6DOF->setEnabled(false);
}

void ROSGUI::on_3DOF()
{
        main_window_ui_->dial1DOF->   setEnabled(true);
        main_window_ui_->dial2DOF->   setEnabled(true);
        main_window_ui_->dial3DOF->   setEnabled(true);
        main_window_ui_->dial4DOF->   setEnabled(false);
        main_window_ui_->dial5DOF->   setEnabled(false);
        main_window_ui_->dial6DOF->   setEnabled(false);
        main_window_ui_->spinBox1DOF->setEnabled(true);
        main_window_ui_->spinBox2DOF->setEnabled(true);
        main_window_ui_->spinBox3DOF->setEnabled(true);
        main_window_ui_->spinBox4DOF->setEnabled(false);
        main_window_ui_->spinBox5DOF->setEnabled(false);
        main_window_ui_->spinBox6DOF->setEnabled(false);
}

void ROSGUI::on_4DOF()
{
        main_window_ui_->dial1DOF->   setEnabled(true);
        main_window_ui_->dial2DOF->   setEnabled(true);
        main_window_ui_->dial3DOF->   setEnabled(true);
        main_window_ui_->dial4DOF->   setEnabled(true);
        main_window_ui_->dial5DOF->   setEnabled(false);
        main_window_ui_->dial6DOF->   setEnabled(false);
        main_window_ui_->spinBox1DOF->setEnabled(true);
        main_window_ui_->spinBox2DOF->setEnabled(true);
        main_window_ui_->spinBox3DOF->setEnabled(true);
        main_window_ui_->spinBox4DOF->setEnabled(true);
        main_window_ui_->spinBox5DOF->setEnabled(false);
        main_window_ui_->spinBox6DOF->setEnabled(false);

}


void ROSGUI::on_5DOF()
{
        main_window_ui_->dial1DOF->   setEnabled(true);
        main_window_ui_->dial2DOF->   setEnabled(true);
        main_window_ui_->dial3DOF->   setEnabled(true);
        main_window_ui_->dial4DOF->   setEnabled(true);
        main_window_ui_->dial5DOF->   setEnabled(true);
        main_window_ui_->dial6DOF->   setEnabled(false);
        main_window_ui_->spinBox1DOF->setEnabled(true);
        main_window_ui_->spinBox2DOF->setEnabled(true);
        main_window_ui_->spinBox3DOF->setEnabled(true);
        main_window_ui_->spinBox4DOF->setEnabled(true);
        main_window_ui_->spinBox5DOF->setEnabled(true);
        main_window_ui_->spinBox6DOF->setEnabled(false);

}


void ROSGUI::on_6DOF()
{
       main_window_ui_->dial1DOF->   setEnabled(true);
       main_window_ui_->dial2DOF->   setEnabled(true);
       main_window_ui_->dial3DOF->   setEnabled(true);
       main_window_ui_->dial4DOF->   setEnabled(true);
       main_window_ui_->dial5DOF->   setEnabled(true);
       main_window_ui_->dial6DOF->   setEnabled(true);
       main_window_ui_->spinBox1DOF->setEnabled(true);
       main_window_ui_->spinBox2DOF->setEnabled(true);
       main_window_ui_->spinBox3DOF->setEnabled(true);
       main_window_ui_->spinBox4DOF->setEnabled(true);
       main_window_ui_->spinBox5DOF->setEnabled(true);
       main_window_ui_->spinBox6DOF->setEnabled(true);
}

void ROSGUI::offWidgets(){
  main_window_ui_->dial1DOF->   setEnabled(false);
  main_window_ui_->dial2DOF->   setEnabled(false);
  main_window_ui_->dial3DOF->   setEnabled(false);
  main_window_ui_->dial4DOF->   setEnabled(false);
  main_window_ui_->dial5DOF->   setEnabled(false);
  main_window_ui_->dial6DOF->   setEnabled(false);
  main_window_ui_->spinBox1DOF->setEnabled(false);
  main_window_ui_->spinBox2DOF->setEnabled(false);
  main_window_ui_->spinBox3DOF->setEnabled(false);
  main_window_ui_->spinBox4DOF->setEnabled(false);
  main_window_ui_->spinBox5DOF->setEnabled(false);
  main_window_ui_->spinBox6DOF->setEnabled(false);
}


//void ROSGUI::on_checkBox_2_toggled(int checked)
//{
//  if (checked==Qt::Checked)
//  {
//   mRviz->refreshTF(true);
//  }
//   else
//  {
// //tfrv=true;
//   mRviz->refreshTF(false);
//  }
//}

//void ROSGUI::on_checkBox_3_toggled(int checked=1)
//{
//  if (checked==Qt::Checked)
//  {
//   mRviz->refreshRM(true);
//  }
//   else
//  {
//    mRviz->refreshRM(false);
//  }
//}

void ROSGUI::on_comboBox_currentIndexChanged(int index=0)
{
  switch (index){
   case 0:
  {
    mRviz->refreshTF(false, false, false);
    mRviz->refreshRM(true);

   break;
  }
   case 1:
 {
    mRviz->refreshTF(true, true, false);
    mRviz->refreshRM(true);
   break;
 }
   case 2:
  {
    mRviz->refreshTF(true, true, true);
    mRviz->refreshRM(false);
   break;
  }
   case 3:
 {
    mRviz->refreshTF(true, true, false);
    mRviz->refreshRM(false);
   break;
 }
   case 4:
 {
    mRviz->refreshTF(true, true, true);
    mRviz->refreshRM(true);

   break;
   }
  }
}

void ROSGUI::on_comboBox_2_currentIndexChanged(int index=0)
{
  mRviz->setTool(index);
  if (index==4 || index==5){
  main_window_ui_->comboBox_2->setCurrentIndex(0);
  }
  Q_EMIT statusTool(index) ;

}

void ROSGUI::on_checkBox_3_toggled(bool x)
{

  Ws=!Ws;
  if (x){
    std::cout << "hola checkbox3" <<std::endl;

  }
  mRviz->refreshWs(x);

}

void ROSGUI::on_enableW() {

    main_window_ui_->pushIK->setEnabled(true);
    main_window_ui_->PushTrajectory->setEnabled(true);
    main_window_ui_->groupBox->setEnabled(true);

    main_window_ui_->xBox->    setEnabled(true);
    main_window_ui_->yBox->    setEnabled(true);
    main_window_ui_->zBox->    setEnabled(true);
    main_window_ui_->rollBox-> setEnabled(true);
    main_window_ui_->pitchBox->setEnabled(true);
    main_window_ui_->yawBox->  setEnabled(true);

    main_window_ui_->xSlider->     setEnabled(true);
    main_window_ui_->ySlider->     setEnabled(true);
    main_window_ui_->zSlider->     setEnabled(true);
    main_window_ui_->rollSlider->  setEnabled(true);
    main_window_ui_->pitchSlider-> setEnabled(true);
    main_window_ui_->yawSlider->   setEnabled(true);
}



void ROSGUI::on_checkBox_DH_toggled(bool y)
{


  DHb = y;

}

bool ROSGUI::init()
{

this->on_enableW();

  jointsv = new modelparam;

 if(!jointsv->initmodel(nj, DHb))
 {
          std::cerr << "Error at init model" <<std::endl;
         return false;
 }



 if(!jointsv->readJntLimitsFromROSParamURDF(
                                      joints_lower_limit_
                                      , joints_upper_limit_
                                      ))
 {
          std::cerr << "Error at readJntLimitsFromROSParamURDF" <<std::endl;
         return false;
 }

  KDL::Vector tcpXYZ  = KDL::Vector(0.0,0.0,0.0);
  KDL::Rotation tcpRPY= KDL::Rotation(0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);


//  if (!jointsv->InverseK(tcpXYZ, tcpRPY, pos_joint));
//  {
//    std::cerr << "Error at Publish Joint for IKinematics" <<std::endl;
//   return false;
//  }

  j(nj) =  0;
  this->FKdata(j);
   return true;

}

std::vector<double> ROSGUI::getJointLowerLimits()
{   //joints= new modelparam;
      std::vector< double > readL = joints_lower_limit_;
    return readL;
}

std::vector< double > ROSGUI::getJointUpperLimits()
{
    std::vector< double > readU = joints_upper_limit_;
    return readU;
}


void ROSGUI::updatetoURDF()
{
  if(!init())
  {
     ROS_ERROR("Error publisher");
  }
  //radians for revolute, meters for prismatic
  joint_lower = getJointLowerLimits();
  joint_upper = getJointUpperLimits();
 // chainFK     = chain();


 std::cout << " px "<< pos_mat6.p.x() <<	" py "<< pos_mat6.p.y() <<	" pz "<< pos_mat6.p.z() <<std::endl;

 // std::cout << std::setprecision(3) << pos_mat << "\t\t";

 stringX = QString::number(pos_mat6.p.x()); //Convert Double to String
 stringY = QString::number(pos_mat6.p.y());
 stringZ = QString::number(pos_mat6.p.z());

 pos_mat6.M.GetRPY(roll, pitch, yaw);
 stringYaw   = QString::number(yaw*ToG); //Convert Double to String
 stringPitch = QString::number(pitch*ToG);
 stringRoll  = QString::number(roll*ToG);

  main_window_ui_->fkX->setText(stringX);
  main_window_ui_->fkY->setText(stringY);
  main_window_ui_->fkZ->setText(stringZ);

  main_window_ui_->fkYaw->setText(stringYaw);
  main_window_ui_->fkPitch->setText(stringPitch);
  main_window_ui_->fkRoll->setText(stringRoll);

//if (main_window_ui_->checkBoxPrismatic->checkState()){
//ToG=1;


  main_window_ui_->dial1DOF->setMinimum(joint_lower[0]*ToG);
  main_window_ui_->dial1DOF->setMaximum(joint_upper[0]*ToG);
  main_window_ui_->dial1DOF->setSingleStep(1);
  main_window_ui_->spinBox1DOF->setMinimum(joint_lower[0]*ToG);
  main_window_ui_->spinBox1DOF->setMaximum(joint_upper[0]*ToG);
  main_window_ui_->spinBox1DOF->setSingleStep(1);

  main_window_ui_->dial2DOF->setMinimum(joint_lower[1]*ToG);
  main_window_ui_->dial2DOF->setMaximum(joint_upper[1]*ToG);
  main_window_ui_->dial2DOF->setSingleStep(1);
  main_window_ui_->spinBox2DOF->setMinimum(joint_lower[1]*ToG);
  main_window_ui_->spinBox2DOF->setMaximum(joint_upper[1]*ToG);
  main_window_ui_->spinBox2DOF->setSingleStep(1);

  main_window_ui_->dial3DOF->setMinimum(joint_lower[2]*ToG);
  main_window_ui_->dial3DOF->setMaximum(joint_upper[2]*ToG);
  main_window_ui_->dial3DOF->setSingleStep(1);
  main_window_ui_->spinBox3DOF->setMinimum(joint_lower[2]*ToG);
  main_window_ui_->spinBox3DOF->setMaximum(joint_upper[2]*ToG);
  main_window_ui_->spinBox3DOF->setSingleStep(1);

  main_window_ui_->dial4DOF->setMinimum(joint_lower[3]*ToG);
  main_window_ui_->dial4DOF->setMaximum(joint_upper[3]*ToG);
  main_window_ui_->dial4DOF->setSingleStep(1);
  main_window_ui_->spinBox4DOF->setMinimum(joint_lower[3]*ToG);
  main_window_ui_->spinBox4DOF->setMaximum(joint_upper[3]*ToG);
  main_window_ui_->spinBox4DOF->setSingleStep(1);

  main_window_ui_->dial5DOF->setMinimum(joint_lower[4]*ToG);
  main_window_ui_->dial5DOF->setMaximum(joint_upper[4]*ToG);
  main_window_ui_->dial5DOF->setSingleStep(1);
  main_window_ui_->spinBox5DOF->setMinimum(joint_lower[4]*ToG);
  main_window_ui_->spinBox5DOF->setMaximum(joint_upper[4]*ToG);
  main_window_ui_->spinBox5DOF->setSingleStep(1);

  main_window_ui_->dial6DOF->setMinimum(joint_lower[5]*ToG);
  main_window_ui_->dial6DOF->setMaximum(joint_upper[5]*ToG);
  main_window_ui_->dial6DOF->setSingleStep(1);
  main_window_ui_->spinBox6DOF->setMinimum(joint_lower[5]*ToG);
  main_window_ui_->spinBox6DOF->setMaximum(joint_upper[5]*ToG);
  main_window_ui_->spinBox6DOF->setSingleStep(1);
  send_val.data.resize(12);
  for (int i =0; i<6;i++)
  {
  send_val.data[i] = joint_lower[i]*ToG;
  send_val.data[i+6] = joint_upper[i]*ToG;
}
  joint_value_pub.publish(send_val);

  }

void ROSGUI::resetvalue(){
  main_window_ui_->dial1DOF->    setValue(resetv);
  main_window_ui_->dial2DOF->    setValue(resetv);
  main_window_ui_->dial3DOF->    setValue(resetv);
  main_window_ui_->dial4DOF->    setValue(resetv);
  main_window_ui_->dial5DOF->    setValue(resetv);
  main_window_ui_->dial6DOF->    setValue(resetv);


  main_window_ui_->spinBox1DOF->    setValue(resetv);
  main_window_ui_->spinBox2DOF->    setValue(resetv);
  main_window_ui_->spinBox3DOF->    setValue(resetv);
  main_window_ui_->spinBox4DOF->    setValue(resetv);
  main_window_ui_->spinBox5DOF->    setValue(resetv);
  main_window_ui_->spinBox6DOF->    setValue(resetv);
}
void ROSGUI::executeIK(){
  std::vector<double> jointvalues(6);
  trajectory_msgs::JointTrajectory msg;

  KDL::Vector tcpXYZ= KDL::Vector(main_window_ui_->xBox->value(),main_window_ui_->yBox->value(),main_window_ui_->zBox->value());
  KDL::Rotation tcpRPY= KDL::Rotation::RPY(main_window_ui_->xSlider->value(),main_window_ui_->ySlider->value(),main_window_ui_->zSlider->value());

//  KDL::Vector tcpXYZ  = KDL::Vector(0.2,0.0,0.0);
  //KDL::Rotation tcpRPY= KDL::Rotation(0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);

//  KDL::Rotation tcpRPY= KDL::Rotation::RPY(0.0,0.0,0.0);
  std::cout << "KDL ROT  " << nj <<std::endl;

    if (!jointsv->InverseK(tcpXYZ, tcpRPY, pos_joint))
    {
      std::cout << "Error IK" <<std::endl;
    }

    for(int i=0;i<nj;i++){
    jointvalues[i] = pos_joint(i)*ToG; //array [i]
    std::cout << pos_joint(i)*ToG <<std::endl;
    std::cout << "Publish Joint for IKinematics" <<std::endl;
       }

    main_window_ui_->joint1->setText(QString::number(jointvalues[0]));
    main_window_ui_->joint2->setText(QString::number(jointvalues[1]));
    main_window_ui_->joint3->setText(QString::number(jointvalues[2]));
    main_window_ui_->joint4->setText(QString::number(jointvalues[3]));
    main_window_ui_->joint5->setText(QString::number(jointvalues[4]));
    main_window_ui_->joint6->setText(QString::number(jointvalues[5]));

    msg = this->createArmPositionCommand(jointvalues);
     joint_pub.publish(msg) ;

}

void ROSGUI::executeFK(){

  trajectory_msgs::JointTrajectory msg;
  std::vector<double> jointvalues(6); //joint Values format Double

//  j(nj)=0;
//  j(0) = main_window_ui_->spinBox1DOF->value()/ToG;
//  j(1) = main_window_ui_->spinBox2DOF->value()/ToG;
//  j(2) = main_window_ui_->spinBox3DOF->value()/ToG;
//  j(3) = main_window_ui_->spinBox4DOF->value()/ToG;
//  j(4) = main_window_ui_->spinBox5DOF->value()/ToG;
//  j(5) = main_window_ui_->spinBox6DOF->value()/ToG;


//     // move arm straight up
     jointvalues[0] = main_window_ui_->spinBox1DOF->value();
     jointvalues[1] = main_window_ui_->spinBox2DOF->value();
     jointvalues[2] = main_window_ui_->spinBox3DOF->value();
     jointvalues[3] = main_window_ui_->spinBox4DOF->value();
     jointvalues[4] = main_window_ui_->spinBox5DOF->value();
     jointvalues[5] = main_window_ui_->spinBox6DOF->value();
     msg = this->createArmPositionCommand(jointvalues);
     joint_pub.publish(msg);


//     ros::Duration(5).sleep();

//     // move arm back
//     jointvalues[0] = 0.11;
//     jointvalues[1] = 0.11;
//     jointvalues[2] = -0.11;
//     jointvalues[3] = 0.11;
//     jointvalues[4] = 0.111;
//     msg = this->createArmPositionCommand(jointvalues);
//     joint_pub.publish(msg); // son publicados en el nodo

   //this->FKdata(j);

}
void ROSGUI::timeOut()
{

}

void ROSGUI::FKdata(KDL::JntArray j){


  if(!jointsv->ForwardK(pos_mat6,
                        pos_mat5,
                        pos_mat4,
                        pos_mat3,
                        pos_mat2,
                        pos_mat1,
                        j))
  {
           std::cerr << "Error at Publish Joint for FKinematics" <<std::endl;
  }
  stringX = QString::number(pos_mat6.p.x()); //Convert Double to String
  stringY = QString::number(pos_mat6.p.y());
  stringZ = QString::number(pos_mat6.p.z());

  pos_mat6.M.GetRPY(roll, pitch, yaw);

  stringYaw   = QString::number(yaw*ToG); //Convert Double to String
  stringPitch = QString::number(pitch*ToG);
  stringRoll  = QString::number(roll*ToG);

   main_window_ui_->fkX->setText(stringX);
   main_window_ui_->fkY->setText(stringY);
   main_window_ui_->fkZ->setText(stringZ);

   main_window_ui_->fkYaw->setText(stringYaw);
   main_window_ui_->fkPitch->setText(stringPitch);
   main_window_ui_->fkRoll->setText(stringRoll);

    main_window_ui_->label_1MT1x1->setText(QString::number(pos_mat1.M.UnitX().x()));
    main_window_ui_->label_1MT1x2->setText(QString::number(pos_mat1.M.UnitX().y()));
    main_window_ui_->label_1MT1x3->setText(QString::number(pos_mat1.M.UnitX().z()));
    main_window_ui_->label_1MT1x4->setText(QString::number(pos_mat1.p.x()));
    main_window_ui_->label_1MT2x1->setText(QString::number(pos_mat1.M.UnitY().x()));
    main_window_ui_->label_1MT2x2->setText(QString::number(pos_mat1.M.UnitY().y()));
    main_window_ui_->label_1MT2x3->setText(QString::number(pos_mat1.M.UnitY().z()));
    main_window_ui_->label_1MT2x4->setText(QString::number(pos_mat1.p.y()));
    main_window_ui_->label_1MT3x1->setText(QString::number(pos_mat1.M.UnitZ().x()));
    main_window_ui_->label_1MT3x2->setText(QString::number(pos_mat1.M.UnitZ().y()));
    main_window_ui_->label_1MT3x3->setText(QString::number(pos_mat1.M.UnitZ().z()));
    main_window_ui_->label_1MT3x4->setText(QString::number(pos_mat1.p.z()));
    main_window_ui_->label_1MT4x1->setText(QString("0"));
    main_window_ui_->label_1MT4x2->setText(QString("0"));
    main_window_ui_->label_1MT4x3->setText(QString("0"));
    main_window_ui_->label_1MT4x4->setText(QString("1"));

    main_window_ui_->label_2MT1x1->setText(QString::number(pos_mat2.M.UnitX().x()));
    main_window_ui_->label_2MT1x2->setText(QString::number(pos_mat2.M.UnitX().y()));
    main_window_ui_->label_2MT1x3->setText(QString::number(pos_mat2.M.UnitX().z()));
    main_window_ui_->label_2MT1x4->setText(QString::number(pos_mat2.p.x()));
    main_window_ui_->label_2MT2x1->setText(QString::number(pos_mat2.M.UnitY().x()));
    main_window_ui_->label_2MT2x2->setText(QString::number(pos_mat2.M.UnitY().y()));
    main_window_ui_->label_2MT2x3->setText(QString::number(pos_mat2.M.UnitY().z()));
    main_window_ui_->label_2MT2x4->setText(QString::number(pos_mat2.p.y()));
    main_window_ui_->label_2MT3x1->setText(QString::number(pos_mat2.M.UnitZ().x()));
    main_window_ui_->label_2MT3x2->setText(QString::number(pos_mat2.M.UnitZ().y()));
    main_window_ui_->label_2MT3x3->setText(QString::number(pos_mat2.M.UnitZ().z()));
    main_window_ui_->label_2MT3x4->setText(QString::number(pos_mat2.p.z()));
    main_window_ui_->label_2MT4x1->setText(QString("0"));
    main_window_ui_->label_2MT4x2->setText(QString("0"));
    main_window_ui_->label_2MT4x3->setText(QString("0"));
    main_window_ui_->label_2MT4x4->setText(QString("1"));

    main_window_ui_->label_3MT1x1->setText(QString::number(pos_mat3.M.UnitX().x()));
    main_window_ui_->label_3MT1x2->setText(QString::number(pos_mat3.M.UnitX().y()));
    main_window_ui_->label_3MT1x3->setText(QString::number(pos_mat3.M.UnitX().z()));
    main_window_ui_->label_3MT1x4->setText(QString::number(pos_mat3.p.x()));
    main_window_ui_->label_3MT2x1->setText(QString::number(pos_mat3.M.UnitY().x()));
    main_window_ui_->label_3MT2x2->setText(QString::number(pos_mat3.M.UnitY().y()));
    main_window_ui_->label_3MT2x3->setText(QString::number(pos_mat3.M.UnitY().z()));
    main_window_ui_->label_3MT2x4->setText(QString::number(pos_mat3.p.y()));
    main_window_ui_->label_3MT3x1->setText(QString::number(pos_mat3.M.UnitZ().x()));
    main_window_ui_->label_3MT3x2->setText(QString::number(pos_mat3.M.UnitZ().y()));
    main_window_ui_->label_3MT3x3->setText(QString::number(pos_mat3.M.UnitZ().z()));
    main_window_ui_->label_3MT3x4->setText(QString::number(pos_mat3.p.z()));
    main_window_ui_->label_3MT4x1->setText(QString("0"));
    main_window_ui_->label_3MT4x2->setText(QString("0"));
    main_window_ui_->label_3MT4x3->setText(QString("0"));
    main_window_ui_->label_3MT4x4->setText(QString("1"));

    main_window_ui_->label_4MT1x1->setText(QString::number(pos_mat4.M.UnitX().x()));
    main_window_ui_->label_4MT1x2->setText(QString::number(pos_mat4.M.UnitX().y()));
    main_window_ui_->label_4MT1x3->setText(QString::number(pos_mat4.M.UnitX().z()));
    main_window_ui_->label_4MT1x4->setText(QString::number(pos_mat4.p.x()));
    main_window_ui_->label_4MT2x1->setText(QString::number(pos_mat4.M.UnitY().x()));
    main_window_ui_->label_4MT2x2->setText(QString::number(pos_mat4.M.UnitY().y()));
    main_window_ui_->label_4MT2x3->setText(QString::number(pos_mat4.M.UnitY().z()));
    main_window_ui_->label_4MT2x4->setText(QString::number(pos_mat4.p.y()));
    main_window_ui_->label_4MT3x1->setText(QString::number(pos_mat4.M.UnitZ().x()));
    main_window_ui_->label_4MT3x2->setText(QString::number(pos_mat4.M.UnitZ().y()));
    main_window_ui_->label_4MT3x3->setText(QString::number(pos_mat4.M.UnitZ().z()));
    main_window_ui_->label_4MT3x4->setText(QString::number(pos_mat4.p.z()));
    main_window_ui_->label_4MT4x1->setText(QString("0"));
    main_window_ui_->label_4MT4x2->setText(QString("0"));
    main_window_ui_->label_4MT4x3->setText(QString("0"));
    main_window_ui_->label_4MT4x4->setText(QString("1"));

    main_window_ui_->label_5MT1x1->setText(QString::number(pos_mat5.M.UnitX().x()));
    main_window_ui_->label_5MT1x2->setText(QString::number(pos_mat5.M.UnitX().y()));
    main_window_ui_->label_5MT1x3->setText(QString::number(pos_mat5.M.UnitX().z()));
    main_window_ui_->label_5MT1x4->setText(QString::number(pos_mat5.p.x()));
    main_window_ui_->label_5MT2x1->setText(QString::number(pos_mat5.M.UnitY().x()));
    main_window_ui_->label_5MT2x2->setText(QString::number(pos_mat5.M.UnitY().y()));
    main_window_ui_->label_5MT2x3->setText(QString::number(pos_mat5.M.UnitY().z()));
    main_window_ui_->label_5MT2x4->setText(QString::number(pos_mat5.p.y()));
    main_window_ui_->label_5MT3x1->setText(QString::number(pos_mat5.M.UnitZ().x()));
    main_window_ui_->label_5MT3x2->setText(QString::number(pos_mat5.M.UnitZ().y()));
    main_window_ui_->label_5MT3x3->setText(QString::number(pos_mat5.M.UnitZ().z()));
    main_window_ui_->label_5MT3x4->setText(QString::number(pos_mat5.p.z()));
    main_window_ui_->label_5MT4x1->setText(QString("0"));
    main_window_ui_->label_5MT4x2->setText(QString("0"));
    main_window_ui_->label_5MT4x3->setText(QString("0"));
    main_window_ui_->label_5MT4x4->setText(QString("1"));

    main_window_ui_->label_6MT1x1->setText(QString::number(pos_mat6.M.UnitX().x()));
    main_window_ui_->label_6MT1x2->setText(QString::number(pos_mat6.M.UnitX().y()));
    main_window_ui_->label_6MT1x3->setText(QString::number(pos_mat6.M.UnitX().z()));
    main_window_ui_->label_6MT1x4->setText(QString::number(pos_mat6.p.x()));
    main_window_ui_->label_6MT2x1->setText(QString::number(pos_mat6.M.UnitY().x()));
    main_window_ui_->label_6MT2x2->setText(QString::number(pos_mat6.M.UnitY().y()));
    main_window_ui_->label_6MT2x3->setText(QString::number(pos_mat6.M.UnitY().z()));
    main_window_ui_->label_6MT2x4->setText(QString::number(pos_mat6.p.y()));
    main_window_ui_->label_6MT3x1->setText(QString::number(pos_mat6.M.UnitZ().x()));
    main_window_ui_->label_6MT3x2->setText(QString::number(pos_mat6.M.UnitZ().y()));
    main_window_ui_->label_6MT3x3->setText(QString::number(pos_mat6.M.UnitZ().z()));
    main_window_ui_->label_6MT3x4->setText(QString::number(pos_mat6.p.z()));
    main_window_ui_->label_6MT4x1->setText(QString("0"));
    main_window_ui_->label_6MT4x2->setText(QString("0"));
    main_window_ui_->label_6MT4x3->setText(QString("0"));
    main_window_ui_->label_6MT4x4->setText(QString("1"));


}
trajectory_msgs::JointTrajectory ROSGUI::createArmPositionCommand(std::vector<double>& newPositions)
{
  int numberOfJoints = 6;
  trajectory_msgs::JointTrajectory msg;

  trajectory_msgs::JointTrajectoryPoint point;
/*  msg.points.resize(1);
  point.positions.resize(5); */       // Do not resize like That ----> | point.resize | point[0].positions|
  for (int i = 0; i < 6; i++) {
    point.positions.push_back(newPositions[i]);
    point.velocities.push_back(70);
    point.accelerations.push_back(0);
  }
  point.time_from_start = ros::Duration(0.1);
  msg.points.push_back(point);
  msg.points.push_back(point);

  for (int i = 0; i < 6; i++) {
    std::stringstream jointName;
    jointName << "joint_" << (i + 1);
    msg.joint_names.push_back(jointName.str());
  }

  msg.header.frame_id = "my_lab_uni";
  msg.header.stamp = ros::Time::now();

  return msg; //Son Empaquetados en Mensajes En formato de Joints
}

void ROSGUI::trajectoryCallback(const trajectory_msgs::JointTrajectory &msg)
{
// double joint_positions[6] {0.0};
// int i =0;
//  joint_positions[i]=msg.points[i].positions[i];
  arduino.data.resize(6);
     joint_positions_["joint_1"]= msg.points[0].positions[0]/ToG;
     joint_positions_["joint_2"]= msg.points[0].positions[1]/ToG;
     joint_positions_["joint_3"]= msg.points[0].positions[2]/ToG;
     joint_positions_["joint_4"]= msg.points[0].positions[3]/ToG;
     joint_positions_["joint_5"]= msg.points[0].positions[4]/ToG;
     joint_positions_["joint_6"]= msg.points[0].positions[5]/ToG;

//ROS_INFO("the [%d] positions are[%f]\n",i,joint_positions[0]/*,joint_positions[1]*//*,joint_positions[2],joint_positions[3],joint_positions[4],joint_positions[5]*/);
std::cout <<  msg << std::endl;

  j(nj)=0;
  j(0) = msg.points[0].positions[0]/ToG;
  j(1) = msg.points[0].positions[1]/ToG;
  j(2) = msg.points[0].positions[2]/ToG;
  j(3) = msg.points[0].positions[3]/ToG;
  j(4) = msg.points[0].positions[4]/ToG;
  j(5) = msg.points[0].positions[5]/ToG;
  main_window_ui_->spinBox1DOF->    setValue( j(0)*ToG);
  main_window_ui_->spinBox2DOF->    setValue( j(1)*ToG);
  main_window_ui_->spinBox3DOF->    setValue( j(2)*ToG);
  main_window_ui_->spinBox4DOF->    setValue( j(3)*ToG);
  main_window_ui_->spinBox5DOF->    setValue( j(4)*ToG);
  main_window_ui_->spinBox6DOF->    setValue( j(5)*ToG);

this->updateDialer();
this->FKdata(j);
    joint_1_plot = j(0)*ToG;
    joint_2_plot = j(1)*ToG;
    joint_3_plot = j(2)*ToG;
    joint_4_plot = j(3)*ToG;
    joint_5_plot = j(4)*ToG;
    joint_6_plot = j(5)*ToG;

  arduino.data[0]= joint_1_plot;
  arduino.data[1]= joint_2_plot;
  arduino.data[2] =joint_3_plot;
  arduino.data[3] =joint_4_plot;
  arduino.data[4] =joint_5_plot;
  arduino.data[5] =joint_6_plot;
     arduinopub.publish(arduino);


//ros::Rate loop_rate(10);

//while(true)
//{
//   // lock the state publisher objects and run

////    if(robot_state_pub_ != NULL)
////    {
//       robot_state_pub_->publishTransforms(joint_positions1_, ros::Time::now()/*+ msg.points[0].time_from_start*/, "my_lab_uni");
//       robot_state_pub_->publishFixedTransforms("my_lab_uni");

////    }
//    loop_rate.sleep();

// }

}



void ROSGUI::on_spinBox_valueChanged(int arg1)
{
  std::cout <<  arg1 << std::endl;
  switch (arg1) {






    case 0:
    {
    main_window_ui_->lineDH11->setEnabled(false);
    main_window_ui_->lineDH12->setEnabled(false);
    main_window_ui_->lineDH13->setEnabled(false);
    main_window_ui_->lineDH14->setEnabled(false);

    main_window_ui_->lineDH11->setText("0");
    main_window_ui_->lineDH12->setText("0");
    main_window_ui_->lineDH13->setText("0");
    main_window_ui_->lineDH14->setText("0");

    main_window_ui_->lineDH21->setEnabled(false);
    main_window_ui_->lineDH22->setEnabled(false);
    main_window_ui_->lineDH23->setEnabled(false);
    main_window_ui_->lineDH24->setEnabled(false);

    main_window_ui_->lineDH31->setEnabled(false);
    main_window_ui_->lineDH32->setEnabled(false);
    main_window_ui_->lineDH33->setEnabled(false);
    main_window_ui_->lineDH34->setEnabled(false);

    main_window_ui_->lineDH41->setEnabled(false);
    main_window_ui_->lineDH42->setEnabled(false);
    main_window_ui_->lineDH43->setEnabled(false);
    main_window_ui_->lineDH44->setEnabled(false);

    main_window_ui_->lineDH51->setEnabled(false);
    main_window_ui_->lineDH52->setEnabled(false);
    main_window_ui_->lineDH53->setEnabled(false);
    main_window_ui_->lineDH54->setEnabled(false);

    main_window_ui_->lineDH61->setEnabled(false);
    main_window_ui_->lineDH62->setEnabled(false);
    main_window_ui_->lineDH63->setEnabled(false);
    main_window_ui_->lineDH64->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH1min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH1max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH1min->setValue(0);
    main_window_ui_->doubleSpinBoxDH1max->setValue(0);

    main_window_ui_->doubleSpinBoxDH2min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH2max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH3min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH3max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH4min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH4max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH5min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH5max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH6min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH6max->setEnabled(false);


    break;
    }
    case 1:
    {
    main_window_ui_->lineDH11->setEnabled(true);
    main_window_ui_->lineDH12->setEnabled(true);
    main_window_ui_->lineDH13->setEnabled(true);
    main_window_ui_->lineDH14->setEnabled(true);

    main_window_ui_->lineDH21->setEnabled(false);
    main_window_ui_->lineDH22->setEnabled(false);
    main_window_ui_->lineDH23->setEnabled(false);
    main_window_ui_->lineDH24->setEnabled(false);

    main_window_ui_->lineDH21->setText("0");
    main_window_ui_->lineDH22->setText("0");
    main_window_ui_->lineDH23->setText("0");
    main_window_ui_->lineDH24->setText("0");

    main_window_ui_->lineDH31->setEnabled(false);
    main_window_ui_->lineDH32->setEnabled(false);
    main_window_ui_->lineDH33->setEnabled(false);
    main_window_ui_->lineDH34->setEnabled(false);

    main_window_ui_->lineDH41->setEnabled(false);
    main_window_ui_->lineDH42->setEnabled(false);
    main_window_ui_->lineDH43->setEnabled(false);
    main_window_ui_->lineDH44->setEnabled(false);

    main_window_ui_->lineDH51->setEnabled(false);
    main_window_ui_->lineDH52->setEnabled(false);
    main_window_ui_->lineDH53->setEnabled(false);
    main_window_ui_->lineDH54->setEnabled(false);

    main_window_ui_->lineDH61->setEnabled(false);
    main_window_ui_->lineDH62->setEnabled(false);
    main_window_ui_->lineDH63->setEnabled(false);
    main_window_ui_->lineDH64->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH1min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH1max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH2min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH2max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH2min->setValue(0);
    main_window_ui_->doubleSpinBoxDH2max->setValue(0);

    main_window_ui_->doubleSpinBoxDH3min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH3max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH4min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH4max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH5min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH5max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH6min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH6max->setEnabled(false);
    main_window_ui_->checkBox_DH->setEnabled(true);

    Q_EMIT(on_1DOF());

    break;
    }

    case 2:
    {
    main_window_ui_->lineDH11->setEnabled(true);
    main_window_ui_->lineDH12->setEnabled(true);
    main_window_ui_->lineDH13->setEnabled(true);
    main_window_ui_->lineDH14->setEnabled(true);

    main_window_ui_->lineDH21->setEnabled(true);
    main_window_ui_->lineDH22->setEnabled(true);
    main_window_ui_->lineDH23->setEnabled(true);
    main_window_ui_->lineDH24->setEnabled(true);

    main_window_ui_->lineDH31->setEnabled(false);
    main_window_ui_->lineDH32->setEnabled(false);
    main_window_ui_->lineDH33->setEnabled(false);
    main_window_ui_->lineDH34->setEnabled(false);

    main_window_ui_->lineDH31->setText("0");
    main_window_ui_->lineDH32->setText("0");
    main_window_ui_->lineDH33->setText("0");
    main_window_ui_->lineDH34->setText("0");

    main_window_ui_->lineDH41->setEnabled(false);
    main_window_ui_->lineDH42->setEnabled(false);
    main_window_ui_->lineDH43->setEnabled(false);
    main_window_ui_->lineDH44->setEnabled(false);

    main_window_ui_->lineDH51->setEnabled(false);
    main_window_ui_->lineDH52->setEnabled(false);
    main_window_ui_->lineDH53->setEnabled(false);
    main_window_ui_->lineDH54->setEnabled(false);

    main_window_ui_->lineDH61->setEnabled(false);
    main_window_ui_->lineDH62->setEnabled(false);
    main_window_ui_->lineDH63->setEnabled(false);
    main_window_ui_->lineDH64->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH1min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH1max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH2min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH2max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH3min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH3max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH3min->setValue(0);
    main_window_ui_->doubleSpinBoxDH3max->setValue(0);

    main_window_ui_->doubleSpinBoxDH4min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH4max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH5min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH5max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH6min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH6max->setEnabled(false);
    main_window_ui_->checkBox_DH->setEnabled(true);


    Q_EMIT(on_2DOF());


    break;
    }
    case 3:
    {
    main_window_ui_->lineDH11->setEnabled(true);
    main_window_ui_->lineDH12->setEnabled(true);
    main_window_ui_->lineDH13->setEnabled(true);
    main_window_ui_->lineDH14->setEnabled(true);

    main_window_ui_->lineDH21->setEnabled(true);
    main_window_ui_->lineDH22->setEnabled(true);
    main_window_ui_->lineDH23->setEnabled(true);
    main_window_ui_->lineDH24->setEnabled(true);

    main_window_ui_->lineDH31->setEnabled(true);
    main_window_ui_->lineDH32->setEnabled(true);
    main_window_ui_->lineDH33->setEnabled(true);
    main_window_ui_->lineDH34->setEnabled(true);

    main_window_ui_->lineDH41->setEnabled(false);
    main_window_ui_->lineDH42->setEnabled(false);
    main_window_ui_->lineDH43->setEnabled(false);
    main_window_ui_->lineDH44->setEnabled(false);

    main_window_ui_->lineDH41->setText("0");
    main_window_ui_->lineDH42->setText("0");
    main_window_ui_->lineDH43->setText("0");
    main_window_ui_->lineDH44->setText("0");

    main_window_ui_->lineDH51->setEnabled(false);
    main_window_ui_->lineDH52->setEnabled(false);
    main_window_ui_->lineDH53->setEnabled(false);
    main_window_ui_->lineDH54->setEnabled(false);

    main_window_ui_->lineDH61->setEnabled(false);
    main_window_ui_->lineDH62->setEnabled(false);
    main_window_ui_->lineDH63->setEnabled(false);
    main_window_ui_->lineDH64->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH1min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH1max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH2min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH2max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH3min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH3max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH4min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH4max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH4min->setValue(0);
    main_window_ui_->doubleSpinBoxDH4max->setValue(0);

    main_window_ui_->doubleSpinBoxDH5min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH5max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH6min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH6max->setEnabled(false);
    main_window_ui_->checkBox_DH->setEnabled(true);


    Q_EMIT(on_3DOF());


    break;
    }


    case 4:
    {
    main_window_ui_->lineDH11->setEnabled(true);
    main_window_ui_->lineDH12->setEnabled(true);
    main_window_ui_->lineDH13->setEnabled(true);
    main_window_ui_->lineDH14->setEnabled(true);

    main_window_ui_->lineDH21->setEnabled(true);
    main_window_ui_->lineDH22->setEnabled(true);
    main_window_ui_->lineDH23->setEnabled(true);
    main_window_ui_->lineDH24->setEnabled(true);

    main_window_ui_->lineDH31->setEnabled(true);
    main_window_ui_->lineDH32->setEnabled(true);
    main_window_ui_->lineDH33->setEnabled(true);
    main_window_ui_->lineDH34->setEnabled(true);

    main_window_ui_->lineDH41->setEnabled(true);
    main_window_ui_->lineDH42->setEnabled(true);
    main_window_ui_->lineDH43->setEnabled(true);
    main_window_ui_->lineDH44->setEnabled(true);

    main_window_ui_->lineDH51->setEnabled(false);
    main_window_ui_->lineDH52->setEnabled(false);
    main_window_ui_->lineDH53->setEnabled(false);
    main_window_ui_->lineDH54->setEnabled(false);

    main_window_ui_->lineDH51->setText("0");
    main_window_ui_->lineDH52->setText("0");
    main_window_ui_->lineDH53->setText("0");
    main_window_ui_->lineDH54->setText("0");

    main_window_ui_->lineDH61->setEnabled(false);
    main_window_ui_->lineDH62->setEnabled(false);
    main_window_ui_->lineDH63->setEnabled(false);
    main_window_ui_->lineDH64->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH1min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH1max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH2min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH2max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH3min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH3max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH4min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH4max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH5min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH5max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH5min->setValue(0);
    main_window_ui_->doubleSpinBoxDH5max->setValue(0);

    main_window_ui_->doubleSpinBoxDH6min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH6max->setEnabled(false);
    main_window_ui_->checkBox_DH->setEnabled(true);


    Q_EMIT(on_4DOF());


    break;
    }


    case 5:
    {
    main_window_ui_->lineDH11->setEnabled(true);
    main_window_ui_->lineDH12->setEnabled(true);
    main_window_ui_->lineDH13->setEnabled(true);
    main_window_ui_->lineDH14->setEnabled(true);

    main_window_ui_->lineDH21->setEnabled(true);
    main_window_ui_->lineDH22->setEnabled(true);
    main_window_ui_->lineDH23->setEnabled(true);
    main_window_ui_->lineDH24->setEnabled(true);

    main_window_ui_->lineDH31->setEnabled(true);
    main_window_ui_->lineDH32->setEnabled(true);
    main_window_ui_->lineDH33->setEnabled(true);
    main_window_ui_->lineDH34->setEnabled(true);

    main_window_ui_->lineDH41->setEnabled(true);
    main_window_ui_->lineDH42->setEnabled(true);
    main_window_ui_->lineDH43->setEnabled(true);
    main_window_ui_->lineDH44->setEnabled(true);

    main_window_ui_->lineDH51->setEnabled(true);
    main_window_ui_->lineDH52->setEnabled(true);
    main_window_ui_->lineDH53->setEnabled(true);
    main_window_ui_->lineDH54->setEnabled(true);

    main_window_ui_->lineDH61->setEnabled(false);
    main_window_ui_->lineDH62->setEnabled(false);
    main_window_ui_->lineDH63->setEnabled(false);
    main_window_ui_->lineDH64->setEnabled(false);

    main_window_ui_->lineDH61->setText("0");
    main_window_ui_->lineDH62->setText("0");
    main_window_ui_->lineDH63->setText("0");
    main_window_ui_->lineDH64->setText("0");

    main_window_ui_->doubleSpinBoxDH1min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH1max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH2min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH2max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH3min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH3max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH4min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH4max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH5min->setEnabled(true);
    main_window_ui_->doubleSpinBoxDH5max->setEnabled(true);

    main_window_ui_->doubleSpinBoxDH6min->setEnabled(false);
    main_window_ui_->doubleSpinBoxDH6max->setEnabled(false);

    main_window_ui_->doubleSpinBoxDH6min->setValue(0);
    main_window_ui_->doubleSpinBoxDH6max->setValue(0);
    main_window_ui_->checkBox_DH->setEnabled(true);


    Q_EMIT(on_5DOF());


    break;
    }


    case 6:
{
  main_window_ui_->lineDH11->setEnabled(true);
  main_window_ui_->lineDH12->setEnabled(true);
  main_window_ui_->lineDH13->setEnabled(true);
  main_window_ui_->lineDH14->setEnabled(true);

  main_window_ui_->lineDH21->setEnabled(true);
  main_window_ui_->lineDH22->setEnabled(true);
  main_window_ui_->lineDH23->setEnabled(true);
  main_window_ui_->lineDH24->setEnabled(true);

  main_window_ui_->lineDH31->setEnabled(true);
  main_window_ui_->lineDH32->setEnabled(true);
  main_window_ui_->lineDH33->setEnabled(true);
  main_window_ui_->lineDH34->setEnabled(true);

  main_window_ui_->lineDH41->setEnabled(true);
  main_window_ui_->lineDH42->setEnabled(true);
  main_window_ui_->lineDH43->setEnabled(true);
  main_window_ui_->lineDH44->setEnabled(true);

  main_window_ui_->lineDH51->setEnabled(true);
  main_window_ui_->lineDH52->setEnabled(true);
  main_window_ui_->lineDH53->setEnabled(true);
  main_window_ui_->lineDH54->setEnabled(true);

  main_window_ui_->lineDH61->setEnabled(true);
  main_window_ui_->lineDH62->setEnabled(true);
  main_window_ui_->lineDH63->setEnabled(true);
  main_window_ui_->lineDH64->setEnabled(true);

  main_window_ui_->doubleSpinBoxDH1min->setEnabled(true);
  main_window_ui_->doubleSpinBoxDH1max->setEnabled(true);

  main_window_ui_->doubleSpinBoxDH2min->setEnabled(true);
  main_window_ui_->doubleSpinBoxDH2max->setEnabled(true);

  main_window_ui_->doubleSpinBoxDH3min->setEnabled(true);
  main_window_ui_->doubleSpinBoxDH3max->setEnabled(true);

  main_window_ui_->doubleSpinBoxDH4min->setEnabled(true);
  main_window_ui_->doubleSpinBoxDH4max->setEnabled(true);

  main_window_ui_->doubleSpinBoxDH5min->setEnabled(true);
  main_window_ui_->doubleSpinBoxDH5max->setEnabled(true);

  main_window_ui_->doubleSpinBoxDH6min->setEnabled(true);
  main_window_ui_->doubleSpinBoxDH6max->setEnabled(true);
  main_window_ui_->checkBox_DH->setEnabled(true);


    Q_EMIT(on_6DOF());

break;
}


  } //switch

}

void ROSGUI::on_pushButton_3_toggled()
{
  nh_.deleteParam("root_link");
  nh_.deleteParam("tip_link");
  nh_.setParam("root_link","mbase_link");
  nh_.setParam("tip_link","tool0");

  int data;
  data = main_window_ui_->spinBox->value();
  valueDH.data.resize(24);
  valueDH.data[0] =  std::stod(main_window_ui_->lineDH11->text().toStdString());
  valueDH.data[1] =  std::stod(main_window_ui_->lineDH12->text().toStdString());
  valueDH.data[2] =  std::stod(main_window_ui_->lineDH13->text().toStdString());
  valueDH.data[3] =  std::stod(main_window_ui_->lineDH14->text().toStdString());

  valueDH.data[4] =  std::stod(main_window_ui_->lineDH21->text().toStdString());
  valueDH.data[5] =  std::stod(main_window_ui_->lineDH22->text().toStdString());
  valueDH.data[6] =  std::stod(main_window_ui_->lineDH23->text().toStdString());
  valueDH.data[7] =  std::stod(main_window_ui_->lineDH24->text().toStdString());

  valueDH.data[8] =  std::stod(main_window_ui_->lineDH31->text().toStdString());
  valueDH.data[9]  =  std::stod(main_window_ui_->lineDH32->text().toStdString());
  valueDH.data[10] =  std::stod(main_window_ui_->lineDH33->text().toStdString());
  valueDH.data[11] =  std::stod(main_window_ui_->lineDH34->text().toStdString());

  valueDH.data[12] =  std::stod(main_window_ui_->lineDH41->text().toStdString());
  valueDH.data[13] =  std::stod(main_window_ui_->lineDH42->text().toStdString());
  valueDH.data[14] =  std::stod(main_window_ui_->lineDH43->text().toStdString());
  valueDH.data[15] =  std::stod(main_window_ui_->lineDH44->text().toStdString());

  valueDH.data[16] =  std::stod(main_window_ui_->lineDH51->text().toStdString());
  valueDH.data[17] =  std::stod(main_window_ui_->lineDH52->text().toStdString());
  valueDH.data[18] =  std::stod(main_window_ui_->lineDH53->text().toStdString());
  valueDH.data[19] =  std::stod(main_window_ui_->lineDH54->text().toStdString());

  valueDH.data[20] =  std::stod(main_window_ui_->lineDH61->text().toStdString());
  valueDH.data[21] =  std::stod(main_window_ui_->lineDH62->text().toStdString());
  valueDH.data[22] =  std::stod(main_window_ui_->lineDH63->text().toStdString());
  valueDH.data[23] =  std::stod(main_window_ui_->lineDH64->text().toStdString());
  std::cout <<  valueDH.data[10] << std::endl;
  int rot ;
  rot = main_window_ui_->comboBox_4->currentIndex();
  if(!jointsv->treeforDH(model,valueDH,rot))
  {
     std::cerr << "Error at model DH" <<std::endl;
  }

  std::cerr << "No de Joints" << nj_2 <<std::endl;
 


  main_window_ui_->dial1DOF->setMinimum(main_window_ui_->doubleSpinBoxDH1min->value());
  main_window_ui_->dial1DOF->setMaximum(main_window_ui_->doubleSpinBoxDH1max->value());
  main_window_ui_->dial1DOF->setSingleStep(1);
  main_window_ui_->spinBox1DOF->setMinimum(main_window_ui_->doubleSpinBoxDH1min->value());
  main_window_ui_->spinBox1DOF->setMaximum(main_window_ui_->doubleSpinBoxDH1max->value());
  main_window_ui_->spinBox1DOF->setSingleStep(1);

  main_window_ui_->dial2DOF->setMinimum(main_window_ui_->doubleSpinBoxDH2min->value());
  main_window_ui_->dial2DOF->setMaximum(main_window_ui_->doubleSpinBoxDH2max->value());
  main_window_ui_->dial2DOF->setSingleStep(1);
  main_window_ui_->spinBox2DOF->setMinimum(main_window_ui_->doubleSpinBoxDH2min->value());
  main_window_ui_->spinBox2DOF->setMaximum(main_window_ui_->doubleSpinBoxDH2max->value());
  main_window_ui_->spinBox2DOF->setSingleStep(1);

  main_window_ui_->dial3DOF->setMinimum(main_window_ui_->doubleSpinBoxDH3min->value());
  main_window_ui_->dial3DOF->setMaximum(main_window_ui_->doubleSpinBoxDH3max->value());
  main_window_ui_->dial3DOF->setSingleStep(1);
  main_window_ui_->spinBox3DOF->setMinimum(main_window_ui_->doubleSpinBoxDH3min->value());
  main_window_ui_->spinBox3DOF->setMaximum(main_window_ui_->doubleSpinBoxDH3max->value());
  main_window_ui_->spinBox3DOF->setSingleStep(1);

  main_window_ui_->dial4DOF->setMinimum(main_window_ui_->doubleSpinBoxDH4min->value());
  main_window_ui_->dial4DOF->setMaximum(main_window_ui_->doubleSpinBoxDH4max->value());
  main_window_ui_->dial4DOF->setSingleStep(1);
  main_window_ui_->spinBox4DOF->setMinimum(main_window_ui_->doubleSpinBoxDH4min->value());
  main_window_ui_->spinBox4DOF->setMaximum(main_window_ui_->doubleSpinBoxDH4max->value());
  main_window_ui_->spinBox4DOF->setSingleStep(1);

  main_window_ui_->dial5DOF->setMinimum(main_window_ui_->doubleSpinBoxDH5min->value());
  main_window_ui_->dial5DOF->setMaximum(main_window_ui_->doubleSpinBoxDH5max->value());
  main_window_ui_->dial5DOF->setSingleStep(1);
  main_window_ui_->spinBox5DOF->setMinimum(main_window_ui_->doubleSpinBoxDH5min->value());
  main_window_ui_->spinBox5DOF->setMaximum(main_window_ui_->doubleSpinBoxDH5max->value());
  main_window_ui_->spinBox5DOF->setSingleStep(1);

  main_window_ui_->dial6DOF->setMinimum(main_window_ui_->doubleSpinBoxDH6min->value());
  main_window_ui_->dial6DOF->setMaximum(main_window_ui_->doubleSpinBoxDH6max->value());
  main_window_ui_->dial6DOF->setSingleStep(1);
  main_window_ui_->spinBox6DOF->setMinimum(main_window_ui_->doubleSpinBoxDH6min->value());
  main_window_ui_->spinBox6DOF->setMaximum(main_window_ui_->doubleSpinBoxDH6max->value());
  main_window_ui_->spinBox6DOF->setSingleStep(1);

//  main_window_ui_->dial6DOF->setMinimum(joint_lower[5]*ToG);
//  main_window_ui_->dial6DOF->setMaximum(joint_upper[5]*ToG);
//  main_window_ui_->dial6DOF->setSingleStep(1);
//  main_window_ui_->spinBox6DOF->setMinimum(joint_lower[5]*ToG);
//  main_window_ui_->spinBox6DOF->setMaximum(joint_upper[5]*ToG);
//  main_window_ui_->spinBox6DOF->setSingleStep(1);



  this->updatetreeforDH(model);
  // if(!init())
  // {
  //    ROS_ERROR("Error publisher");
  // }


 if(!jointsv->initmodel(nj, DHb))
 {
          std::cerr << "Error at init model" <<std::endl;
         
 }
j(nj) =  0;
  this->FKdata(j);

//      joint_value_pub.publish(send_val);
  send_val.data.resize(12);
  send_val.data[0] = main_window_ui_->doubleSpinBoxDH1min->value();
  send_val.data[1] = main_window_ui_->doubleSpinBoxDH2min->value();
  send_val.data[2] = main_window_ui_->doubleSpinBoxDH3min->value();
  send_val.data[3] = main_window_ui_->doubleSpinBoxDH4min->value();
  send_val.data[4] = main_window_ui_->doubleSpinBoxDH5min->value();
  send_val.data[5] = main_window_ui_->doubleSpinBoxDH6min->value();

  send_val.data[6] = main_window_ui_->doubleSpinBoxDH1max->value();
  send_val.data[7] = main_window_ui_->doubleSpinBoxDH2max->value();
  send_val.data[8] = main_window_ui_->doubleSpinBoxDH3max->value();
  send_val.data[9] = main_window_ui_->doubleSpinBoxDH4max->value();
  send_val.data[10]= main_window_ui_->doubleSpinBoxDH5max->value();
  send_val.data[11]= main_window_ui_->doubleSpinBoxDH6max->value();

  joint_value_pub.publish(send_val);

}

void ROSGUI::mouseMoved(QMouseEvent *event) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);

    double x = main_window_ui_->graph_canvas->xAxis->pixelToCoord(event->x());
    double y = main_window_ui_->graph_canvas->yAxis->pixelToCoord(event->y());

    oss << "Graph values: x: " << x << "\ty: " << y ;
    main_window_ui_->label_182->setText(QString::fromStdString(oss.str()));
}
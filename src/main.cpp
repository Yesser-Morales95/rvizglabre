#include "rosgui.h"
//#include "include/ui_rosgui.h"
#include <ui_rosgui.h>
//#include "include/secondwindow.h"
//#include "include/thirdwindow.h"
#include <QProcess>
//#include "include/rvizg_node.h"
#include <QApplication>
#include <ros/ros.h>

int main(int argc, char **argv)
{
// QProcess proc;     //   <QtGui>
// proc.start("gnome-terminal --geometry=50x10-0-10 -x bash -c \"roscore\" ");
  if(!ros::isInitialized())
  {
    ros::init(argc, argv, "my_lab_uni", ros::init_options::AnonymousName);
//    ros::NodeHandle n;
//    ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
//    tf::TransformBroadcaster broadcaster;
//    ros::Rate loop_rate(30);
  }
   QApplication a(argc, argv);
   ROSGUI *rosgui = new ROSGUI;

    rosgui->show();
    return a.exec();
    delete rosgui;
}

#include "modelparam.h"
#include <ros/ros.h>


modelparam::modelparam()
{
//double readJntLimitsFromROSParamURDF();

}
bool modelparam::initmodel()
{

  std::string param_nameR = "robot_editor/robot_description";
  std::string full_param_name;
  std::string xml_string;
  //std::string xml_string2;

  std::string param_nameCr = "root_link";
  std::string full_param_nameCr;
  std::string xml_stringCr;
  std::string param_nameCt = "tip_link";
  std::string full_param_nameCt;
  std::string xml_stringCt;
                                 //Parametro Robot Description
    nh.searchParam(param_nameR, full_param_name);
      if (nh.hasParam(full_param_name)){nh.getParam(full_param_name.c_str(), xml_string);}

      else { ROS_ERROR("Parameter %s not set, shutting down node... in model param" , full_param_name.c_str());
             nh.shutdown();
             //return false;
      }

      if (xml_string.size() == 0)
      {      ROS_ERROR("Unable to load robot model from parameter in model param %s",full_param_name.c_str());
             nh.shutdown();
             //return false;
      }

      ROS_DEBUG("%s content\n%s", full_param_name.c_str(), xml_string.c_str());
     // xml_string2 =  "/home/yesser/ros_qtc_plugin/src/rvizglabre/modelos/sia10f.urdf";
                               //Parametro Root Link KDL CHAIN

      nh.searchParam(param_nameCr, full_param_nameCr);
          if (nh.hasParam(full_param_nameCr)){nh.getParam(full_param_nameCr.c_str(), xml_stringCr);}

          else { ROS_ERROR("Parameter %s not set, shutting down node... in model param" , full_param_nameCr.c_str());
                 nh.shutdown();
                 //return false;
          }

          if (xml_stringCr.size() == 0)
          {      ROS_ERROR("Unable to load robot model from parameter in model param %s",full_param_nameCr.c_str());
                 nh.shutdown();
                 //return false;
          }

          ROS_DEBUG("%s content\n%s", full_param_nameCr.c_str(), xml_stringCr.c_str());
                               //Parametro Tip Link KDL CHAIN
       nh.searchParam(param_nameCt, full_param_nameCt);
          if (nh.hasParam(full_param_nameCt)){nh.getParam(full_param_nameCt.c_str(), xml_stringCt);}

          else { ROS_ERROR("Parameter %s not set, shutting down node... in model param" , full_param_nameCt.c_str());
          nh.shutdown();
          //return false;
          }

          if (xml_stringCt.size() == 0)
          {      ROS_ERROR("Unable to load robot model from parameter in model param %s",full_param_nameCt.c_str());
          nh.shutdown();
          //return false;
          }

          ROS_DEBUG("%s content\n%s", full_param_nameCt.c_str(), xml_stringCt.c_str());

      /// Get urdf model out of robot_description
          if (!urdf_model.initString(xml_string))    //Si se abre desde una direccion de ruta de archivo .initFile
          {
              ROS_ERROR("Failed to parse urdf file in model param");
              nh.shutdown();
              //return false;
          }
          ROS_DEBUG("Successfully parsed urdf file");

      /// Get KDL TREE
          if (!kdl_parser::treeFromUrdfModel(urdf_model, kdl_tree)){
                 ROS_ERROR("Failed to construct kdl tree");
                 nh.shutdown();
              }

      /// Get KDL CHAIN
          if(!kdl_tree.getChain(xml_stringCr, xml_stringCt, kdl_chain))
              {
                  ROS_ERROR("Error getting KDL chain");
                   nh.shutdown();
              }
          return true;

}

bool modelparam::ForwardK(KDL::Vector &pos_mat, KDL::JntArray j)
{
  j1=j;
  if (!initmodel())
  {ROS_ERROR("Failed to parse urdf file in model param");
   }

  //  Obtaining FK

//  double positions[] =

//    {0, 0, 0, 0, 0, 0};
  unsigned int nj = 0;
  nj = kdl_chain.getNrOfJoints();
  ROS_INFO("[Segments,joints]:[%d]",nj);


  // fksolver = new KDL::ChainFkSolverPos_recursive(kdl_chain);
KDL::ChainFkSolverPos_recursive fksolver(kdl_chain);
KDL::JntArray q(nj);

for(unsigned int i = 0;i < nj;i++)
{
  q(i)=j1(i);
}



 double roll = 0.0 , pitch = 0.0 , yaw = 0.0;

 fksolver.JntToCart(q, result,nj);
 KDL::Rotation R;
   R = result.M;

   R.GetRPY(roll,pitch,yaw);



  pos_mat = result.p;

  return true;

}


bool modelparam::readJntLimitsFromROSParamURDF(std::vector<double> &lower_limits, std::vector<double> &upper_limits)

{

  if (!initmodel())
  {ROS_ERROR("Failed to parse urdf file in model param");
   }


  lower_limits.clear();
  upper_limits.clear();

  //KDL WRAPPER


          int nbr_segs = kdl_chain.getNrOfSegments();
         // lower_limits.resize(kdl_chain.getNrOfJoints());
              std::vector<std::string> seg_names;

              for(int i=0; i<nbr_segs; i++)
                  seg_names.push_back(kdl_chain.getSegment(i).getJoint().getName());

              for (std::map<std::string,boost::shared_ptr<urdf::Joint> >::iterator joint = urdf_model.joints_.begin(); joint != urdf_model.joints_.end(); ++joint) {
                  if ( joint->second->limits && std::find(seg_names.begin(), seg_names.end(),joint->second->name)!=seg_names.end() ) {
                      if (joint->second->limits->lower != joint->second->limits->upper) {
                         // limited_jnt_names.push_back(joint->second->name);
                          lower_limits.push_back(joint->second->limits->lower);
                          upper_limits.push_back(joint->second->limits->upper);
                }
               }
              }

           return true;
}

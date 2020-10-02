#pragma once
// ROS
#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PointStamped.h"
#include "nav_msgs/OccupancyGrid.h"
#include <iostream>
namespace ros_augmented_gridmaps {

class AugmentedGridMap
{
  public:
  AugmentedGridMap(ros::NodeHandle& nodeHandle);
  ~AugmentedGridMap();
  
  private:
  // ROS stuff
  ros::NodeHandle& nodeHandle_;
  ros::Subscriber mapSubscriber;
  ros::Subscriber pointSubscriber;
  ros::Publisher augmented_map_pub;
  ros::Publisher augmented_metadata_pub;
  ros::Subscriber obstacleSubscriber;
  
  void saveMap(const nav_msgs::OccupancyGrid &map);   
  void addPointCallback(const geometry_msgs::PointStamped &punto);   
  void addObstacleToMap(geometry_msgs::PointStamped added_point);

  std::vector<geometry_msgs::Point> obstacles;  
  float obstacle_radius;
  nav_msgs::OccupancyGrid original_map;
  nav_msgs::MapMetaData map_metadata;
  nav_msgs::OccupancyGrid enhanced_map;
};
}

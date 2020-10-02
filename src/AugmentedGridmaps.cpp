#include "augmented_gridmaps/AugmentedGridMap.hpp"

namespace ros_augmented_gridmaps{

AugmentedGridMap::AugmentedGridMap(ros::NodeHandle &nodeHandle)
  :nodeHandle_(nodeHandle)
{                     
  mapSubscriber = nodeHandle_.subscribe("map",1,&AugmentedGridMap::saveMap,this);
  pointSubscriber = nodeHandle_.subscribe("point_obstacle",1,&AugmentedGridMap::addPointCallback,this);
  // Latched publisher for data
  augmented_map_pub = nodeHandle.advertise<nav_msgs::OccupancyGrid>("augmented_map", 1, true);
  augmented_map_pub.publish( enhanced_map );
  
  augmented_metadata_pub = nodeHandle.advertise<nav_msgs::MapMetaData>("augmented_map_metadata", 1, true);
  augmented_metadata_pub.publish( map_metadata );

  nodeHandle_.param<float>("obstacle_radius",obstacle_radius,0.05);
  ROS_INFO("Map enhancer node Initialization finished");
  return;
}

AugmentedGridMap::~AugmentedGridMap()
{
}

void AugmentedGridMap::saveMap(const nav_msgs::OccupancyGrid &map)
{
  original_map.data = map.data;
  original_map.info = map.info;
  original_map.header.frame_id = map.header.frame_id;
  original_map.header.stamp = map.header.stamp;

  enhanced_map.data = map.data;
  enhanced_map.info = map.info;
  enhanced_map.header.frame_id = map.header.frame_id;
  enhanced_map.header.stamp = ros::Time();;
 
  map_metadata = map.info; 
  
  ROS_INFO("Got a map of: [%d,%d] @ %f resolution", enhanced_map.info.width, enhanced_map.info.height, enhanced_map.info.resolution);
  augmented_map_pub.publish( enhanced_map );
  augmented_metadata_pub.publish( map_metadata );
  return;
}

void AugmentedGridMap::addPointCallback(const geometry_msgs::PointStamped &added_point)
{
  // Gets a point message and adds it ass an obstacle to the gridmap.

  // Checks if original map has data
  if (original_map.data.size() < 1)
  {
    ROS_ERROR("Do not have original map to enhance yet");
    return;
  }

  std::string point_frame = added_point.header.frame_id;
  if ( point_frame.compare(original_map.header.frame_id) != 0)
  {
    ROS_ERROR("Point and map are on different frames!!!");
    ROS_ERROR("Point frame: \t %s", point_frame.c_str());
    ROS_ERROR("Map frame: \t %s", original_map.header.frame_id.c_str());
    return;
  }
  
  ROS_INFO("Adding point on: [%f,%f]",added_point.point.x,
            added_point.point.y );
  addObstacleToMap(added_point);

  augmented_map_pub.publish( enhanced_map );
}

void AugmentedGridMap::addObstacleToMap(geometry_msgs::PointStamped added_point)
{
  //Convert point to cell coordinates
  float x_coord = added_point.point.x;
  float y_coord = added_point.point.y;
 
  float x_orig = map_metadata.origin.position.x; 
  float y_orig = map_metadata.origin.position.y; 

  float resolution = map_metadata.resolution;

  int cell_x = (x_coord - x_orig )/resolution;
  int cell_y = (y_coord - y_orig )/resolution;
 
  std::cout<<"Cell:[" << cell_x << "," << cell_y <<"]\n";
 
  if ( (cell_x > map_metadata.width ) || (cell_x < 0))
  {
    ROS_ERROR("Point falls x coordinate outside map");
    return;
  }
  
  if ( (cell_y > map_metadata.height ) || (cell_y < 0))
  {
    ROS_ERROR("Point falls y coordinate outside map");
    return; 
  }

  obstacles.push_back(added_point.point);
 
  float min_x = cell_x - obstacle_radius/resolution;
  if (min_x < 0 )
  {                   
    min_x = 0;
  }
  float max_x = cell_x + obstacle_radius/resolution;
  if (max_x > map_metadata.width)
  {
    max_x = map_metadata.width;
  }

  float min_y = cell_y - obstacle_radius/resolution;
  if (min_y < 0 )
  {                   
    min_y = 0;
  }
  float max_y = cell_y + obstacle_radius/resolution;
  if (max_y > map_metadata.height)
  {
    max_y = map_metadata.height;
  }

  std::cout<<"Max:[" << max_x << "," << max_y <<"]\n";
  std::cout<<"Min:[" << min_x << "," << min_y <<"]\n";
  for (int i = min_x; i < max_x ; i++)
  {
    for (int j = min_y; j < max_y ; j++)
    {
      enhanced_map.data[i+j*map_metadata.width] = 100;
    }
  }
  
  return;

}


}

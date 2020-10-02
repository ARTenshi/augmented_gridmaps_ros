# Augmented gridmaps for ROS.

For most robot mapping applications ROS utilizes the well known OccupancyGrid.
Simple way to define arbitrary geometry maps with finite precision.  However
this format is really akward if you want or need to use for something else.
Specifically dynamic updates to the map.  That is by desing, ros map_server was
created to provide a well stablished static map to other nodes. Not to modify
it at run-time. In occassions it would be helpful to modify occupancy grids for
the long run, for example if an unmapped obstacle has appeared on run-time. 

This small package was created to solve that issue, it allows to add, or
augment obstacles to the map with obstacles on runtime and re-publish for the
enhanced to the rest of interested nodes. This can be helpfull when these new
obstacles are not gonna leave the robot workspace and you want you planner to
avoid trying to get through them

## Dependencies

This package was created with simplicity in mind. It only depends on core ROS
libraries and messages so as long as you have a working ROS install it should
work.  This should also be quite compatible between ros versions, but was only
tested on ros-melodic.

##  Usage

This package only has one node: augmented_gridmaps_node. It reads from `/map`
and publishes to `/augmented_map`.

For now it only receives point-type obstacles or a given radius. These need to
be published to `/point_obstacle` topic as PointStamped geometry msgs.

To run a simple demo you can use the provide launch file:

```bash

 roslaunch augmented_gridmaps augmented_gridmaps.launch 

``` 

Thi is requires ros map server to run. From there you can use the `Publish point`
button on rviz to dynamically add obstacles to the map. 

## Nodes

TODO

### augmented_gridmaps_node.

Topics

Parameters






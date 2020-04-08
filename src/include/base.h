#include <ros/ros.h>
#include <string>
#include <vector>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>

class Base{
public:
    void run(int argc, char** argv);
    typedef geometry_msgs::Pose Pose;

private:
    const unsigned int MAX_INT = 3;

    virtual std::vector<Pose> path() = 0;

    void log_position(const Pose&);

    bool move_to_goal(const Pose&);
};

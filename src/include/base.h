#include <string>
#include <vector>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>

class Base{
public:
    typedef geometry_msgs::Pose Pose;

    void run(int argc, char** argv);

private:
    const unsigned int MAX_INT = 3;

    virtual std::vector<Pose> path() = 0;

    void log_position(const Pose&);

    bool move_to_goal(const Pose&);
};

// #include <ros/ros.h>
#include <string>
#include <vector>
// #include <move_base_msgs/MoveBaseAction.h>
// #include <actionlib/client/simple_action_client.h>

class Base{
    const unsigned int MAX_INT = 3;

    virtual std::vector<Pose> path();

    void log_position(Pose);

    bool move_to_goal(Pose);

public:
    void run(int argc, char** argv);
};
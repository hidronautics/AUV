#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include <sstream>
#include <gate/GateDescriptor.h>
#include <gate/GateDetector.h>
#include <util/ImgprocUtil.h>
#include "common/AbstractImageConverter.h"
#include "auv_common/OptionalPoint2D.h"

static const std::string OPENCV_WINDOW = "Image window";

static const std::string ENABLE_WINDOWS_PARAM = "debugVision";

static const std::string CAMERA_TOPIC = "/cam_front_1/image_raw";

static const std::string GATE_PUBLISH_TOPIC = "/gate";

static const std::string GATE_LOCATOR_NODE_NAME = "gate_locator";

class GatePublisher : public AbstractImageConverter {

private:

    ros::Publisher gatePublisher;

    GateDetector detector;

    bool windowsEnabled;

protected:

    void process(const cv_bridge::CvImagePtr &cv_ptr) {
        cv::Mat image = cv_ptr->image;

        GateDescriptor gate = detector.detect(image, true);

        auv_common::OptionalPoint2D msg;

        if (gate.hasGate()) {
            cv::Point2f center = convertToCentralCoordinates(gate.getCenter(), image.cols, image.rows);

            msg.hasPoint = true;
            msg.x = center.x;
            msg.y = center.y;
            gatePublisher.publish(msg);

            std::vector<cv::Point2f> corners = gate.getCorners();
            cv::circle(image, corners[0], 10, CV_RGB(0, 255, 0));
            cv::circle(image, corners[1], 10, CV_RGB(0, 255, 0));
            cv::circle(image, corners[2], 10, CV_RGB(0, 255, 0));
            cv::circle(image, corners[3], 10, CV_RGB(0, 255, 0));
            cv::circle(image, gate.getCenter(), 10, CV_RGB(0, 0, 255));
        } else {
            msg.hasPoint = false;
            msg.x = 0;
            msg.y = 0;
            gatePublisher.publish(msg);
        }

        if (windowsEnabled) {
            cv::imshow(OPENCV_WINDOW, image);
            cv::waitKey(3);
        }
    }

public:

    GatePublisher(const std::string &inputImageTopic, bool enableWindows) : AbstractImageConverter(inputImageTopic),
                                                                            windowsEnabled(enableWindows) {
        gatePublisher = nodeHandle.advertise<auv_common::OptionalPoint2D>(GATE_PUBLISH_TOPIC, 100);
        if (windowsEnabled)
            cv::namedWindow(OPENCV_WINDOW, CV_WINDOW_AUTOSIZE);
    }

    ~GatePublisher() {
        if (windowsEnabled)
            cv::destroyWindow(OPENCV_WINDOW);
    }

};


/* TODO Fix code style */
int main(int argc, char **argv)
{

#if CV_MAJOR_VERSION == 2
    ROS_INFO("%s", "OpenCV version = 2");
#elif CV_MAJOR_VERSION == 3
    ROS_INFO("%s", "OpenCV version = 3");
#endif

    ros::init(argc, argv, GATE_LOCATOR_NODE_NAME);

    ros::NodeHandle nodeHandle(GATE_LOCATOR_NODE_NAME);
    bool windowsEnabled;
    nodeHandle.param(ENABLE_WINDOWS_PARAM, windowsEnabled, false);

    GatePublisher gatePublisher(CAMERA_TOPIC, windowsEnabled);

    ros::spin();

    return 0;
}
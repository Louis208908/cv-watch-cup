#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <std_msgs/String.h>

static const std::string OPENCV_WINDOW = "Image window";


class ImageConverter{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;

public:
    ros::Publisher pub;
    std_msgs::String str;
    int xPosition;
    int yPosition;
    int CupDistance;

    ImageConverter(): 
    it_(nh_)  //assign nh_ to it_
    {
    // Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("/camera/color/image_raw", 1, &ImageConverter::imageCb, this);
        image_pub_ = it_.advertise("/image_converter/output_video", 1);
        pub = nh_.advertise<std_msgs::String>("cup", 1);
        cv::namedWindow(OPENCV_WINDOW);
    }

    ~ImageConverter(){
        cv::destroyWindow(OPENCV_WINDOW);
    }


    void imageCb(const sensor_msgs::ImageConstPtr &msg) {
        cv_bridge::CvImagePtr cv_ptr;
        try{
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception &e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        std::stringstream strstr;
        int width = 1280;
        int height = 720;
        xPosition = 0;
        yPosition = 0;
        CupDistance = 30;
        int circleSize = 20;
        nh_.param<int>("XPose", xPosition, 300);
        // nh_.getParam("/XPose", xPosition);
        nh_.param<int>("YPose", yPosition, 300);
        nh_.param<int>("cupDistance", CupDistance, 30);
        nh_.param<int>("circleSize", circleSize, 20);
        // nh_.getParam("/YPose", yPosition);

        // printf("%d %d",xPosition,yPosition);
        
        // nh_.getParam("/cupDistance",CupDistance);
        
        int r = 0;
        int g = 0;
        int b = 0;
        int RedCount = 0;
        int GreenCount = 0;
        int XTemp;
        int YTemp;
        XTemp = xPosition;
        YTemp = yPosition;
        for(int qq = 0; qq < 5; qq ++){
            cv::circle(cv_ptr->image, cv::Point(XTemp, YTemp), circleSize, CV_RGB(0, 255, 0));
            XTemp += CupDistance;
        }
        
        for(int count = -circleSize / 2; count < circleSize / 2; count += 1){
             r = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3];
             g = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3 + 1];
             if (r > g)
                 RedCount++;
             else if (g > r)
                 GreenCount++;
        }
        if (RedCount > GreenCount)
            strstr << '1';
        else
            strstr << '0';
        RedCount = 0;
        GreenCount = 0;
        xPosition += CupDistance;
        for (int count = -circleSize / 2; count < circleSize / 2; count += 1){
            r = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3];
            g = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3 + 1];
            if (r > g)
                RedCount++;
            else if (g > r)
                GreenCount++;
        }
        //b = msg->data[xPosition * 3 + (yPosition-1) * width * 3 + 2];
        if (RedCount > GreenCount)
            strstr << '1';
        else
            strstr << '0';
        RedCount = 0;
        GreenCount = 0;
        xPosition += CupDistance;
        for (int count = -circleSize / 2; count < circleSize / 2; count += 1){
            r = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3];
            g = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3 + 1];
            if (r > g)
                RedCount++;
            else if (g > r)
                GreenCount++;
        }
        //b = msg->data[xPosition * 3 + (yPosition-1) * width * 3 + 2];
        if (RedCount > GreenCount)
            strstr << '1';
        else
            strstr << '0';
        RedCount = 0;
        GreenCount = 0;
        xPosition += CupDistance;
        for (int count = -circleSize / 2; count < circleSize / 2; count += 1){
            r = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3];
            g = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3 + 1];
            if (r > g)
                RedCount++;
            else if (g > r)
                GreenCount++;
        }
        //b = msg->data[xPosition * 3 + (yPosition-1) * width * 3 + 2];
        if (RedCount > GreenCount)
            strstr << '1';
        else
            strstr << '0';
        RedCount = 0;
        GreenCount = 0;
        xPosition += CupDistance;
        for (int count = -circleSize / 2; count < circleSize / 2; count += 1){
            r = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3];
            g = msg->data[(xPosition + count) * 3 + (yPosition - 1) * width * 3 + 1];
            if (r > g)
                RedCount++;
            else if (g > r)
                GreenCount++;
        }
        //b = msg->data[xPosition * 3 + (yPosition-1) * width * 3 + 2];
        if (RedCount > GreenCount)
            strstr << '1';
        else
            strstr << '0';
        RedCount = 0;
        GreenCount = 0;
        xPosition += CupDistance;
        ImageConverter::str.data = strstr.str();
        pub.publish(str);
        
        cv::imshow(OPENCV_WINDOW, cv_ptr->image);
        cv::waitKey(3);

        image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char **argv){
  ros::init(argc, argv, "cupDetectWithFunction");
  ImageConverter ic;
  ros::spin();
  return 0;
}

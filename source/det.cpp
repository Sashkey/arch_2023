#define _USE_MATH_DEFINES

#include <opencv2/opencv.hpp>
#include <vector>
#include <math.h>
#include "det.h"

using namespace std;
using namespace cv;


Detector::Detector(int id) {
    cap.open(id);
    if (!cap.isOpened())
        cout << "Error opening video stream or file" << endl;
    update_image();
}

Point Detector::find_rect_centre(Rect r) {
    int x = r.x + (r.width / 2);
    int y = r.y + (r.height / 2);
    return Point(x, y);
}
Point Detector::find_centre_beetwen_point(Point p1, Point p2) {
    int x = (p1.x + p2.x) / 2;
    int y = (p1.y + p2.y) / 2;
    return Point(x, y);
}

int Detector::angle_beetwen_point(Point p1, Point p2) {
    double hypotenuse = pow((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y), 2);
    asin(hypotenuse / (p2.x - p1.x));
    double deg = atan2((p2.x - p1.x), (p2.y - p1.y)) / M_PI * 180;
    if (deg >= 0)
        return deg;
    else
        return 360 + deg;
}

int Detector::distance_beetwen_point(Point p1, Point p2) {
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

void Detector::update_angle_bot() {
    Point p1 = find_rect_centre(rect_bot_f);
    Point p2 = find_rect_centre(rect_bot_r);
    centre_bot = find_centre_beetwen_point(p1, p2);
    angle_bot = angle_beetwen_point(p2, p1);
}

void Detector::update_angle_to_target() {
    Point point_target = find_rect_centre(rect_target);
    int angle_bot_centre_to_target = angle_beetwen_point(centre_bot, point_target);
    angle_target = angle_bot_centre_to_target - angle_bot;
}

void Detector::update_distance_to_target() {
    Point point_target = find_rect_centre(rect_target);
    distance_target = distance_beetwen_point(centre_bot, point_target);
}



Mat Detector::get_image() {
    return frame_res;
}

void Detector::update_image() {
    cap >> frame;
    frame_res = frame;
    rect_bot_f = det_rect(color_bot_f);
    rect_bot_r = det_rect(color_bot_r);
    rect_target = det_rect(color_target);
    update_angle_bot();
    update_angle_to_target();
    update_distance_to_target();
}

int Detector::get_angle_to_target() {
    return angle_target;
}
int Detector::get_angle_bot() {
    return angle_bot;
}
int Detector::get_distance_to_target() {
    return distance_target;
}

bool Detector::delta_angle_to_target(int delta) {
    if (abs(angle_target) > delta)
        return true;
    else
        return false;
}
bool Detector::delta_distance_to_target(int delta) {
    if (distance_target > delta)
        return true;
    else
        return false;
}

void Detector::set_color_bot_f(int h, int s, int v) {
    color_bot_f.h = h;
    color_bot_f.s = s;
    color_bot_f.v = v;
}
void Detector::set_color_bot_r(int h, int s, int v) {
    color_bot_r.h = h;
    color_bot_r.s = s;
    color_bot_r.v = v;
}
void Detector::set_color_target(int h, int s, int v) {
    color_target.h = h;
    color_target.s = s;
    color_target.v = v;
}

void Detector::draw_rect_bot_f(int frame_r, int frame_g, int frame_b) {
    rectangle(frame_res, rect_bot_f, Scalar(frame_r, frame_g, frame_b), 2);
}
void Detector::draw_rect_bot_r(int frame_r, int frame_g, int frame_b) {
    rectangle(frame_res, rect_bot_r, Scalar(frame_r, frame_g, frame_b), 2);
}
void Detector::draw_rect_target(int frame_r, int frame_g, int frame_b) {
    rectangle(frame_res, rect_target, Scalar(frame_r, frame_g, frame_b), 2);
}

void Detector::draw_image() {
    draw_rect_bot_f(0, 255, 0);
    draw_rect_bot_r(255, 0, 0);
    draw_rect_target(0, 0, 255);
    Mat res_image = get_image();
    Point text_position(20, 30);
    int font_size = 1;
    Scalar font_Color(90, 100, 0);
    int font_weight = 2;
    putText(res_image, to_string(get_angle_to_target()), text_position, FONT_HERSHEY_COMPLEX_SMALL, font_size, font_Color, font_weight);
    text_position.x = 20;
    text_position.y = 60;
    putText(res_image, to_string(get_distance_to_target()), text_position, FONT_HERSHEY_COMPLEX_SMALL, font_size, font_Color, font_weight);

    imshow("tmp", res_image);
}

Rect Detector::det_rect(hsv color) {
    Mat image_hsv;
    vector<vector<Point>> contours;
    cvtColor(frame, image_hsv, COLOR_BGR2HSV);
    Mat tmp_img(frame.size(), CV_8U);
    inRange(image_hsv, Scalar(color.h - 5, color.s - 25, color.v - 35), Scalar(color.h + 5, color.s + 25, color.v + 35), tmp_img);
    dilate(tmp_img, tmp_img, Mat(), Point(-1, -1), 3);
    erode(tmp_img, tmp_img, Mat(), Point(-1, -1), 1);
    findContours(tmp_img, contours, 0, 1);

    Rect max_rect(0, 0, 0, 0);
    for (int i = 0; i < contours.size(); i++) {
        Mat sticker;
        Rect rect = boundingRect(contours[i]);
        if (rect.height * rect.width > max_rect.height * max_rect.width) {
            max_rect = rect;
        }
    }
    return max_rect;
}

Detector::~Detector() {
    cap.release();
    destroyAllWindows();
}
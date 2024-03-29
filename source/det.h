#pragma once
#include <opencv2/opencv.hpp>


using namespace cv;

struct hsv {
    int h;
    int s;
    int v;
    hsv(int _h, int _s, int _v) : h(_h), s(_s), v(_v) {}
};

class Detector {
    Mat frame;
    Mat frame_res;
    VideoCapture cap;

    Rect rect_bot_f;
    Rect rect_bot_r;
    Rect rect_target;

    hsv color_bot_f = hsv(0, 0, 0);
    hsv color_bot_r = hsv(0, 0, 0);
    hsv color_target = hsv(0, 0, 0);

    Point centre_bot;

    int angle_bot;
    int angle_target;
    int distance_target;

    Point find_rect_centre(Rect r);
    Point find_centre_beetwen_point(Point p1, Point p2);
    int angle_beetwen_point(Point p1, Point p2);
    int distance_beetwen_point(Point p1, Point p2);

    void update_angle_bot();
    void update_angle_to_target();
    void update_distance_to_target();

    Rect det_rect(hsv color);

public:

    Detector(int id);
    Detector(Detector& d);
    Detector& operator=(Detector& d);

    void set_color_bot_f(int h, int s, int v);
    void set_color_bot_r(int h, int s, int v);
    void set_color_target(int h, int s, int v);

    Mat get_image();
    void update_image();
    void draw_image();

    int get_angle_bot();
    int get_angle_to_target();
    int get_distance_to_target();

    bool delta_angle_to_target(int delta);
    bool delta_distance_to_target(int delta);

    void draw_rect_bot_f(int frame_r, int frame_g, int frame_b);
    void draw_rect_bot_r(int frame_r, int frame_g, int frame_b);
    void draw_rect_target(int frame_r, int frame_g, int frame_b);

    ~Detector();
};
#include <memory>
#include "det.h"
#include "dispenser.h"
#include "robot.h"

using namespace std;
using namespace cv;

int main() {
	shared_ptr<Detector> det(new Detector(0));
	det->set_color_bot_f(34, 65, 255);
	det->set_color_bot_r(176, 130, 170);
	det->set_color_target(90, 249, 68);
}
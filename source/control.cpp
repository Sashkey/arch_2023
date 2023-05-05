#include "control.h"
#include "dispenser.h"
#include "det.h"

void Control::processEvents() {
	switch (state) {
	case EWait:
		if (int order_exist = 1) {
			state = EPouring;
		}
		break;
	case EPouring:
		if (flag_pouring) {
			if (disp->drink_ready()) {
				flag_pouring = false;
				state = EMoveToTarget;
			}
		}
		break;
	case EMoveToTarget:
		if (det->delta_angle_to_target(10)) {
			if (det->get_angle_to_target() > 0)
				state = ERotateLeft;
			else
				state = ERotateRight;
			break;
		}

		if (det->delta_distance_to_target(160))
			state = EForward;
		break;
	case ERotateLeft:
		robot->left();
		state = EMoveToTarget;
		break;
	case ERotateRight:
		robot->right();
		state = EMoveToTarget;
		break;
	case EForward:
		robot->right();
		state = EMoveToTarget;
		break;
	}
}
void Control::run() {
	while (1) {
		det->update_image();
		det->draw_image();
		processEvents();
		waitKey(0);
		break;
	}
}
Control::Control(std::shared_ptr<Detector> det, std::shared_ptr<Robot> robot, std::shared_ptr<Dispenser> disp) : det(det), robot(robot), disp(disp) {
	this->state = EWait;
}
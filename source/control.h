#pragma once

#include "det.h"
#include "dispenser.h"
#include "robot.h"

enum EState { EWait, EMoveToTarget, ERotateLeft, ERotateRight, EForward, EPouring};

class Control {
	std::shared_ptr<Detector> det;
	std::shared_ptr<Robot> robot;
	std::shared_ptr<Dispenser> disp;

	EState state;

	bool flag_pouring = false;

	void processEvents();

public:
	Control(std::shared_ptr<Detector> det, std::shared_ptr<Robot> robot, std::shared_ptr<Dispenser> disp);
	void run();
};
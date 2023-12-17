#include "button.hpp"

int buttonState(float winwidth, float winheight, float aX, float aY, bool split) {

	int res = 0;

	if (!split) {

		float xP = aX / winwidth;
		float yP = aY / winheight;

		if (xP >= 0.35f && xP <= 0.47f && yP >= 0.858f && yP <= 0.936f) {
			res = 1;
		}
		if (xP >= 0.525f && xP <= 0.648f && yP >= 0.858f && yP <= 0.936f) {
			res = 2;
		}

	}
	else {
		
		float xP = aX / (winwidth / 2.f);
		float yP = aY / (winheight);

		float xP2 = (aX - winwidth / 2.f) / (winwidth / 2.f);


		if (xP >= 0.35f && xP <= 0.47f && yP >= 0.858f && yP <= 0.936f) {
			res = 1;
		}
		if (xP >= 0.525f && xP <= 0.648f && yP >= 0.858f && yP <= 0.936f) {
			res = 2;
		}

		if (xP2 >= 0.35f && xP2 <= 0.47f && yP >= 0.858f && yP <= 0.936f) {
			res = 1;
		}
		if (xP2 >= 0.525f && xP2 <= 0.648f && yP >= 0.858f && yP <= 0.936f) {
			res = 2;
		}

	}


	return res;

}
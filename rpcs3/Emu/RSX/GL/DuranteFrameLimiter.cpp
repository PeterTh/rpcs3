#include "DuranteFrameLimiter.h"

#include <Windows.h>

DuranteFrameLimiter& DuranteFrameLimiter::get() {
	if(!instance) instance = new DuranteFrameLimiter();
	return *instance;
}

void DuranteFrameLimiter::limit() {
	if(true) {
		double targetTime = 1.0 / targetFPS;
		double remTime = targetTime - (timeStamp() - lastExit);

		if(remTime > SLEEP_THRESHOLD) {
			Sleep((DWORD)(1000.0 * (remTime - SLEEP_THRESHOLD)));
		}
		do {
			remTime = targetTime - (timeStamp() - lastExit);
		} while(remTime > FUDGE_FACTOR);
	}

	double frameTime = timeStamp() - lastExit;
	previousFrametimes[curFtIdx++] = frameTime;
	if(curFtIdx == DURANTE_NUM_FRAMES_AVG) {
		curFtIdx = 0;
		//PHYRE_PRINTF("FPS: %10.6f\n", getAvgFPS());
	}
	double frameTimeSum = 0.0;
	for(int i = 0; i < DURANTE_NUM_FRAMES_AVG; ++i) frameTimeSum += previousFrametimes[i];
	frameTimeAvg = frameTimeSum / DURANTE_NUM_FRAMES_AVG;

	lastExit = timeStamp();
}

void DuranteFrameLimiter::setLimit(double targetFPS) {
	targetFPS = targetFPS;
}

void DuranteFrameLimiter::toggleLimit() {
	if(targetFPS == 60.0) targetFPS = 30.0;
	else targetFPS = 60.0;
}

double DuranteFrameLimiter::getAvgFPS() const {
	return 1.0 / frameTimeAvg;
}

DuranteFrameLimiter* DuranteFrameLimiter::instance = nullptr;

const double DuranteFrameLimiter::FUDGE_FACTOR =   0.01 / 1000.0; // 0.01 milliseconds
const double DuranteFrameLimiter::SLEEP_THRESHOLD = 2.0 / 1000.0; // 2 milliseconds


DuranteFrameLimiter::DuranteFrameLimiter()
	: curFtIdx(0)
	, frameTimeAvg(16.6)
	, targetFPS(60.0)
{
	for(int i = 0; i < DURANTE_NUM_FRAMES_AVG; ++i) previousFrametimes[i] = frameTimeAvg;

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	perfFreq = (double)freq.QuadPart;
	lastExit = 0.0;
}

double DuranteFrameLimiter::timeStamp() const {
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	return (double)ticks.QuadPart / perfFreq;
}
#pragma once

#define DURANTE_FRAME_LIMITER

#define DURANTE_NUM_FRAMES_AVG 60

class DuranteFrameLimiter {
public:
	static DuranteFrameLimiter& get();

	void limit();

	void setLimit(double limit);
	void toggleLimit();

	double getAvgFPS() const;

private:
	static DuranteFrameLimiter* instance;
	DuranteFrameLimiter();

	double timeStamp() const;

	double targetFPS;

	double perfFreq;
	double lastExit;

	const static double FUDGE_FACTOR;
	const static double SLEEP_THRESHOLD;

	double previousFrametimes[DURANTE_NUM_FRAMES_AVG];
	unsigned curFtIdx;
	double frameTimeAvg;
};
#pragma once

class Random {
public:
	Random();
	virtual ~Random();
	int nextInt(int);
	float nextFloat();
	double nextGaussian();
	bool nextBoolean();
};

#include "cornerPoint.h"

cornerPoint::cornerPoint()
{
	float x = 0;
	float y = 0;
	int leftpick = 0;
	int rightpick = 0;
}

cornerPoint::cornerPoint(float xtmp, float ytmp, int l, int r)
{
	float x = xtmp;
	float y = ytmp;
	int leftpick = l;
	int rightpick = r;
}
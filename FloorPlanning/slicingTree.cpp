
#include "slicingTree.h"

slicingTree::slicingTree(string nodeName, int setNum, double cella, double cellr, double cells)
{
	name = nodeName;
	set = setNum;
	a = cella;
	r = cellr;
	s = cells;
	cellx = 0;// this cell's x position
	celly = 0;// this cell's y position
	cellw = 0;// this cell's width
	cellh = 0;// this cell's height
	LorR = -1;
	if( name != "*" && name != "+")
	{
		leaf = true;// it's a leaf
	}
	else
		leaf = false;
	left = NULL;
	right = NULL;
	parent = NULL;
	if(leaf)
	{
		calculateBoundingCurve();// calculate this leaf's bounding curve
	}
}

void slicingTree::calculateBoundingCurve()
{
	if(set == 1)
		forSet1();
	else
		forSet2();
}

void slicingTree::forSet1()
{
	double hr, wr, hs, ws; // height and width
	hr = sqrt(a*r);
	wr = sqrt(a/r);
	hs = sqrt(a*s);
	ws = sqrt(a/s);
	int col = 2;
	if(fabs(r - s) < same)// consider it is a certain shape cell
	{
		cornerNum = 1;// only one corner

		vector<double> cornerpt;
		cornerpt.push_back(wr);
		cornerpt.push_back(hr);
		bc.push_back(cornerpt);

	}
	else// it can be rotated
	{
		double w1 = min(wr, ws);// min width
		double w2 = max(wr, ws);// max width
		cornerNum = 1 + static_cast<int>( (w2-w1) /gap ); // calculate the number of the linear corners 
		for(int i = 0; i<cornerNum-1; i++)//  for each new corner points and r point
		{
			double w = w1 + i * gap;
			vector<double> cornerpt;// new point
			cornerpt.push_back(w);
			cornerpt.push_back(a / w);
			bc.push_back(cornerpt);
		}
		vector<double> cornerpt;
		cornerpt.push_back(w2);
		cornerpt.push_back(a / w2);
		bc.push_back(cornerpt);
	}
}

void slicingTree::forSet2()
{
	double hr, wr, hs, ws; // height and width
	hr = sqrt(a*r);
	wr = sqrt(a/r);
	hs = sqrt(a*s);
	ws = sqrt(a/s);
	int col = 2;
	if(fabs(r - s) < same)// consider it is a fixed shape cell
	{
		cornerNum = 2;// two corners, it can be rotated

		vector<double> cornerpt1;
		cornerpt1.push_back(wr);
		cornerpt1.push_back(hr);
		bc.push_back(cornerpt1);

		vector<double> cornerpt2;
		cornerpt2.push_back(hr);
		cornerpt2.push_back(wr);
		bc.push_back(cornerpt2);

	}
	else// it is free orientation
	{
		double w1 = min(min(hr,hs),min(wr,ws));// min width
		double w2 = max(max(hr,hs),max(wr,ws));// max width
		cornerNum = 1 + static_cast<int>( (w2-w1) /gap ); // calculate the number of the linear corners 
		for(int i = 0; i<cornerNum-1; i++)//  for each new corner points and r point
		{
			double w = w1 + i * gap;
			vector<double> cornerpt;// new point
			cornerpt.push_back(w);
			cornerpt.push_back(a / w);
			bc.push_back(cornerpt);
		}
		vector<double> cornerpt;
		cornerpt.push_back(w2);
		cornerpt.push_back(a / w2);
		bc.push_back(cornerpt);
	}
}
#ifndef slicingTree_h  
#define slicingTree_h

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#define same 0.1// normally make 'same' and 'gap' equal
#define gap 0.2

using namespace std;

class slicingTree
{
public:
	slicingTree(string, int, double, double, double);
	string name;
	bool leaf;
	int set;
	double a;
	double r;
	double s;
	int LorR;// 0 means it is a left child, 1 means it is a right child, -1 means it is root
	int which;// which corner is used
	float cellx;// this cell's x position
	float celly;// this cell's y position
	float cellw;// this cell's width
	float cellh;// this cell's height
	int cornerNum;
	vector<vector<double> > bc;// row = cornerNum, col = 2
	vector<vector<int> > pick;// ->bc// where the bc come from
	slicingTree *left;
	slicingTree *right;
	slicingTree *parent;

private:
	void calculateBoundingCurve(void);
	void forSet1(void);
	void forSet2(void);
};

#endif
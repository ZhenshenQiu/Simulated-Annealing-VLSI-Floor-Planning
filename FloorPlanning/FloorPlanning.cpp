#include "FloorPlanning.h"
using namespace FloorPlan;

#define FILENAME "module1.txt"

cool Anneal(initialTem, coolFactor, lowTemStop);

cool::cool(double t, double p, double l)
{
	temperature = t;// initial temperature
	coolRatio = p;// cool down parameter
	lowTem = l;
	acceptMove = 0;
	totalMove = 0;
	downMove = 0;
	upMove = 0;
	neighbor = 0;
	areaW = 0;
	areaH = 0;
	area = 0;
}

Void FloorPlanning::annealActive(void)
{
	SolidBrush^ Brush = gcnew SolidBrush( Color::White);
	Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->FillRectangle(Brush, rootW + 1, 60, textW, textH);
	delete Brush;
	delete formGraphics;
	DrawStringString("Downhill Moves: " + System::Convert::ToString(Anneal.downMove), rootW, 60);
	DrawStringString("Accepted Moves: " + System::Convert::ToString(Anneal.acceptMove), rootW, 90);
	DrawStringString("Total Attempts to Move: " + System::Convert::ToString(Anneal.totalMove), rootW, 120);
}

Void FloorPlanning::printOut(void)
{
	cleanRoot();
	drawCellShape(Anneal.root, Anneal.which, 0, 0, rootW, rootH);
	Anneal.debugTree(Anneal.root);
	DrawStringString("Temperature: " + System::Convert::ToString(static_cast<float>(Anneal.temperature)*4), rootW, 0);
	DrawStringString("Cost: " + System::Convert::ToString(Anneal.area), rootW, 30);
	DrawStringString("Downhill Moves: " + System::Convert::ToString(Anneal.downMove), rootW, 60);
	DrawStringString("Uphill Moves: " + System::Convert::ToString(Anneal.upMove), rootW, 90);
	DrawStringString("Accepted Moves: " + System::Convert::ToString(Anneal.acceptMove), rootW, 120);
	DrawStringString("Total Attempts to Move: " + System::Convert::ToString(Anneal.totalMove), rootW, 150);
	DrawStringString("Number of neighbors: " + System::Convert::ToString(Anneal.neighbor), rootW, 180);
	//sw->WriteLine(System::Convert::ToString(static_cast<float>(Anneal.temperature*2000)));
}

Void FloorPlanning::button1_Click(System::Object^  sender, System::EventArgs^  e) 
{
	txtRead(FILENAME);
	Anneal.createTree();
	Anneal.getNeighbor(Anneal.root);
	Anneal.accCurve(Anneal.root);// return the final bounding curve
	Anneal.updateCoolInfo(Anneal.root);
	printOut();
 }

Void FloorPlanning::button2_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//this->Controls->Remove(this->button1);
	
	Anneal.oneTemperature(Anneal.root);
	Anneal.accCurve(Anneal.root);// return the final bounding curve
	Anneal.updateCoolInfo(Anneal.root);
	printOut();
	//if(Anneal.acceptMove < static_cast<double>(Anneal.totalMove)*0.005 || Anneal.temperature < Anneal.lowTem)// stop annealing condition
	//	MessageBox::Show(this,"Annealing is done!");
	Anneal.temperatureDown();
 }

Void FloorPlanning::button3_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//ofstream outf; 
	int i=0;
	while (1)
	{
		Anneal.oneTemperature(Anneal.root);
		Anneal.accCurve(Anneal.root);// return the final bounding curve
		Anneal.updateCoolInfo(Anneal.root);
		printOut();
		Anneal.costRecord[i] = Anneal.area;
		i++;
		if(Anneal.acceptMove <static_cast<double>(Anneal.totalMove)*0.025 || Anneal.temperature < Anneal.lowTem)// stop annealing condition
			break;
		Anneal.temperatureDown();
	}
	Anneal.debugTree(Anneal.root);
	MessageBox::Show(this,"Annealing is done!");
 }

void cool::oneTemperature(slicingTree *temp)// cool down one step and update the information
{
	int a =0 ;
	for(totalMove = 0; totalMove < moveEachTem; )
	{
		//if( downMove >= neighbor)// || totalMove > 4*neighbor)// end cool down condition
		//	break;
		//else
		{
			int rollMove = rand() % 3 + 1;// choose one move 
			switch(rollMove)
			{
			case 1:
				if(neighbor1.size() > 0)
					Step1(temp);
				break;
			case 2:
				if(neighbor2.size() > 0)
					Step2(temp);
				break;
			case 3:
				if(neighbor3.size() > 0)
					Step3(temp);
				break;
			}
			
		}
		accCurve(Anneal.root);// return the final bounding curve
		updateCoolInfo(Anneal.root);
	}
}

void cool::temperatureDown(void)
{
	temperature = temperature * coolRatio;
	downMove = 0;
	upMove = 0;
	acceptMove = 0;
	totalMove = 0;
}

void cool::Step1(slicingTree *temp)
{
	int rollPair = rand() % neighbor1.size();// choose one pair and swap them
	swapNode(neighbor1[rollPair][0], neighbor1[rollPair][1]);
	if(!decision())// do not accept it, change back
		swapNode(neighbor1[rollPair][0], neighbor1[rollPair][1]);
	getNeighbor(root);
}

void cool::Step2(slicingTree *temp)
{
	int rollPair = rand() % neighbor2.size();// choose one pair and swap them
	newChain(rollPair);
	if(!decision())// do not accept it, change back
		newChain(rollPair);
	getNeighbor(root);
}

void cool::Step3(slicingTree *temp)
{
	int rollPair = rand() % neighbor3.size();// choose one pair and swap them
	string a = vtree[neighbor3[rollPair][0]]->name;
	string b = vtree[neighbor3[rollPair][1]]->name;
	swapNode(neighbor3[rollPair][0], neighbor3[rollPair][1]);
	createTree();
	getNeighbor(root);
	if(!decision())// do not accept it, change back
	{
		unsigned int first, second;
		for(unsigned int i = 0; i<vtree.size() - 1; i++)
		{
			if(vtree[i]->name == b && vtree[i+1]->name == a)
			{
				first = i;
				second = i + 1;
				break;
			}
		}
		swapNode(first, second);// can not swap like this
		createTree();
		getNeighbor(root);
	}
}

bool cool::decision()
{
	totalMove++;
	Anneal.accCurve(Anneal.root);// return the final bounding curve
	float areaPrev = Anneal.area; 
	Anneal.updateCoolInfo(Anneal.root);
	float deltaE = ( Anneal.area - areaPrev );
	if(temperature < initialTem * 0.85 )
	{
		which = 0;// initially choose the middle one
		for(unsigned int i=1; i<root->bc.size(); i++)// check constraint
		{
			if( (root->bc[i][1]/root->bc[i][0]) > p && (root->bc[i][1]/root->bc[i][0]) < q)// constraints
			{
				which = 1;
				break;
			}
		}
		if( which == 0)
		{
			return false;
		}
	}
	if( deltaE < 0)// it is a downhill move
	{
		Anneal.downMove++;
		Anneal.acceptMove++;
		return true;// accept it
	}
	else if ( deltaE < 0.00001 )
	{
		Anneal.acceptMove++;
		return true;
	}
	else
	{
		Anneal.upMove++;
		double P = pow(naturee, -deltaE / Anneal.temperature);
		double roll = static_cast<double>(rand() % 10000) / 10000.0;
		if(roll < P)
		{
			Anneal.acceptMove++;
			return true;
		}
		else
			return false;
	}
}

void cool::newChain(int p)
{
	for(unsigned int i = 0; i<neighbor2[p].size(); i++)
	{
		if(vtree[neighbor2[p][i]]->name == "*")
			vtree[neighbor2[p][i]]->name = "+";
		else
			vtree[neighbor2[p][i]]->name = "*";
	}
}

void cool::swapNode(int a, int b)
{
	string nameT = vtree[a]->name;
	bool leafT = vtree[a]->leaf;
	int setT = vtree[a]->set;
	double aT = vtree[a]->a;
	double rT = vtree[a]->r;
	double sT = vtree[a]->s;
	int LorRT = vtree[a]->LorR;
	int whichT = vtree[a]->which;
	int cornerNumT = vtree[a]->cornerNum;
	vector<vector<double> > bcT = vtree[a]->bc;
	vector<vector<int> > pickT = vtree[a]->pick;// ->bc// where the bc come from
	slicingTree *leftT = vtree[a]->left;
	slicingTree *rightT = vtree[a]->right;
	slicingTree *parentT = vtree[a]->parent;


	vtree[a]->name = vtree[b]->name;
	vtree[a]->leaf = vtree[b]->leaf;
	vtree[a]->set = vtree[b]->set;
	vtree[a]->a = vtree[b]->a;
	vtree[a]->r = vtree[b]->r;
	vtree[a]->s = vtree[b]->s;
	vtree[a]->LorR = vtree[b]->LorR;
	vtree[a]->which = vtree[b]->which;
	vtree[a]->cornerNum = vtree[b]->cornerNum;
	vtree[a]->bc = vtree[b]->bc;
	vtree[a]->pick = vtree[b]->pick;// ->bc// where the bc come from
	vtree[a]->left = vtree[b]->left;
	vtree[a]->right = vtree[b]->right;
	vtree[a]->parent = vtree[b]->parent;

	vtree[b]->name = nameT;
	vtree[b]->leaf = leafT;
	vtree[b]->set = setT;
	vtree[b]->a = aT;
	vtree[b]->r = rT;
	vtree[b]->s = sT;
	vtree[b]->LorR = LorRT;
	vtree[b]->which = whichT;
	vtree[b]->cornerNum = cornerNumT;
	vtree[b]->bc = bcT;
	vtree[b]->pick = pickT;// ->bc// where the bc come from
	vtree[b]->left = leftT;
	vtree[b]->right = rightT;
	vtree[b]->parent = parentT;

}

void cool::updateCoolInfo(slicingTree *temp)
{
	//areaW = static_cast<float>(temp->bc[0][0]);
	//areaH = static_cast<float>(temp->bc[0][1]);
	area = maxArea;
	which = temp->bc.size()/2;// initially choose the middle one
	for(unsigned int i=1; i<temp->bc.size(); i++)// choose a final corner point
	{
		if( (temp->bc[i][1]/temp->bc[i][0]) > p && (temp->bc[i][1]/temp->bc[i][0]) < q)// constraints
		{
				which = i;
				break;
		}
	}
	for(unsigned int i=1; i<temp->bc.size(); i++)// choose a final corner point
	{
		if( (temp->bc[i][1]/temp->bc[i][0]) > p && (temp->bc[i][1]/temp->bc[i][0]) < q)// constraints
		{
			float area2 = static_cast<float>((temp->bc[i][1]) * (temp->bc[i][0]));
			if(  area2 < area )
			{
				area = area2;
				which = i;
			}
		}
	}
	areaW = static_cast<float>(temp->bc[which][0]);
	areaH = static_cast<float>(temp->bc[which][1]);
	area = areaW * areaH;

}

void cool::getNeighbor(slicingTree *temp)
{
	getNeighbor1(temp);
	getNeighbor2(temp);
	getNeighbor3(temp);
	neighbor = neighbor1.size() + neighbor2.size() + neighbor3.size();
}

void cool::getNeighbor1(slicingTree *temp)// find neighbor leaf
{
	neighbor1.clear();
	stack<int> stemp;
	for(unsigned int i = 0; i<vtree.size(); i++)
	{
		if(vtree[i]->name != "*" && vtree[i]->name != "+" )// it is a leaf
		{
			stemp.push(i);
			if(stemp.size() == 2)// find a pair
			{
				vector<int> vtemp(2);
				vtemp[1] = stemp.top();// put in the vtemp
				stemp.pop();
				vtemp[0] = stemp.top();// put in the vtemp
				stemp.pop();
				neighbor1.push_back(vtemp);
				stemp.push(i);// put the second into stack again
			}
		}
	}
}

void cool::getNeighbor2(slicingTree *temp)// find neighbor node
{
	neighbor2.clear();
	stack<int> stemp;
	for(unsigned int i = 0; i<vtree.size(); i++)
	{
		if(vtree[i]->name == "*" || vtree[i]->name == "+" )// it is a node
		{
			stemp.push(i);
		}
		else
		{
			vector<int> vtemp;
			if(stemp.size() > 0)
			{
				for(unsigned int i = 0; i < stemp.size(); i++)
				{
					vtemp.push_back(stemp.top());
					stemp.pop();
				}
				neighbor2.push_back(vtemp);
			}
		}
		vector<int> vtemp;
			if(stemp.size() > 0)
			{
				for(unsigned int i = 0; i < stemp.size(); i++)
				{
					vtemp.push_back(stemp.top());
					stemp.pop();
				}
				neighbor2.push_back(vtemp);
			}
	}
	int a = 0;
}

void cool::getNeighbor3(slicingTree *temp)
{
	neighbor3.clear();
	for(unsigned int i = 0; i<vtree.size() - 1; i++)
	{
		if(i + 1 > vtree.size() - 1)
			int a = 0;
		if((vtree[i]->leaf && !vtree[i+1]->leaf) || (vtree[i+1]->leaf && !vtree[i]->leaf))// neighbor cell and cut
		{
			vector<int> vtemp(2);
			vtemp[0] = i;
			vtemp[1] = i + 1;
			if(checkLegal(i, i+1))// it is a legal neighbor3
			{
				neighbor3.push_back(vtemp);
			}
		}
	}
}

bool cool::checkLegal(int a, int b)
{
	swapNode(a, b);// swap them first
	int cutNum = 0;
	int cellNum = 0;
	for( unsigned int i = 0; i < vtree.size() - 1; i++)
	{
		if(i + 1 > vtree.size() - 1)
			int a = 0;
		if(vtree[i]->name == vtree[i+1]->name)// cause an illegal expression
		{
			swapNode(a,b);// swap back
			return false;
		}
		if(vtree[i]->leaf)
			cellNum++;
		else
			cutNum++;
		if(cutNum >= cellNum)// not skew tree
		{
			swapNode(a,b);
			return false;
		}
	}
	swapNode(a,b);
	return true;
}

/**********************   read the net list file   **********************************/
void FloorPlanning::txtRead(String^ filename)  
{   
#pragma region   read the txt file

	try
	{
	// Create an instance of StreamReader to read from a file.
      StreamReader^ sr = gcnew StreamReader( filename );
      try
      {
         String^ line;
         // Read and display lines from the file until the end of 
         // the file is reached.
         for(int i=0; line = sr->ReadLine();i++)
         {
			 // MessageBox::Show(this,line);
			 string s = marshal_as<std::string>(line);
			 checkLine(s, i);// check every line and do different operations 
			 Console::WriteLine( line );
         }
      }
      finally
      {
         if ( sr )
            delete (IDisposable^)sr;
      }
   }
   catch ( Exception^ e ) 
   {
      // Let the user know what went wrong.
	  MessageBox::Show(this,"I suck");
      Console::WriteLine( "The file could not be read:" );
      Console::WriteLine( e->Message );
   }
#pragma endregion

}

int FloorPlanning::drawCellShape(slicingTree *temp, int n, float parentX, float parentY, float parentW, float parentH)
{
	if(temp->leaf)
	{
		temp->which = n;
		temp = Anneal.getPosition(temp, parentX, parentY, parentW, parentH);
		FillRectangleRectangle(temp->cellx, temp->celly, temp->cellw, temp->cellh);
		DrawStringString(temp->name, temp->cellx, temp->celly);
		//DrawStringString(temp->name, temp->cellx + temp->cellw/2, temp->celly + temp->cellh/2);
		return 0;
	}
	else
	{
		temp->which = n;// record which bc is used
		temp = Anneal.getPosition(temp, parentX, parentY, parentW, parentH);// get the size and position of this area
		if(temp->name == "*")// vertical partition
		{
			int leftChoice = temp->pick[temp->which][leftN];// which left bc is used
			int rightChoice = temp->pick[temp->which][rightN];// which right bc is used
			float leftw = static_cast<float>(temp->left->bc[leftChoice][pickW]);// left area width
			float rightw = static_cast<float>(temp->right->bc[rightChoice][pickW]);// right area width

			float lw = leftw / (leftw + rightw) * temp->cellw;
			float lh = temp->cellh;
			float rw = rightw / (leftw + rightw) * temp->cellw;
			float rh = temp->cellh;

			float lx = temp->cellx;
			float ly = temp->celly;
			float rx = lw + lx;
			float ry = temp->celly;
			
			DrawRectangleRectangle(temp->cellx, temp->celly, temp->cellw, temp->cellh);

			drawCellShape(temp->left, leftChoice, lx, ly, lw, lh);// find the left leaf
			drawCellShape(temp->right, rightChoice, rx, ry,rw, rh);// find the right leaf
		}
		else// horizontal partition
		{
			int leftChoice = temp->pick[temp->which][leftN];// which left bc is used
			int rightChoice = temp->pick[temp->which][rightN];// which right bc is used
			float lefth = static_cast<float>(temp->left->bc[leftChoice][pickH]);// left area height
			float righth = static_cast<float>(temp->right->bc[rightChoice][pickH]);// right area height

			float lh = lefth / (lefth + righth) * temp->cellh;
			float lw = temp->cellw;
			float rh = righth / (lefth + righth) * temp->cellh;
			float rw = temp->cellw;

			float rx = temp->cellx;
			float ry = temp->celly;
			float lx = temp->cellx;
			float ly = ry + rh;
			
			DrawRectangleRectangle(temp->cellx, temp->celly, temp->cellw, temp->cellh);

			drawCellShape(temp->left, leftChoice, lx, ly, lw, lh);// find the left leaf
			drawCellShape(temp->right, rightChoice, rx, ry,rw, rh);// find the right leaf
		}
	}
	return 0;
}

slicingTree *cool::getPosition(slicingTree *temp, float parentX, float parentY, float parentW, float parentH)
{
	if( (temp->bc[temp->which][pickW] / temp->bc[temp->which][pickH]) > (parentW / parentH) )
	{
		temp->cellw = parentW;
		temp->cellh = static_cast<float>(temp->cellw * (temp->bc[temp->which][pickH]) / (temp->bc[temp->which][pickW]));
		temp->cellx = parentX;
		temp->celly = parentY + (parentH - temp->cellh) / 2;
	}
	else
	{
		temp->cellh = parentH;
		temp->cellw = static_cast<float>(temp->cellh / (temp->bc[temp->which][pickH]) * (temp->bc[temp->which][pickW]));
		temp->cellx = parentX + (parentW - temp->cellw) / 2;
		temp->celly = parentY;
	}
	return temp;
}

int cool::accCurve(slicingTree *temp)
{
	if(temp->leaf)// it is a leaf node
	{
		return 0;
	}
	else
	{
		accCurve(temp->left);// go to the left node
		accCurve(temp->right);// go to the right node
		newBC(temp);// generate the nodes' bounding curve
		return 0;
	}
	Anneal.root = temp;// update the root
}

int cool::newBC(slicingTree *temp)// using its two child nodes' bounding curve to get its bounding curve
{
	if(temp->name == "*")// vertical cut
	{
		temp = vCut(temp);
	}
	else// horizontal cut
	{
		temp = hCut(temp);
	}
	temp->cornerNum = temp->bc.size();// update the number of corner Points
	return 0;
}

slicingTree *cool::vCut(slicingTree *temp)
{
	int loop1 = temp->left->bc.size();// number of the bc1 corner points
	int loop2 = temp->right->bc.size();// number of the bc2 corner points
	vector<vector<double> > bc;// create a new bounding curve
	vector<vector<int> > pk;
	for(int i=0; i<loop1; i++)
	{
		for(int j=0; j<loop2; j++)
		{
			vector<double> cornerpt;// new point
			cornerpt.push_back(temp->left->bc[i][0] + temp->right->bc[j][0]);// width
			cornerpt.push_back(max(temp->left->bc[i][1], temp->right->bc[j][1]));// height
			bool r = true;
			for(unsigned int k=0; k<bc.size(); k++)// kick out redundant
			{
				if( (cornerpt[0] - bc[k][0] > -0.001) && (cornerpt[1] - bc[k][1] > -0.001))
				{
					r = false;// it's redundant
					break;
				}
				else if( (cornerpt[0] - bc[k][0] < 0.001) && (cornerpt[1] - bc[k][1] < 0.001) )
				{
					r = false;// find a redundant
					bc[k][0] = cornerpt[0];
					bc[k][1] = cornerpt[1];
					pk[k][0] = i;
					pk[k][1] = j;
					break;
				}
			}
			if(r)
			{
				bc.push_back(cornerpt);
				vector<int> pick;
				pick.push_back(i);
				pick.push_back(j);
				pk.push_back(pick);
			}
		}
	}
	temp->bc = bc;
	temp->pick = pk;
	return temp;
}

slicingTree *cool::hCut(slicingTree *temp)
{
	int loop1 = temp->left->bc.size();// number of the bc1 corner points
	int loop2 = temp->right->bc.size();// number of the bc2 corner points
	vector<vector<double> > bc;// create a new bounding curve
	vector<vector<int> > pk;
	for(int i=0; i<loop1; i++)
	{
		for(int j=0; j<loop2; j++)
		{
			vector<double> cornerpt;// new point
			cornerpt.push_back(max(temp->left->bc[i][0], temp->right->bc[j][0]));// width
			cornerpt.push_back(temp->left->bc[i][1] + temp->right->bc[j][1]);// height
			bool r = true;
			for(unsigned int k=0; k<bc.size(); k++)// kick out redundant
			{
				if( (cornerpt[0] - bc[k][0] > -0.001) && (cornerpt[1] - bc[k][1] > -0.001))
				{
					r = false;// it's redundant
					break;
				}
				else if( (cornerpt[0] - bc[k][0] < 0.001) && (cornerpt[1] - bc[k][1] < 0.001) )
				{
					r = false;// find a redundant
					bc[k][0] = cornerpt[0];
					bc[k][1] = cornerpt[1];
					pk[k][0] = i;
					pk[k][1] = j;
					break;
				}
			}
			if(r)
			{
				bc.push_back(cornerpt);
				vector<int> pick;
				pick.push_back(i);
				pick.push_back(j);
				pk.push_back(pick);
			}
		}
	}
	temp->bc = bc;
	temp->pick = pk;
	return temp;
}


void cool::createTree()
{
	stack<slicingTree *> stemp;
	for(unsigned int i =0; i<vtree.size(); i++)
	{
		if(vtree[i]->leaf)
			stemp.push(vtree[i]);
		else
		{
			vtree[i]->right = stemp.top();
			vtree[i]->right->LorR = 1;// record it is a right child
			vtree[i]->right->parent = vtree[i];
			stemp.pop();
			vtree[i]->left = stemp.top();
			vtree[i]->left->LorR = 0;// record it is a left child
			vtree[i]->left->parent = vtree[i];
			stemp.pop();
			stemp.push(vtree[i]);
		}
	}
	root = stemp.top();
}

char FloorPlanning::checkLine (string s, int n)
{
	unsigned int start;
	unsigned int end;
	vector<string> v;
	string s1;
	for(start = 0, end = 0; end < s.size(); end++)
	{
		if(s[end] == ' ')
		{
			s1 = s.substr(start, end - start);
			start = end + 1;
			v.push_back(s1);
		}
		else if( end == s.size()-1)
		{
			s1 = s.substr(start, end - start + 1);
			start = end + 1;
			v.push_back(s1);
		}
	}
	if(v.size() == 3)// first line is number, p and q
	{ 
		const string sp0 = v[0];
		const char *ss0 = sp0.c_str();
		Anneal.n = atoi(ss0);
		const string sp1 = v[1];
		const char *ss1 = sp1.c_str();
		Anneal.p = atof(ss1);
		const string sp2 = v[2];
		const char *ss2 = sp2.c_str();
		Anneal.q = atof(ss2);
	}
	else // it is a module
	{
		const string sp0 = v[0];
		const char *ss0 = sp0.c_str();
		int name = atoi(ss0);
		const string sp1 = v[1];
		const char *ss1 = sp1.c_str();
		double area = atof(ss1);
		const string sp2 = v[2];
		const char *ss2 = sp2.c_str();
		double cellR = atof(ss2);
		const string sp3 = v[3];
		const char *ss3 = sp3.c_str();
		double cellS = atof(ss3);
		const string sp4 = v[4];
		const char *ss4 = sp4.c_str();
		int set = atoi(ss4);

		slicingTree *node = new slicingTree(v[0], set, area, cellR, cellS);// create a leaf node
		Anneal.vtree.push_back(node);
		//
		// initially, put all cells horizontally, make the temperature highest
		//
		if(n>1)
		{
			//slicingTree *node2 = new slicingTree("*", 0, 0, 0, 0);// create a node
			//Anneal.vtree.push_back(node2);
			if( n % 2 )
			{
				slicingTree *node2 = new slicingTree("+", 0, 0, 0, 0);// create a node
				Anneal.vtree.push_back(node2);
			}
			else
			{
				slicingTree *node2 = new slicingTree("*", 0, 0, 0, 0);// create a node
				Anneal.vtree.push_back(node2);
			}
		}

	}
	return 0;
}

void cool::debugTree(slicingTree *temp)
{
	if(temp == NULL)
		return;
	else
	{
		debugTree(temp->left);
		debugTree(temp->right);
		Anneal.debug.push_back(temp->name);
	}
}

/**********************   Draw the module to the region   ***************************/
Void FloorPlanning::cleanRoot(void)//System::Drawing::Rectangle^ rect)
{
	SolidBrush^ Brush = gcnew SolidBrush( Color::White);
	Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->FillRectangle(Brush, 0, 0, rootH, rootH);
	formGraphics->FillRectangle(Brush, rootW + 1, 0, textW, textH);
	delete Brush;
	delete formGraphics;
	DrawRectangleRectangle(0 , 0, rootW, rootH);
	DrawRectangleRectangle(rootW + 2, 0, textW - 2, textH);
}

/**********************   Draw the module to the region   ***************************/
Void FloorPlanning::FillRectangleRectangle(float a, float b, float c, float d)//System::Drawing::Rectangle^ rect)
{
	int ran = rand() % 5 + 1;
	SolidBrush^ Brush = gcnew SolidBrush(Color::GreenYellow);;
	/*
	switch(ran)
	{
	case 1:
		Brush = gcnew SolidBrush(Color::CornflowerBlue);
		break;
	case 2:
		Brush = gcnew SolidBrush(Color::GreenYellow);
		break;
	case 3:
		Brush = gcnew SolidBrush(Color::OrangeRed);
		break;
	case 4:
		Brush = gcnew SolidBrush(Color::MediumPurple);
		break;
	case 5:
		Brush = gcnew SolidBrush(Color::Yellow);
		break;
	default:
		Brush = gcnew SolidBrush(Color::Beige);
		break;
	}
	*/
	Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->FillRectangle(Brush, a, b, c, d);
	delete Brush;
	delete formGraphics;
	DrawRectangleRectangle(a , b, c, d);
}

/**********************   Draw the module to the region   ***************************/
Void FloorPlanning::DrawRectangleRectangle(float a, float b, float c, float d)//System::Drawing::Rectangle^ rect)
{
	Pen^ pen = gcnew Pen(Color::Black);
	Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->DrawRectangle(pen, a, b, c, d);
	delete pen;
	delete formGraphics;
}

/**********************   Draw the line to the region   ***************************/
Void FloorPlanning::DrawLineLine(float a, float b, float c, float d)
{
	
	System::Drawing::Pen^ pen =
	gcnew System::Drawing::Pen(System::Drawing::Color::Green);
	System::Drawing::Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->DrawLine(pen, a, b, c, d);
	delete pen;
	delete formGraphics;
}

/**********************   Draw String to the region   ***************************/
Void FloorPlanning::DrawStringString(string s, float a, float b)
{
	String ^str = gcnew String(s.c_str());
	System::Drawing::Font ^drawFont = gcnew System::Drawing::Font("Arial", 12);
	SolidBrush ^Brush = gcnew SolidBrush( Color::Black);
	PointF pt = PointF(a, b);
	System::Drawing::Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->DrawString(str, drawFont, Brush, pt);
	delete Brush;
	delete formGraphics;
}
/**********************   Draw String to the region   ***************************/
Void FloorPlanning::DrawStringString(String ^s, float a, float b)
{
	System::Drawing::Font ^drawFont = gcnew System::Drawing::Font("Arial", 12);
	SolidBrush ^Brush = gcnew SolidBrush( Color::Black);
	PointF pt = PointF(a, b);
	System::Drawing::Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->DrawString(s, drawFont, Brush, pt);
	delete Brush;
	delete formGraphics;
}
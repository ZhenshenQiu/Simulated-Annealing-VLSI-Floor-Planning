#pragma once

#include <iostream>
#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <vcclr.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include <stack>
#include <msclr\marshal_cppstd.h>

#include "slicingTree.h"
#include "cornerPoint.h"

#define outfile "output.txt"
#define formW 1000
#define formH 700
#define rootW 700
#define rootH 700
#define buttonM (formW - rootW -2)/3
#define buttonH 50
#define textW formW - rootW -1
#define textH formH - buttonH -2
#define leftN 0
#define rightN 1
#define pickW 0
#define pickH 1
#define initialTem 500.00
#define lowTemStop 0.1
#define coolFactor 0.95
#define moveEachTem 20
#define naturee 2.71828
#define maxArea 1000

namespace FloorPlan {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace std;
	using namespace msclr::interop;

	class cool
	{
	public:
		cool::cool(double, double, double);
		slicingTree *root;// root of the slicing tree
		int n; // total number of modules
		double p;// up constraint
		double q;// down constraint
		vector<slicingTree *> vtree;// polish expression
		void createTree(void);
		int accCurve(slicingTree *);
		int newBC(slicingTree *);
		slicingTree *vCut(slicingTree *);
		slicingTree *hCut(slicingTree *);
		void getAreaSize(slicingTree *);
		void debugTree(slicingTree *);
		void updateCoolInfo(slicingTree *);
		void getNeighbor(slicingTree *);
		void getNeighbor1(slicingTree *);
		void getNeighbor2(slicingTree *);
		void getNeighbor3(slicingTree *);
		slicingTree *getPosition(slicingTree *, float, float, float, float);
		vector<string> debug;

		void oneTemperature(slicingTree *);
		void Step1(slicingTree *);
		void Step2(slicingTree *);
		void Step3(slicingTree *);
		void swapNode(int, int);
		void newChain(int);
		bool decision(void);
		bool checkLegal(int, int);
		void temperatureDown(void);

		int which;// which root corner point is used

		unsigned int acceptMove;
		unsigned int totalMove;
		unsigned int downMove;// number of downhill moves
		unsigned int upMove;
		double temperature;
		float area;
		double coolRatio;
		double lowTem;
		unsigned int neighbor;// number of neighbors

		vector<vector<int> > neighbor1;
		vector<vector<int> > neighbor2;
		vector<vector<int> > neighbor3;
		
		float costRecord[1000];
		

	private:
		float areaW;
		float areaH;
	};
	/// <summary>
	/// FloorPlanning 摘要
	/// </summary>
	public ref class FloorPlanning : public System::Windows::Forms::Form
	{
	public:
		FloorPlanning(void)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			srand((int)time(0));
			sw = gcnew StreamWriter( outfile );
		}
		System::Void printOut(void);
		System::Void annealActive(void);
		System::Void DrawRectangleRectangle(float, float, float, float);//System::Drawing::Rectangle^ rect);
		System::Void FillRectangleRectangle(float, float, float, float);//System::Drawing::Rectangle^ rect);
		System::Void DrawLineLine(float, float, float, float);
		System::Void DrawStringString(string, float, float);
		System::Void DrawStringString(String ^, float , float);
		System::Void txtRead(System::String^ filename) ; 
		System::Void cleanRoot(void);//System::Drawing::Rectangle^ rect)
		char FloorPlanning::checkLine (string, int);
		int drawCellShape(slicingTree *, int, float, float, float, float);
		StreamWriter^ sw;


	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~FloorPlanning()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;
		System::Windows::Forms::Button^  button1;// read the txt
		System::Windows::Forms::Button^  button2;// cool down step by step
		System::Windows::Forms::Button^  button3;// auto cool down
		System::Windows::Forms::TextBox^  textBox1; 
		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) ;
		System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) ;
		System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) ;
		
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			cleanRoot();
			this->BackColor = Color::White;
			//
			// botton1, read txt
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button1->Location = System::Drawing::Point(rootW + 2, formH - buttonH);
			this->button1->Name = L"ReadTxt";
			this->button1->Size = System::Drawing::Size((formW - rootW -2)/3, buttonH);
			this->button1->TabIndex = 0;
			this->button1->Text = L"ReadTxt";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &FloorPlanning::button1_Click);
			this->Controls->Add(this->button1);
			// 
			// button2, cool down the temperature step by step
			// 
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button2->Location = System::Drawing::Point(rootW + buttonM + 2, formH - buttonH);
			this->button2->Name = L"CoolDown";
			this->button2->Size = System::Drawing::Size((formW - rootW -2)/3, buttonH);
			this->button2->TabIndex = 0;
			this->button2->Text = L"CoolDown";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &FloorPlanning::button2_Click);
			// 
			// button3, auto cool down
			// 
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button3->Location = System::Drawing::Point(rootW + buttonM * 2 + 2, formH - buttonH);
			this->button3->Name = L"AutoCoolDown";
			this->button3->Size = System::Drawing::Size((formW - rootW -2)/3, buttonH);
			this->button3->TabIndex = 0;
			this->button3->Text = L"AutoCoolDown";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &FloorPlanning::button3_Click);
			//
			// textbox1
			//
			this->textBox1 = gcnew TextBox;
			this->textBox1->Location = System::Drawing::Point(rootW + 1, 0);
			this->textBox1->Size = System::Drawing::Size(textW, textH);
			this->textBox1->Multiline = true;// Set the Multiline property to true.
			this->textBox1->ScrollBars = ScrollBars::Vertical;// Add vertical scroll bars to the TextBox control.      
			this->textBox1->AcceptsReturn = true;// Allow the RETURN key to be entered in the TextBox control.      
			this->textBox1->AcceptsTab = true;// Allow the TAB key to be entered in the TextBox control.
			this->textBox1->WordWrap = true;// Set WordWrap to True to allow text to wrap to the next line.
			this->textBox1->Text = "Welcome!";// Set the default text of the control.
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(formW, formH + 1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button3);
			//this->Controls->Add(this->textBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
	
		}
#pragma endregion
	};
}

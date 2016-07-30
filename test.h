#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>

using namespace std;

class Test
{
public:
	Test();
	~Test();
	void add(int number);
	vector<int> get();

private:
	vector<int> mVec;
};

#endif
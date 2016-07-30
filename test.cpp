#include "test.h"

Test::Test()
{
	
}

Test::~Test()
{

}

void Test::add(int number)
{
	mVec.push_back(number);
}

vector<int> Test::get()
{
	return mVec;
}
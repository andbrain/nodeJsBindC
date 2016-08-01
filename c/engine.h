#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <vector>
// #include "tads/qparser.h"
#include "tads/qprocessor.h"
// #include "tads/ireader.h"
#include "tads/document.h"

using namespace std;

class Engine
{
public:
	Engine();
	~Engine();
	vector<int> init(string query);
};

#endif
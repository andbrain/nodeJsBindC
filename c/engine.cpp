#include "engine.h"

Engine::Engine()
{
	cout << "Starting engine.." << endl;
}

Engine::~Engine()
{
	cout << "Ending engine.." << endl;
}

void Engine::init()
{
	mProcessor.Initialize();	
}

vector<int> Engine::process(string query)
{
	cout << "Quering..." << endl;
	cout << query << endl;
	string q = "Is CF mucus abnormal?";
	vector<string> words = mProcessor.SelectWords(q);
	vector<int> topn = mProcessor.ProcessQuery(words, 10);

	return topn;
}
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
	vector<string> words = mProcessor.SelectWords(query);
	vector<int> topn = mProcessor.ProcessQuery(words, 10);

	return topn;
}
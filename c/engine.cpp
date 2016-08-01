#include "engine.h"

Engine::Engine()
{
	cout << "Starting engine.." << endl;
}

Engine::~Engine()
{
	delete mProcessor;
	delete mQuery;
	cout << "Ending engine.." << endl;
}

void Engine::init()
{
	//Initialize, and expect to read indexed base
	vector<Document *>* mQuery = new vector<Document *>();
	mProcessor = new Qprocessor(mQuery); 
	mProcessor->Initialize();	
}

vector<int> Engine::process(string query)
{
	vector<string> words = mProcessor->SelectWords(query);
	vector<int> topn = mProcessor->ProcessQuery(words, 5);

	return topn;
}
#include "engine.h"

Engine::Engine()
{
	cout << "Starting engine.." << endl;
}

Engine::~Engine()
{
	cout << "Ending engine.." << endl;
}

vector<int> Engine::init(string query)
{
	//Initialize, and expect to read indexed base
	vector<Document *>* vQuery = new vector<Document *>();
	Qprocessor *proc = new Qprocessor(vQuery); 
	proc->Initialize();

	// // Search
	string fakeQuery = "Can one distinguish between the effects of mucus hypersecretion and infection on the submucosal glands of the respiratory tract in CF?";
	vector<string> words = proc->SelectWords(fakeQuery);
	vector<int> topn = proc->ProcessQuery(words, 5);
	delete proc;
	delete vQuery;

	return topn;
}
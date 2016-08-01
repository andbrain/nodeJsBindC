#include "qprocessor.h"

Qprocessor::Qprocessor(vector<Document *>* base)
{
	cout << "starting qprocessor.." << endl;
	this->base = base;

	ir = new Ireader("dataset/base");
	ir->Process();
	RetrieveNorma("dataset/norma");
}

Qprocessor::~Qprocessor()
{
	cout << "finishing qprocessor.." << endl;

	for (unsigned int i = 0; i < queries.size(); ++i)
	{
		delete queries[i];
	}

	delete ir;
}

void Qprocessor::RetrieveNorma(string path)
{
	string path_ref = path + ".ref";
	fstream fs(path_ref, ios::in);

	if(!fs.is_open())
	{
		cout << "Error opening file: " << path_ref << endl;
		exit(1);
	}

	string line;
	vector<string> v;

	while(getline(fs,line))
	{
		v = Split(line, ' ');
		if(v.size() == 2)
			norma.insert(pair<string,double>(v[0], atof(v[1].c_str())));
		else if(v.size() == 3)
			norma.insert(pair<string,double>(v[0], atof(v[2].c_str())));
	}

	fs.close();
}

void Qprocessor::Initialize()
{
	LoadStopWords();

	Query *query;

	for (vector<Document *>::iterator i = base->begin(); i != base->end(); ++i)
	{
		query = new Query();
		query->content = SelectWords((*i)->GetAttribute("QU"));
		queries.push_back(query);
	}
}

void Qprocessor::LoadStopWords()
{
	fstream fwords(STOPWORDS_FILE, ios::in);

	string line;

	while(getline(fwords,line))
		stopwords[line] = 1;
	
	fwords.close();
}

bool Qprocessor::IsStopWords(string word)
{
	string word_lower = StrToLower(word);
	unordered_map<string,int>::const_iterator got = stopwords.find(word_lower);

	if(got != stopwords.end())
		return true;

	return false;
}

vector<string> Qprocessor::SelectWords(string question)
{
	string::iterator it;
	string word = "";
	vector<string> terms;

	for(it = question.begin(); it != question.end(); ++it)
	{
		if (isalpha(*it) && word.size() < 40)
			word += *it;
		else
		{
			if(word != "" && !IsStopWords(word))
				terms.push_back(StrToLower(word));			
			word = "";			
		}
	}

	return terms;
}

int Qprocessor::Process()
{
	cout << "..Processing queries" << endl;
	struct timeval begin, end;
	int counter = 1;
	string inf;

	//Proccess each query
	for (vector<Query *>::iterator q = queries.begin(); q != queries.end(); ++q)
	{
		inf = "Query: " + to_string(counter);
		cout << inf << endl;

		counter++;

		/*Measuring time elapsed*/
 		gettimeofday(&begin, NULL);
		
		vector<int> topn = ProcessQuery((*q)->content, 10);
		
		for (unsigned int i = 0; i < topn.size(); ++i)
		{
			cout << topn[i] << endl;
		}

		gettimeofday(&end, NULL);
		int tmili = (int) (1000 * (end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec) / 1000);
		cout << "Time elapsed:" << (double)tmili/1000 << " (s)" << endl;
	}

	return 0;
}

vector<int> Qprocessor::ProcessQuery(vector<string> qWords, int topn)
{
	unordered_map<string,double> weight;
	vector<Score *> ranking;
	// std::vector<string> words = query->content;

	for (std::vector<string>::iterator w = qWords.begin(); w != qWords.end(); ++w)
	{
		Term* inv_list = ir->GetStruct(*w);
		if(inv_list == NULL)
			continue;
		MeasureSim(inv_list, &weight, &ranking);
		DeleteTerm(inv_list);
	}

	CalculateSimilarity( &weight, &ranking);
	CreateRanking(&ranking);
	vector<int> vTopn = GetTopN(topn, &ranking);
	DeleteRanking(&ranking);

	return vTopn;
}

int Qprocessor::MeasureSim(Term *term, unordered_map<string,double> *weight, vector<Score *> *ranking)
{
	Doc *cur = term->document;
	double word_idf = term->idf;
	double gain = term->gain;
	CalculateParcials(word_idf, gain, cur, weight);

	while(cur->next != NULL)
	{
		cur = cur->next;
		CalculateParcials(word_idf, gain, cur, weight);
	}

	return 0;	
}

int Qprocessor::CalculateParcials(double idf, double gain,Doc *doc, unordered_map<string,double> *weight)
{
	double wDoc = CalculateWeight2(doc->frequence, idf, gain); // tf*idf of doc
	double wQuery = idf; //assuming weight of query as 1
	
	double weight_parc = wDoc * wQuery; 
		
	string id(doc->id);
	Trim(id);
	unordered_map<string,double>::iterator w = weight->find(id);
	
	if(w == weight->end())
	{
		weight->insert(pair<string,double>(id, weight_parc));
	}
	else
	{
		double aux = weight->at(id);
		w->second = aux + weight_parc;
	}

	return 0;
}

void Qprocessor::DeleteTerm(Term *term)
{
	Doc *aux;
	while(term->document->next != NULL)
	{
		aux = term->document;
		term->document = term->document->next;
		delete aux;
	}

	delete term->document;
	delete term;

}

void Qprocessor::CalculateSimilarity(unordered_map<string,double> *weight, vector<Score *> *ranking)
{
	unordered_map<string,double>::iterator n,w = weight->begin();
	double sim;
	string strDoc;
	Score *score;
	for (; w != weight->end(); ++w)
	{
		strDoc = w->first;
		Trim(strDoc);
		n = norma.find(strDoc);
		sim = w->second / n->second;
		score = new Score();
		score->document = strDoc;
		score->similarity = sim;

		ranking->push_back(score);
	}
}
	
void Qprocessor::CreateRanking(vector<Score *> *ranking)
{
	sort(ranking->begin(), ranking->end(), sortBysim);
}

vector<int> Qprocessor::GetTopN(int n, vector<Score *> *ranking)
{
	vector<int> vec;
	int docId;
	for (int i = 0; i < n; ++i)
	{
		docId = atoi(ranking->at(i)->document.c_str());
		vec.push_back(docId);
	}

	return vec;	
}

	
void Qprocessor::DeleteRanking(vector<Score *> *ranking)
{
	for (unsigned int i = 0; i < ranking->size(); ++i)
	{
		delete ranking->at(i);
	}
}
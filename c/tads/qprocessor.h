#ifndef QPROCESSOR_H
#define QPROCESSOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <sys/time.h>

#include "document.h"
#include "query.h"
#include "util.h"
#include "ireader.h"
#include "index.h"

#define STOPWORDS_FILE "resources/stopwords.dat"
// #define NORMA "norma.ref"
#define RESULT "result.txt"
#define RANKING "ranking.txt"

using namespace std;

class Qprocessor
{
public:
	Qprocessor(vector<Document *>* base);
	Qprocessor();
	~Qprocessor();
	void Initialize();
	int Process();
	vector<int> ProcessQuery(vector<string> words, int topn);
	vector<string> SelectWords(string question);

private:
	Ireader *ir;
	unordered_map<string,int> stopwords;
	unordered_map<string,double> norma;

	//initializing processor
	void RetrieveNorma(string path);
	void LoadStopWords();
	bool IsStopWords(string word);

	//proccess of queries
	int MeasureSim(Term *term, unordered_map<string,double> *weight, vector<Score *> *ranking);
	void DeleteTerm(Term *term);
	int CalculateParcials(double idf, double gain, Doc *doc, unordered_map<string,double> *weight);
	void CalculateSimilarity(unordered_map<string,double> *weight, vector<Score *> *ranking);
	void CreateRanking(vector<Score *> *ranking);
	vector<int> GetTopN(int n, vector<Score *> *ranking);
	void DeleteRanking(vector<Score *> *ranking);
};

#endif
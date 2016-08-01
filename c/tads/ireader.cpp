#include "ireader.h"

Ireader::Ireader(string filename)
{
	path_ref = filename + ".ref";
	path_dat = filename + ".dat";

	references = new unordered_map<string,double>();
}

Ireader::~Ireader()
{
	delete references;
}

void Ireader::Process()
{
	ReadReferences();
}

void Ireader::ReadReferences()
{
	string line;
	vector<string> v;
	fstream fs(path_ref, ios::in);

	if(!fs.is_open())
	{
		cout << "Error opening file: " << path_ref << endl;
		exit(1);
	}

	while(getline(fs,line))
	{
		v = Split(line, ' ');
		//TODO:: see how to convert string to double
		references->insert(pair<string,double>(v[0], atoi(v[1].c_str())));
	}

	fs.close();
}

Term* Ireader::GetStruct(string term)
{
	//TODO:: Must see a way of dealloc Term pointers after this method
	
	unordered_map<string,double>::iterator it = references->find(term);
	Term *f = NULL;
	Doc *inf, *cur;

	
	if(it != references->end())
	{
		fstream fs(path_dat, ios::in | ios::binary);
		double pos = it->second;

		// f = (struct feature*)malloc(sizeof(struct feature));
		f = new Term();
		fs.seekg(pos);
		fs.read((char*)f, sizeof(Term));
		// fs.read(reinterpret_cast<char *>(f),sizeof(Term));
		
		inf = new Doc();
		fs.read((char*)inf, sizeof(Doc));
		f->document = inf;

		while(inf->next != NULL)
		{
			cur = new Doc();
			fs.read((char*)cur, sizeof(Doc));
			inf->next = cur;
			inf = cur;
		}

		fs.close();

		return f;
	}

	return f;
}

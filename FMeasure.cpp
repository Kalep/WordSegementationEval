#include <string>
#include <vector>
#include <iostream>
#include <strstream>
#include <fstream>

using namespace std;

struct KBestItem
{
	string target;
	string feature;
};

void CompuPR(const bool & tagged, const string &ref, const string &tst, int &refCnt, int &tstCnt, int &intCnt);

int main(int argc,char *argv[])
{
	if(argc != 4)
	{
		cout<<"Usage: ./fmeasure tagflag(y/n) ref-file tst-file"<<endl;
		exit(0);
	}
	
	ifstream fRef(argv[2]);
	if (!fRef)
	{
		cout << "Can not open ref-file" << endl;
		exit(1);
	}

	ifstream fTst(argv[3]);
	if (!fTst)
	{
		cout << "Can not open tst-file" << endl;
		exit(1);
	}

    string flag = argv[1];
    bool tagged = flag == "y" || flag == "Y";   

	vector<string> refSet;
	string line;

	while (getline(fRef, line))
	{
		if (line.length() > 0)
		{
			refSet.push_back(line);
		}
	}

	fRef.close();

	vector<string> tstSet;

	while (getline(fTst, line))
	{
		if (line.length() > 0)
		{
			tstSet.push_back(line);
		}
	}

	fTst.close();

	if (refSet.size() != tstSet.size())
	{
		cout << "ref-set and tst-set do not match" << endl;
		exit(1);
	}

	int totalRefCnt = 0;
	int totalTstCnt = 0;
	int totalIntCnt = 0;

	int sentCnt = (int)refSet.size();
	int i;

	for (i = 0; i < sentCnt; ++i)
	{
		int refCnt, tstCnt, intCnt;

		CompuPR(tagged, refSet[i], tstSet[i], refCnt, tstCnt, intCnt);

        if (refCnt <= 1 || tstCnt <= 1)
        {
            continue;    
        }

		totalRefCnt += refCnt;
		totalTstCnt += tstCnt;
		totalIntCnt += intCnt;
	}
		
	double P = (double)totalIntCnt / (double)totalTstCnt;
	double R = (double)totalIntCnt / (double)totalRefCnt;
	double F = 2 * P * R / (P + R);

	cerr << "PRECISION:\t " << P << endl;
	cerr << "RECALL:\t " << R << endl;
	cerr << "F-MEASURE:\t " << F << endl;

	return 0;
}	

void CompuPR(const bool &tagged, const string &ref, const string &tst, int &refCnt, int &tstCnt, int &intCnt)
{
    vector<pair<int, string> > refParts;
    istrstream isRef(ref.c_str());
    string part;
    int pos = 0;

    while (isRef >> part)
    {
        if (tagged)
        {
            size_t split = part.rfind('/');

            refParts.push_back(pair<int, string>(pos, part));
            pos += (int)split;
        }
        else
        {
            size_t split = part.rfind('/');

            if (split == string::npos)
            {
                refParts.push_back(pair<int, string>(pos, part));
                pos += (int)part.length();
            }
            else
            {
                string word = part.substr(0, split);

                refParts.push_back(pair<int, string>(pos, word));
                pos += (int)split;
            }
        }
    }

    vector<pair<int, string> > tstParts;
    istrstream isTst(tst.c_str());

    pos = 0;

    while (isTst >> part)
    {
        if (tagged)
        {
            size_t split = part.rfind('/');
            
            tstParts.push_back(pair<int, string>(pos, part));
            pos += (int)split;
        }
        else
        {
            size_t split = part.rfind('/');

            if (split == string::npos)
            {
                tstParts.push_back(pair<int, string>(pos, part));
                pos += (int)part.length();
            }
            else
            {
                string word = part.substr(0, split);

                tstParts.push_back(pair<int, string>(pos, word));
                pos += (int)split;
            }
        }
    }

    refCnt = (int)refParts.size();
    tstCnt = (int)tstParts.size();
    intCnt = 0;
    int i = 0, j = 0;

    while (i < refCnt && j < tstCnt)
    {
        if (refParts[i].first == tstParts[j].first)
        {
            if (refParts[i].second == tstParts[j].second)
            {
                ++intCnt;
            }
			else
			{
				;
			}
            ++i;
            ++j;
        }
        else if (refParts[i].first < tstParts[j].first)
        {
            ++i;
        }
        else
        {
            ++j;
        }
    }
}


#include <iostream>
#include "iforest.h"
#include "data.h"
#include <fstream> 
#include <sstream>
#include <math.h>
#include <string>
#include <algorithm>
#include <random>
#include <cstdlib>
using namespace std;


vector<double> stringDecoder(string& line)
{
    vector<double> elements;
    string word="";

    for(int i=0; i<line.size(); i++)
    {
        if(line[i]==' ' || line[i]=='\t')
        {
            if(word=="") continue;
            else
            {
                elements.push_back(stod(word));
                word="";
            }
        }
        else
        {
            word.push_back(line[i]);
        }
    }

    if(word!="")
    {
        elements.push_back(stod(word));
    }
    return elements;
}


void readCoordinates(string const& fileName, vector<vector<double>>& inputMatrix, vector<int>& actualLabel)
{
    fstream fin;
    fin.open(fileName, ios::in);
    string line;
    
    getline(fin, line);
    stringstream s(line);
    string word;

    getline(s, word, ' ');
    int instances = stoi(word);
    getline(s, word, ' ');
    int dimensions = stoi(word);

    inputMatrix.resize(instances, vector<double>(dimensions));
    actualLabel.resize(instances);
 
    for(int i=0; i<instances; i++)
    {
        getline(fin, line);
        vector<double> elements = stringDecoder(line);
        // cout<<elements[0]<<" ";

        for(int j=0; j<dimensions; j++)
        {
            inputMatrix[i][j] = elements[j+1];
            // cout<<inputMatrix[i][j]<<' ';
        }

        actualLabel[i] = int(elements[elements.size()-1]);
        // cout<<actualLabel[i]<<endl;
    }
}

void readLabels(string const& fileName, vector<int>& actualLabel)
{
    fstream fin;
    fin.open(fileName, ios::in);
    string line;
    
    getline(fin, line);
    stringstream s(line);
    string word;

    getline(s, word, ' ');
    int instances = stoi(word);
    getline(s, word, ' ');
    int dimensions = stoi(word);

    actualLabel.resize(instances);
 
    for(int i=0; i<instances; i++)
    {
        getline(fin, line);
        vector<double> elements = stringDecoder(line);

        actualLabel[i] = int(elements[elements.size()-1]);
        // cout<<actualLabel[i]<<endl;
    }
}

double getEuclideanDistance(vector<vector<double>>& inputMatrix, int index1, int index2)
{
    double dis=0.0;

    for(int i=0; i<inputMatrix[0].size(); i++)
    {
        dis+=(inputMatrix[index1][i]-inputMatrix[index2][i]) * (inputMatrix[index1][i]-inputMatrix[index2][i]);
    }

    return sqrt(dis);
}

void getDisMatrix(vector<vector<double>>& disMatrix, const string& filename, vector<int>& actualLabel)
{
    string disMatrixFileName = "disMatrix/disMatrix_"+filename.substr(10);
    string actualLabelFileName = "actualLabel/"+filename.substr(10);
    
    fstream fin;
    fin.open(disMatrixFileName, ios::in);

    if(fin.fail())
    {
        vector<vector<double>> inputMatrix;
        readCoordinates(filename, inputMatrix, actualLabel);

        int numPoints = inputMatrix.size();
        disMatrix.resize(numPoints, vector<double> (numPoints));

        for(int i=0; i<numPoints; i++)
        {
            for(int j=0; j<numPoints; j++)
            {
                disMatrix[i][j]=getEuclideanDistance(inputMatrix, i, j);
            }
        }

        fstream fout;
        fout.open(disMatrixFileName, ios::out);
        for(int i=0; i<disMatrix.size(); i++)
        {
            for(int j=0; j<disMatrix[i].size(); j++)
            {
                fout<<disMatrix[i][j]<<" ";
            }
            fout<<endl;
        }
        fout.close();

        fout.open(actualLabelFileName, ios::out);
        for(int i=0; i<actualLabel.size(); i++)
        {
            fout<<actualLabel[i]<<endl;
        }
        fout.close();
    }
    else
    {
        string line;

        while(getline(fin, line))
        {
            stringstream s(line);
            string word;
            vector<double> v;

            while(getline(s, word, ' '))
            {
                v.push_back(stod(word));
            }
            disMatrix.push_back(v);
        }
        fin.close();

        fin.open(actualLabelFileName, ios::in);
        while(getline(fin, line))
        {
            actualLabel.push_back(stoi(line));
        }
    }

}

void getMassMatrix(vector<vector<double>>& disMatrix, const string& filename, vector<int>& actualLabel)
{
    string disMatrixFileName = "disMatrix/massMatrix_"+filename.substr(10);
    string actualLabelFileName = "actualLabel/"+filename.substr(10);
    
    fstream fin;
    fin.open(disMatrixFileName, ios::in);

    if(fin.fail())
    {
        readLabels(filename, actualLabel);

        data *dataObject = new data();
        dataObject->createDataVector(filename);

        int numOfTrees = 100;
        int minSampleSize = 256;
        int sampleSize;
        double samplingFactor = 0.01;

        int countOfCurrentPoints = dataObject->getnumInstances();
        sampleSize = countOfCurrentPoints * samplingFactor < minSampleSize ? minSampleSize :countOfCurrentPoints * samplingFactor;
        sampleSize = countOfCurrentPoints < sampleSize ? countOfCurrentPoints : sampleSize;

        iforest *iForestObject = new iforest(*dataObject, numOfTrees, sampleSize);
        iForestObject->constructiForest();
        iForestObject->computeNodeMass();
        iForestObject->computeMassMatrix(disMatrix);
    
        fstream fout;
        fout.open(disMatrixFileName, ios::out);
        for(int i=0; i<disMatrix.size(); i++)
        {
            for(int j=0; j<disMatrix[i].size(); j++)
            {
                fout<<disMatrix[i][j]<<" ";
            }
            fout<<endl;
        }
        fout.close();

        fout.open(actualLabelFileName, ios::out);
        for(int i=0; i<actualLabel.size(); i++)
        {
            fout<<actualLabel[i]<<endl;
        }
        fout.close();
    }
    else
    {
        string line;

        while(getline(fin, line))
        {
            stringstream s(line);
            string word;
            vector<double> v;

            while(getline(s, word, ' '))
            {
                v.push_back(stod(word));
            }
            disMatrix.push_back(v);
        }
        fin.close();

        fin.open(actualLabelFileName, ios::in);
        while(getline(fin, line))
        {
            actualLabel.push_back(stoi(line));
        }
    }
}


data::data()
{
    //ctor
}

data::~data()
{
    //dtor
}



/**************Setter methods*****************/


void data::setnumInstances(int numInstances){
    this->numInstances = numInstances;
}

void data::setnumAttributes(int numAttributes){
    this->numAttributes = numAttributes;
}

/**************Getter methods*****************/


int data::getnumInstances()const{
    return this->numInstances;
}

int data::getnumAttributes() const {
    return this->numAttributes;
}

vector<point*> data::getDataVector() const {
    const vector<point*> & refDataVector = this->dataVector;
    return dataVector;
}


/*******takes complete dataset File as input************/

void data::createDataVector(const string & dataFile){
	ifstream inDataFile(dataFile.c_str(),ios::in|ios::binary) ;
	if(!inDataFile){
		cout<<"Can not open input data file: "<<dataFile<<endl;
		exit(0);
	}
	inDataFile>>numInstances>>numAttributes;
	dataVector.resize(numInstances);
	for(int instance = 0; instance < numInstances; instance++){
		point *newPoint = new point();
		newPoint->attributes = new double[numAttributes];
		int id;
		inDataFile>>id;
		for(int attribute = 0; attribute < numAttributes; attribute++){
			inDataFile>>newPoint->attributes[attribute];
		}
		
		inDataFile>>newPoint->label;
		newPoint->isPresent = bool(1);
		dataVector[id] = newPoint;
	}
	inDataFile.close();
}


//***************************************Select sample from the original dataset i.e for static run****************************************//
vector<int> & data::getSample(int sampleSize) const {
	vector<int>* sample = new vector<int>;            
	vector<int> &refSample = *sample;
	int cnt = 0;
    int st_index = rand()%(numInstances-500);
	for(int i = st_index; i < min(st_index+3000, numInstances); i++){
        // cout<<i<<endl;
		if(dataVector[i]->isPresent){
			(*sample).push_back(i);
			cnt++;
		}
	}
    
	std::random_shuffle((*sample).begin(),(*sample).end());
	(*sample).erase((*sample).begin()+sampleSize,(*sample).begin()+cnt);
	return refSample;
}

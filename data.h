#ifndef DATA_H
#define DATA_H
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void getDisMatrix(vector<vector<double>>& , string const&, vector<int>&);
void getMassMatrix(vector<vector<double>>& , string const&, vector<int>&);

typedef struct point{
    double* attributes;
    bool isPresent;
    int label;
}point;

class data
{
    public:
        data();
        virtual ~data();
        void createDataVector(const string &);
		vector<int> & getSample(int) const ;
        vector<point*>  getDataVector() const ;
        int getnumInstances() const;
        void setnumInstances(int);
        int getnumAttributes() const;
        void setnumAttributes(int);
        
        vector<point*> dataVector;
        
    protected:

    private:
    int numInstances;
    int numAttributes;
};
#endif
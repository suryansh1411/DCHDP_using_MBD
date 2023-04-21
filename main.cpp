#include "dchdp.h"
#include "data.h"
#include "iforest.h"
#include <iostream>
#include <vector>
#include <fstream> 
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;


string double_to_string(double val)
{
    val *= 10;
    int n = (int) val;

    string ans=to_string(n);
    if(ans.length()==1)
    {
        return "0."+ans;
    }
    return ans.substr(0, ans.length()-1)+"."+ans.substr(ans.length()-1, 1);
}

int find(vector<int>& par, int x)
{
    if(par[x]==x) return x;
    return par[x]=find(par, par[x]);
}

void myunion(vector<int>& par, int x, int y, vector<int>& rank)
{
    int par_x = find(par, x);
    int par_y = find(par, y);

    if(rank[par_x]==rank[par_y])
    {
        par[par_x]=par_y;
        rank[par_y]++;
    }
    else if(rank[par_x]>rank[par_y])
    {
        par[par_y]=par_x;
    }
    else
    {
        par[par_x]=par_y;
    }
}

vector<int> merge(vector<pair<int, int>> mergers, int numClasses)
{
    vector<int> par(mergers.size()+1);
    vector<int> rank(mergers.size()+1, 0);
    for(int i=0; i<par.size(); i++)
    {
        par[i]=i;
    }
    // cout<<mergers.size()<<endl;

    for(int i=0; i<mergers.size()+1-numClasses; i++)
    {
        myunion(par, mergers[i].first, mergers[i].second, rank);    
    }
    
    for(int i=0; i<par.size(); i++)
    {
        par[i]=find(par, i);
    }
    return par;
}


int main(int argc, char* argv[])
{
    const string &dataFile = argv[1];
	const double &percent = stod(argv[2]);
	const int &minpts = atoi(argv[3]);
    const int &numClasses = atoi(argv[4]);

////////////////

    struct timespec start1,end1;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);

    vector<int> actualLabel;
    vector<vector<double>> disMatrix;

    // getDisMatrix(disMatrix, dataFile, actualLabel);
    getMassMatrix(disMatrix, dataFile, actualLabel);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
    double disMatTime =  (((end1.tv_sec - start1.tv_sec) * 1e9)+(end1.tv_nsec - start1.tv_nsec))*1e-9;

    // fstream hehe;

    // hehe.open("simMatrix.csv", ios::out);
    // for(int i=0; i<disMatrix.size(); i++)
    // {
        // cout<<disMatrix[i].size()<<endl;
        // for(int j=0; j<disMatrix[i].size(); j++)
        // {
        //     cout<<disMatrix[i][j]<<" ";
        // }
        // cout<<endl<<endl;
    // }
////////////////              DCHDP

    // struct timespec start2,end2;
    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start2);

    DCHDP x;
    // vector<vector<int>> dendrogram = x.cluster(disMatrix, percent, minpts);

    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end2);
    // double clusterTime =  (((end2.tv_sec - start2.tv_sec) * 1e9)+(end2.tv_nsec - start2.tv_nsec))*1e-9;


    struct timespec start3,end3;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start3);
    
    vector<pair<int, int>> mergers = x.cluster2(disMatrix, percent, minpts);
    
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end3);
    double clusterTime2 =  (((end3.tv_sec - start3.tv_sec) * 1e9)+(end3.tv_nsec - start3.tv_nsec))*1e-9;

    vector<int> clusters = merge(mergers, numClasses);
    
    // cout<<clusterTime<<"\t"<<clusterTime2<<endl;
    // for(int i=0; i<clusters.size(); i++)
    // {
        // cout<<clusters[i]<<endl;
    // }
////////////////

    fstream fout;
    string outputFileName = "clusterings/dchdp_" + dataFile.substr(10, dataFile.length()-14) + "_" + double_to_string(percent) + "_" + to_string(minpts) + "_" + to_string(numClasses) + ".csv";
    
    fout.open(outputFileName, ios::out);
    fout<<"actual"<<"\t"<<"predicted"<<endl;
    // for(int i=0; i<dendrogram.size(); i++)
    // {
    //     fout<<actualLabel[i]<<"\t"<<dendrogram[dendrogram.size()-numClasses][i]<<endl;
    // }
    for(int i=0; i<clusters.size(); i++)
    {
        fout<<actualLabel[i]<<"\t"<<clusters[i]<<endl;
    }

    fout.close();

/////////////////

    cout<<"Distance Matrix Cal Time : "<<disMatTime<<endl;
    cout<<"Clustering Time : "<<clusterTime2<<endl;

    // for(int i=0; i<dendrogram.size(); i++)
    // {
    //     for(int j=0; j<dendrogram[0].size(); j++)
    //     {
    //         cout<<dendrogram[i][j]<<" ";
    //     }
    //     cout<<endl<<endl;
    // }
}
// ////////////////////           DBSCAN

//     // Obtain dc
//     int numPoints = disMatrix.size();
//     vector<double> distances;
    
//     for(int i=0; i<numPoints; i++)
//     {
//         for(int j=i+1; j<numPoints; j++)
//         {
//             distances.push_back(disMatrix[i][j]);
//         }
//     }
//     sort(distances.begin(), distances.end());

//     int pos = ceil((percent*(double)distances.size())/100.0);
//     double dc = distances[pos];

//     DCHDP x;
//     vector<int> labels = x.checkConnectivity(disMatrix, dc, minpts);

//     fstream fout;
//     string outputFileName = "clusterings/dbscan_" + dataFile.substr(10, dataFile.length()-14) + "_" + double_to_string(percent) + "_" + to_string(minpts) + ".csv";
//     cout<<outputFileName<<endl;

//     fout.open(outputFileName, ios::out);
//     fout<<"actual"<<"\t"<<"predicted"<<endl;
//     for(int i=0; i<labels.size(); i++)
//     {
//         fout<<actualLabel[i]<<"\t"<<labels[i]<<endl;
//     }
//     fout.close();
// }
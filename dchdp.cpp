#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <fstream> 
#include "dchdp.h"
using namespace std;


double inline readMatrix(vector<vector<double>>& disMatrix, int x, int y)
{
    int a = min(x, y);
    int b = max(x, y);
    return disMatrix[b][a];
}

DCHDP::DCHDP()
{
    // cout<<"Hi from DCHDP constructor"<<endl;
}

DCHDP::~DCHDP()
{
    // cout<<"Hi from DCHDP destructor"<<endl;
}

void normalise(vector<double>& v)
{
    double sum = 0.0;
    double mean = 0.0;
    double stdDev = 0.0;

    for(int i=0; i<v.size(); i++)
    {
        sum+=v[i];
    }

    mean = sum/v.size();

    for(int i=0; i<v.size(); i++)
    {
        stdDev += (v[i]-mean)*(v[i]-mean);
    }
    stdDev /= v.size();
    stdDev = sqrt(stdDev);

    for(int i=0; i<v.size(); i++)
    {
        v[i]-=mean;
        v[i]/=stdDev;
    }
}

vector<int> getRegion(int index, vector<vector<double>>& disMatrix, double dc)
{
    vector<int> neighbors;
    for(int i=0; i<disMatrix.size(); i++)
    {
        if(i==index) continue;
        if(readMatrix(disMatrix, i, index) < dc)
        {
            neighbors.push_back(i);
        }
    }

    return neighbors;
}

vector<int> DCHDP::checkConnectivity(vector<vector<double>>& disMatrix, double dc, int minPts)
{
    int numPoints=disMatrix.size();
    vector<int> label(numPoints, -1);
    int numClusters=0;

    for(int i=0; i<numPoints; i++)
    {
        if(label[i]==-1)
        {
            vector<int> neighbors = getRegion(i, disMatrix, dc);

            if(neighbors.size()>=minPts)
            {
                numClusters++;
                label[i]=numClusters;
                vector<bool> vis(numPoints, false);
                vis[i]=true;
                for(int j=0; j<neighbors.size(); j++)
                {
                    int q = neighbors[j];
                    label[q]=numClusters;

                    vector<int> q_neighbors = getRegion(q, disMatrix, dc);
                    if (q_neighbors.size() >= minPts) {
                        for(int k=0; k<q_neighbors.size(); k++)
                        {
                            if(label[q_neighbors[k]]==-1 && vis[q_neighbors[k]]==false)
                            {
                                neighbors.push_back(q_neighbors[k]);
                                vis[q_neighbors[k]]=true;
                            }
                        }
                    }
                }
            }
            
        }
    }

    return label;

    // vector<int> label;

    // string line;
    // fstream hehe;
    // hehe.open("LC7.csv", ios::in);
    // while(getline(hehe, line))
    // {
    //     label.push_back(stoi(line));
    // }

    // return label;
}

double getDensity(int index, vector<vector<double>>& disMatrix, double dc)
{
    double density=0.0;
    for(int i=0; i<disMatrix.size(); i++)
    {
        if(readMatrix(disMatrix, index, i) <= dc) density++;
    }

    return density;
}

vector<vector<int>> DCHDP::cluster(vector<vector<double>>& disMatrix, double percent, int minPts)
{
    //Obtain dc
    int numPoints = disMatrix.size();
    vector<double> distances;
    
    for(int i=0; i<numPoints; i++)
    {
        for(int j=i+1; j<numPoints; j++)
        {
            distances.push_back(readMatrix(disMatrix, i, j));
        }
    }
    sort(distances.begin(), distances.end());

    int pos = ceil((percent*(double)distances.size())/100.0);
    double dc = distances[pos];
    double M = distances[distances.size()-1];
    // cout<<dc<<endl;
    
    // Check density connectivity of points
    vector<int> connected = checkConnectivity(disMatrix, dc, minPts);
    // fstream hehe;
    // hehe.open("label.csv", ios::out);
    // for(int i=0; i<connected.size(); i++)
    // {
    //     hehe<<connected[i]<<endl;
    // }
    

    // Obtain rho
    vector<double> density(numPoints);
    vector<pair<double, int>> sorted_density(numPoints);
    for(int i=0; i<numPoints; i++)
    {
        density[i]=getDensity(i, disMatrix, dc);
        // cout<<density[i]<<endl;
        sorted_density[i] = {density[i], -1*i};
    } 
    
    // fstream hehe;
    // hehe.open("density.csv", ios::out);
    // for(int i=0; i<density.size(); i++)
    // {
    //     hehe<<density[i]<<endl;
    // }


    // Obtain ita' and delta'
    sort(sorted_density.begin(), sorted_density.end());
    reverse(sorted_density.begin(), sorted_density.end());
    for(int i=0; i<density.size(); i++)
    {
        sorted_density[i].second*=-1;
        // cout<<sorted_density[i].first<<" "<<sorted_density[i].second<<endl;
    }

    vector<int> nearestNeighbor(numPoints, -1);
    vector<double> nearestNeighborDistance(numPoints, 99999.0);
    double nnd_0 = 0.0;

    for(int i=1; i<numPoints; i++)
    {
        int point1 = sorted_density[i].second;

        for(int j=0; j<i; j++)
        {
            int point2 = sorted_density[j].second;

            if(connected[point2] == connected[point1])
            {
                if(readMatrix(disMatrix, point1, point2) < nearestNeighborDistance[point1])
                {
                    nearestNeighbor[point1] = point2;
                    nearestNeighborDistance[point1] = readMatrix(disMatrix, point1, point2);
                }
            }
        }

        if(nearestNeighbor[point1]!=-1)
        {
            nnd_0 = max(nnd_0, nearestNeighborDistance[point1]);
        }
        else 
        {
            nearestNeighborDistance[point1]=0.0;
        }
    }

    nearestNeighborDistance[sorted_density[0].second]=nnd_0;
    // for(int i=0; i<nearestNeighborDistance.size(); i++)
    // {
    //     cout<<nearestNeighborDistance[i]<<endl;
    // }

    //Normalise nnd and den
    normalise(density);
    normalise(nearestNeighborDistance);

    // Calculate gamma'
    vector<double> gamma(numPoints, 0.0);
    for(int i=0; i<numPoints; i++)
    {
        // cout<<density[i]<<" "<<nearestNeighborDistance[i]<<endl;
        gamma[i] = density[i]*nearestNeighborDistance[i];
        // cout<<gamma[i]<<endl;
    }

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    for(int i=0; i<numPoints; i++)
    {
        pq.push({gamma[i], i});
    }
    
    // Store labels after every merger
    vector<vector<int>> dendrogram(numPoints, vector<int>(numPoints));
    for(int i=0; i<numPoints; i++)
    {
        dendrogram[0][i]=i;
    }
    int mergers=0;
    vector<int> leftouts;

    while(pq.empty()==false)
    {
        pair<double, int> tmp = pq.top();
        pq.pop();
        // cout<<tmp.second<<endl;

        int point1 = tmp.second;
        if(nearestNeighbor[point1]==-1)
        {
            leftouts.push_back(point1);
        }
        else
        {
            // double Nscore = (nearestNeighborDistance[point1]*(double)density[point1])/M;
            // cout<<Nscore<<endl;
            int point2 = nearestNeighbor[point1];

            int label1 = dendrogram[mergers][point1];
            int label2 = dendrogram[mergers][point2];

            int newLabel = min(label1, label2);

            for(int i=0; i<numPoints; i++)
            {
                dendrogram[mergers+1][i] = dendrogram[mergers][i];
                if(dendrogram[mergers+1][i]==label1 || dendrogram[mergers+1][i]==label2)
                {
                    dendrogram[mergers+1][i]=newLabel;
                }
            }

            mergers++;
        }
    }

    // for(int i=0; i<leftouts.size(); i++)
    // {
    //     cout<<leftouts[i]<<endl;
    // }

    for(int i=1; i<leftouts.size(); i++)
    {
        int label1 = dendrogram[mergers][leftouts[i-1]];
        int label2 = dendrogram[mergers][leftouts[i]];

        int newLabel = min(label1, label2);

        for(int i=0; i<numPoints; i++)
        {
            dendrogram[mergers+1][i] = dendrogram[mergers][i];
            if(dendrogram[mergers+1][i]==label1 || dendrogram[mergers+1][i]==label2)
            {
                dendrogram[mergers+1][i]=newLabel;
            }
        }

        mergers++;        
    }

    return dendrogram;

}







vector<pair<int, int>> DCHDP::cluster2(vector<vector<double>>& disMatrix, double percent, int minPts)
{
    //Obtain dc
    int numPoints = disMatrix.size();
    vector<double> distances;
    
    for(int i=0; i<numPoints; i++)
    {
        for(int j=i+1; j<numPoints; j++)
        {
            distances.push_back(readMatrix(disMatrix, i, j));
        }
    }
    sort(distances.begin(), distances.end());

    int pos = ceil((percent*(double)distances.size())/100.0);
    double dc = distances[pos];
    double M = distances[distances.size()-1];
    // cout<<dc<<endl;
    
    // Check density connectivity of points
    vector<int> connected = checkConnectivity(disMatrix, dc, minPts);
    // fstream hehe;
    // hehe.open("label.csv", ios::out);
    // for(int i=0; i<connected.size(); i++)
    // {
    //     hehe<<connected[i]<<endl;
    // }
    

    // Obtain rho
    vector<double> density(numPoints);
    vector<pair<double, int>> sorted_density(numPoints);
    for(int i=0; i<numPoints; i++)
    {
        density[i]=getDensity(i, disMatrix, dc);
        // cout<<density[i]<<endl;
        sorted_density[i] = {density[i], -1*i};
    } 
    
    // fstream hehe;
    // hehe.open("density.csv", ios::out);
    // for(int i=0; i<density.size(); i++)
    // {
    //     hehe<<density[i]<<endl;
    // }


    // Obtain ita' and delta'
    sort(sorted_density.begin(), sorted_density.end());
    reverse(sorted_density.begin(), sorted_density.end());
    for(int i=0; i<density.size(); i++)
    {
        sorted_density[i].second*=-1;
        // cout<<sorted_density[i].first<<" "<<sorted_density[i].second<<endl;
    }

    vector<int> nearestNeighbor(numPoints, -1);
    vector<double> nearestNeighborDistance(numPoints, 99999.0);
    double nnd_0 = 0.0;

    for(int i=1; i<numPoints; i++)
    {
        int point1 = sorted_density[i].second;

        for(int j=0; j<i; j++)
        {
            int point2 = sorted_density[j].second;

            if(connected[point2] == connected[point1])
            {
                if(readMatrix(disMatrix, point1, point2) < nearestNeighborDistance[point1])
                {
                    nearestNeighbor[point1] = point2;
                    nearestNeighborDistance[point1] = readMatrix(disMatrix, point1, point2);
                }
            }
        }

        if(nearestNeighbor[point1]!=-1)
        {
            nnd_0 = max(nnd_0, nearestNeighborDistance[point1]);
        }
        else 
        {
            nearestNeighborDistance[point1]=0.0;
        }
    }

    nearestNeighborDistance[sorted_density[0].second]=nnd_0;
    // for(int i=0; i<nearestNeighborDistance.size(); i++)
    // {
    //     cout<<nearestNeighborDistance[i]<<endl;
    // }

    //Normalise nnd and den
    normalise(density);
    normalise(nearestNeighborDistance);

    // Calculate gamma'
    vector<double> gamma(numPoints, 0.0);
    for(int i=0; i<numPoints; i++)
    {
        // cout<<density[i]<<" "<<nearestNeighborDistance[i]<<endl;
        gamma[i] = density[i]*nearestNeighborDistance[i];
        // cout<<gamma[i]<<endl;
    }

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    for(int i=0; i<numPoints; i++)
    {
        pq.push({gamma[i], i});
    }
    
    // Store labels after every merger
    // vector<vector<int>> dendrogram(numPoints, vector<int>(numPoints));
    // for(int i=0; i<numPoints; i++)
    // {
    //     dendrogram[0][i]=i;
    // }
    // int mergers=0;
    // vector<int> leftouts;

    // while(pq.empty()==false)
    // {
    //     pair<double, int> tmp = pq.top();
    //     pq.pop();
    //     // cout<<tmp.second<<endl;

    //     int point1 = tmp.second;
    //     if(nearestNeighbor[point1]==-1)
    //     {
    //         leftouts.push_back(point1);
    //     }
    //     else
    //     {
    //         // double Nscore = (nearestNeighborDistance[point1]*(double)density[point1])/M;
    //         // cout<<Nscore<<endl;
    //         int point2 = nearestNeighbor[point1];

    //         int label1 = dendrogram[mergers][point1];
    //         int label2 = dendrogram[mergers][point2];

    //         int newLabel = min(label1, label2);

    //         for(int i=0; i<numPoints; i++)
    //         {
    //             dendrogram[mergers+1][i] = dendrogram[mergers][i];
    //             if(dendrogram[mergers+1][i]==label1 || dendrogram[mergers+1][i]==label2)
    //             {
    //                 dendrogram[mergers+1][i]=newLabel;
    //             }
    //         }

    //         mergers++;
    //     }
    // }

    // // for(int i=0; i<leftouts.size(); i++)
    // // {
    // //     cout<<leftouts[i]<<endl;
    // // }

    // for(int i=1; i<leftouts.size(); i++)
    // {
    //     int label1 = dendrogram[mergers][leftouts[i-1]];
    //     int label2 = dendrogram[mergers][leftouts[i]];

    //     int newLabel = min(label1, label2);

    //     for(int i=0; i<numPoints; i++)
    //     {
    //         dendrogram[mergers+1][i] = dendrogram[mergers][i];
    //         if(dendrogram[mergers+1][i]==label1 || dendrogram[mergers+1][i]==label2)
    //         {
    //             dendrogram[mergers+1][i]=newLabel;
    //         }
    //     }

    //     mergers++;        
    // }

    // return dendrogram;


    vector<pair<int, int>> mergers;
    vector<int> leftouts;

    while(pq.empty()==false)
    {
        pair<double, int> tmp = pq.top();
        pq.pop();
        // cout<<tmp.second<<endl;

        int point1 = tmp.second;
        if(nearestNeighbor[point1]==-1)
        {
            leftouts.push_back(point1);
        }
        else
        {
            // double Nscore = (nearestNeighborDistance[point1]*(double)density[point1])/M;
            // cout<<Nscore<<endl;
            int point2 = nearestNeighbor[point1];
            mergers.push_back({point1, point2});
            // int label1 = dendrogram[mergers][point1];
    //         int label2 = dendrogram[mergers][point2];

    //         int newLabel = min(label1, label2);

    //         for(int i=0; i<numPoints; i++)
    //         {
    //             dendrogram[mergers+1][i] = dendrogram[mergers][i];
    //             if(dendrogram[mergers+1][i]==label1 || dendrogram[mergers+1][i]==label2)
    //             {
    //                 dendrogram[mergers+1][i]=newLabel;
    //             }
    //         }

    //         mergers++;
        }
    }

    // for(int i=0; i<leftouts.size(); i++)
    // {
    //     cout<<leftouts[i]<<endl;
    // }

    for(int i=1; i<leftouts.size(); i++)
    {
        // int label1 = dendrogram[mergers][leftouts[i-1]];
    //     int label2 = dendrogram[mergers][leftouts[i]];

    //     int newLabel = min(label1, label2);

    //     for(int i=0; i<numPoints; i++)
    //     {
    //         dendrogram[mergers+1][i] = dendrogram[mergers][i];
    //         if(dendrogram[mergers+1][i]==label1 || dendrogram[mergers+1][i]==label2)
    //         {
    //             dendrogram[mergers+1][i]=newLabel;
    //         }
    //     }
        mergers.push_back({leftouts[i], leftouts[i-1]});
        // mergers++;        
    }

    return mergers;
}
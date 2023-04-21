#ifndef IFOREST_H
#define IFOREST_H
#include <map>
#include <unordered_map>
#include "data.h"
#include "itree.h"

class iforest
{
    public:
	iforest(data & , int , int );
	virtual ~iforest();
	void fit();
	void constructiForest();
	void computeNodeMass();
	int computeLCA(int, int);
	void computeLCA_lookup();
	double dissScoreComputation(int point1,int point2);
	void computeMassMatrix(vector<vector<double>> &);
	void write_smallest_largest_leaf(const string &);
	
	

	private:    
	public:
    
    //private:
    int _numiTrees;										//number of iTrees in the iForest.
	int _sampleSize;									//sample size representing the iForest.
    int _maxTreeHeight;									//max Height of each iTree in iForest.
  	int _maxNumOfNodes;									//max number of node possible in each iTree.
	const data & _dataObject;
	vector<vector<int>> LCA_lookup;							//reference of the input dataObject, only a container not responsible for deletion of the object.
	vector<itree*> _iTrees;								//list of pointers to the iTrees in the forest.
};

#endif // IFOREST_H

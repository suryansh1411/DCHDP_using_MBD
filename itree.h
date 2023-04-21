#ifndef ITREE_H
#define ITREE_H
#include "data.h"
#include "treenode.h"
#include <vector>


class itree
{
    public:
        itree(const data &);
        itree(const data &,int, int, int);
        void constructiTree();
        void computeNodeMassforTree();
		void find_markedNodes(int, double, vector<vector<int>> &);
		treenode * rootNode;
		vector<int> _pointToNode;   //stores leaf node associated with each point in the dataset.
		vector<int> _pointTomarkedNode;
	vector<long double> _nodeMass;     //stores nodemass of nodes ordered as per the nodeIds.
		vector<treenode*> treeNodes;
		int smallest_leaf;
		int largest_leaf;
		virtual ~itree();
        
        
    //protected:

    //private:
	int _sampleSize;    
	int _maxTreeHeight;
    int _maxNumOfNodes;
	const data & _dataObject;
		
};

#endif // ITREE_H

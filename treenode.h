#ifndef TREENODE_H
#define TREENODE_H
# include "data.h"
#include <vector>
#include <boost/serialization/list.hpp>


class treenode
{
    public:

        int nodeId;
        int parentId;
        int lChildId;
        int rChildId;
        treenode *parentAdd;
        treenode *lChildAdd;
        treenode *rChildAdd;
        vector<int> dataPointIndices;
        int splitAttribute;
        double splitValue;
        int nodeSize;   
        int nodeHeight;
        bool isLeaf;
        int nodeMass;
		 

        treenode();
        treenode(int);
        virtual ~treenode();
		//double invertedCumulativeProbabilityFunction(double target)
        double splitInfoSelection(const data &);
		double PGIFsplitInfoSelection(const data &);
        void createLeftChild();
		void createRightChild();

    protected:

    private:
};

#endif // TREENODE_H

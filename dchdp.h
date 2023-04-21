#include <vector>
using namespace std;

class DCHDP{

public:
    DCHDP();
    virtual ~DCHDP();
    vector<vector<int>> cluster(vector<vector<double>>&, double, int);
    vector<pair<int, int>> cluster2(vector<vector<double>>&, double, int);
    // vector<int> getRegion(int, vector<vector<double>>& , double);
    vector<int> checkConnectivity(vector<vector<double>>& , double, int);
    
};

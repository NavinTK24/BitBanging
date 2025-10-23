#include<iostream>
#include<vector>
#include<map>
#include<variant>

using namespace std;
using intOrVector = variant<int, vector<int>>;
int total[3] = {0,0,0};

void printLoads(const map<int, vector<intOrVector>> &n) {
    for(auto &row : n) {
        cout<<"Phase["<<row.first<<"] = ";
        for(auto &element : row.second) {
            if(holds_alternative<int>(element)) {
                cout<<get<int>(element)<<",";
                total[row.first-1] += get<int>(element);
            } else {
                cout<<"{ ";
                for(int val : get<vector<int>>(element)) {
                    cout<<val<<",";
                }
                cout<<"}";
            }
        }
        cout<<endl;
    }
}

int main() {
    cout<<"Hello World\n";
    
    map<int, vector<intOrVector>> n;
    n[1] = {4,3,2,4,0,7};
    n[2] = {3,2,1,1,2,7};
    n[3] = {6,3,2,1,0,5};

    for(int i=0; i<n[1].size(); i++) {
        if (holds_alternative<int>(n[1][i]) && get<int>(n[1][i]) == 7) {
            if (holds_alternative<int>(n[3][i]) && get<int>(n[3][i]) == 0) {
                // Just replace directly
                n[3][i] = get<int>(n[1][i]);
            } else {
                // Make it nested
                vector<int> nestedVec;
                nestedVec.push_back(get<int>(n[3][i]));
                nestedVec.push_back(get<int>(n[1][i]));
                n[3][i] = nestedVec;
            }
            // After moving, set n[1][i] to 0 (for clarity)
            n[1][i] = 0;
        }
    }

    printLoads(n);
    cout<<total[0];

}
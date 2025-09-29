#include<iostream>
#include<vector>
using namespace std;
int main() {
    vector<int> array;
    array.push_back(5);
    array.push_back(2);
    array.push_back(2);
    array.push_back(3);
    int target = 4;
    int n = target;
    int state = true;
    int m=0;
    while(state) {
        bool status = true;
        for(int i=0; i<array.size(); i++) {
            if(array[i]==n) {
                cout<<array[i]<<endl;
                array.erase(array.begin() + i);
                status = false;
                if(m<target) {
                    m+=n;
                    n = target-m;
                }

            }
        }
        if(status) {
            n=n-1;
        }
        if(n<=0) {
            state=false;
        }
    }

} 
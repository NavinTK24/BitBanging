#include<iostream>
using namespace std;
int main() {
    int array[5] = {1,2,3,4,5};
    int target = 6;
    bool state = false;

    while(!state) {
        for(int i=0; i<5; i++) {
            if(array[i] == target) {
                cout<<"target found at "<<i<<endl;
                state = true;
                break;
            }
        }
        if(!state) {
            target = target-1;
            cout<<"doing target -1"<<endl;
        }

    }
} 
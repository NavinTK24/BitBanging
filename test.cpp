#include<iostream>
using namespace std;
int main() {
    int array[5] = {1,2,3,4,5};
    int target = 6;
    int n = target;
    bool state = false;

    while (!state) {
        for(int i=0; i<5; i++) {
            if(array[i] == n) {
                cout<<array[i]<<endl;           
                state = true;          
                break;
            }
        }
        if(state) {
            int m=n;
            n = target-n;
            m += n;
            state = false;
            if(m<=0) {
                state = true;
                break;
            }
        } else {
            n = n-1;
        }  
        
    }
    
} 
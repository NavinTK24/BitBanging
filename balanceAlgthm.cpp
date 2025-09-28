#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

int main() {
    cout<<"Hello World"<<endl;
    vector<int> Rphase = {0,4,3,4,2,1,2};
    vector<int> Yphase = {3,2,2,1,3,4,2};
    vector<int> Bphase = {0,0,2,2,1,3,1};

    int totalLoad = 0;
    int Rtotal=0, Ytotal=0, Btotal=0;
    
    for(int i=0; i<Rphase.size(); i++) {
        totalLoad += Rphase[i] + Yphase[i] + Bphase[i]; //42
        Rtotal += Rphase[i]; //16
        Ytotal += Yphase[i]; //17
        Btotal += Bphase[i]; //09
    }

    int Rbalanced = totalLoad/3; //14 //14 //14
    int Ybalanced = (totalLoad - (Rbalanced))/2;
    int Bbalanced = (totalLoad - (Rbalanced+Ybalanced));

    cout<<"R phase Load: "<<setfill('0')<<setw(2)<<Rtotal<<"A :: R balanced state: "<<Rbalanced<<"A"<<endl;
    cout<<"Y phase Load: "<<setfill('0')<<setw(2)<<Ytotal<<"A :: Y balanced state: "<<Ybalanced<<"A"<<endl;
    cout<<"B phase Load: "<<setfill('0')<<setw(2)<<Btotal<<"A :: B balanced state: "<<Bbalanced<<"A"<<endl;
    cout<<endl;

    int Rneeds = Rbalanced - Rtotal;
    int Yneeds = Ybalanced - Ytotal;
    int Bneeds = Bbalanced - Btotal;
    


    return 0;
}
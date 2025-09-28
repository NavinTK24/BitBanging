#include<iostream>
#include<vector>
#include<map>
#include<iomanip>
using namespace std;

int main() {
    cout<<"Hello World"<<endl;
    map<int, vector<int>> phaseLoads;

    phaseLoads[1] = {0,4,3,4,2,1,0};
    phaseLoads[2] = {3,2,2,1,3,4,2};
    phaseLoads[3] = {0,0,2,2,1,3,1};

    for(int i=1; i<4; i++) {
        cout<<"Phase "<<i<<": [";
        for(int j: phaseLoads[i]) {
            cout<<j<<",";
        }
        cout<<"]"<<endl;
    } cout<<endl;

    int phaseTotals[3] = {0,0,0};
    
    for(int i=0; i<phaseLoads[1].size(); i++) {
        phaseTotals[0] += phaseLoads[1][i]; //16
        phaseTotals[1] += phaseLoads[2][i]; //17
        phaseTotals[2] += phaseLoads[3][i]; //09
    }
    
    int totalLoad = phaseTotals[0] + phaseTotals[1] + phaseTotals[2];
    int balanced[3];
    balanced[0] = totalLoad/3;
    balanced[1] = (totalLoad - (balanced[0]))/2;
    balanced[2] = (totalLoad - (balanced[0]+balanced[1]));

    int phaseNeeds[3];

    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(phaseTotals[i] == balanced[j]) {
                int n = balanced[i];
                balanced[i] =  balanced[j];
                balanced[j] = n;
            } 
        }
    }

    for(int i=0; i<3; i++) {
        phaseNeeds[i] = balanced[i] - phaseTotals[i];
    }

    cout<<"R phase Load: "<<setfill('0')<<setw(2)<<phaseTotals[0]<<"A :: R balanced state: "<<balanced[0]<<"A"<<" :: To Balance: "<<setfill('0')<<setw(2)<<phaseNeeds[0]<<"A"<<endl;
    cout<<"Y phase Load: "<<setfill('0')<<setw(2)<<phaseTotals[1]<<"A :: Y balanced state: "<<balanced[1]<<"A"<<" :: To Balance: "<<setfill('0')<<setw(2)<<phaseNeeds[1]<<"A"<<endl;
    cout<<"B phase Load: "<<setfill('0')<<setw(2)<<phaseTotals[2]<<"A :: B balanced state: "<<balanced[2]<<"A"<<" :: To Balance: "<<setfill('0')<<setw(2)<<phaseNeeds[2]<<"A"<<endl;
    cout<<endl;

    vector<int> negatives;
    vector<int> positives;
    for(int i=0; i<3; i++) { 
        if(phaseNeeds[i]<0) {
            negatives.push_back(i);
        } 
        if(phaseNeeds[i]>0) {
            positives.push_back(i);
        }
    }

    for(int p: positives) {
        for(int n: negatives) {
            int target = phaseNeeds[p];
            int state = false;

            while(!state) {
                if(target>=(-phaseNeeds[n])) {
                    int toGive = -phaseNeeds[n];
                    for(int i=0; i<phaseLoads[1].size(); i++) {
                        if(phaseLoads[n+1][i]== toGive) {
                            cout<<"Shift "<<toGive<<"A load from ";
                            if((n+1) == 1) cout<<"R Phase";
                            else if((n+1) == 2) cout<<"Y Phase";
                            else cout<<"B phase";
                            cout<<" to ";
                            if((p+1) == 1) cout<<"R Phase";
                            else if((p+1) == 2) cout<<"Y Phase";
                            else cout<<"B phase";
                            cout<<endl;

                            state = true;
                            phaseNeeds[n] = phaseNeeds[n] + phaseLoads[n+1][i];
                            phaseNeeds[p] = phaseNeeds[p]-toGive;
                            break;
                        }
                    }
                    if(!state && (toGive>0)) {
                        toGive = toGive-1;
                    }
                   
                }

                if(target<(-phaseNeeds[n])) {
                    int toGive = target;
                    for(int i=0; i<phaseLoads[1].size(); i++) {
                        if(phaseLoads[n+1][i]== toGive) {
                            cout<<"Shift "<<toGive<<"A load from ";
                            if((n+1) == 1) cout<<"R Phase";
                            else if((n+1) == 2) cout<<"Y Phase";
                            else cout<<"B phase";
                            cout<<" to ";
                            if((p+1) == 1) cout<<"R Phase";
                            else if((p+1) == 2) cout<<"Y Phase";
                            else cout<<"B phase";
                            cout<<endl;

                            state = true;
                            phaseNeeds[n] = phaseNeeds[n] + phaseLoads[n+1][i];
                            phaseNeeds[p] = phaseNeeds[p]-toGive;
                            break;
                        }
                    }
                    if(!state && (toGive>0)) {
                        toGive = toGive-1;
                    }
                   
                }

            }
           
        }
    }

    return 0;
}
#include<iostream>
#include<vector>
#include<map>
#include<variant>
#include<iomanip>
#include<algorithm>

using namespace std;
using intOrVector = variant<int, vector<int>>;

int phaseTotals[3] = {0,0,0};

void printLoads(const map<int, vector<intOrVector>> &phaseLoads) {
    for(auto &row : phaseLoads) {

        cout<<"Phase["<<row.first<<"] = ";
        for(auto &element : row.second) {

            if(holds_alternative<int>(element)) {
                cout<<get<int>(element)<<",";
                phaseTotals[row.first-1] += get<int>(element);
            } else {
                cout<<"{";
                for(int val : get<vector<int>>(element)) {
                    cout<<val<<",";
                    phaseTotals[row.first] += val;
                }
                cout<<"},";
            }
        }
        cout<<endl;
    }
}

int main() {
    cout<<"Hello World"<<endl;

    map<int, vector<intOrVector>>phaseLoads;

    phaseLoads[1] = {5,6,7,8,1,4};     //R
    phaseLoads[2] = {17,1,13,11,5,0};  //Y
    phaseLoads[3] = {3,6,9,12,1,0};    //B

    printLoads(phaseLoads);
    
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

    cout<<"R phase Load: "<<setfill('0')<<setw(2)<<phaseTotals[0]<<"A :: R balanced state: "<<balanced[0]<<"A"<<" :: To Balance it needs: "<<setfill('0')<<setw(2)<<phaseNeeds[0]<<"A"<<endl;
    cout<<"Y phase Load: "<<setfill('0')<<setw(2)<<phaseTotals[1]<<"A :: Y balanced state: "<<balanced[1]<<"A"<<" :: To Balance it needs: "<<setfill('0')<<setw(2)<<phaseNeeds[1]<<"A"<<endl;
    cout<<"B phase Load: "<<setfill('0')<<setw(2)<<phaseTotals[2]<<"A :: B balanced state: "<<balanced[2]<<"A"<<" :: To Balance it needs: "<<setfill('0')<<setw(2)<<phaseNeeds[2]<<"A"<<endl;
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
            int toGive = -phaseNeeds[n];
            int search = toGive;
            int state = true;
            int m = 0;
            
            if(phaseNeeds[p]<(-phaseNeeds[n])) {
                toGive = target;
                search = toGive; 
            }
            
            while(state) {
                bool status = true;

                for(int i=0; i<phaseLoads[n+1].size(); i++) {    
                    if(holds_alternative<int>(phaseLoads[n+1][i]) && get<int>(phaseLoads[n+1][i]) == search) {
                        
                        cout<<"Shift "<<get<int>(phaseLoads[n+1][i])<<"A load from ";
                        if((n+1) == 1) cout<<"R Phase";
                        else if((n+1) == 2) cout<<"Y Phase";
                        else cout<<"B phase";
                        cout<<" to ";
                        if((p+1) == 1) cout<<"R Phase";
                        else if((p+1) == 2) cout<<"Y Phase";
                        else cout<<"B phase";
                        cout<<endl;
                        
                        status = false;
                        if(m<toGive) {
                            m+=search;
                            search = toGive-m;
                        }

                        if(get<int>(phaseLoads[p+1][i]) == 0) {
                            phaseLoads[p+1][i] = phaseLoads[n+1][i];
                        } else {
                            vector<int> nestedVec;
                            nestedVec.push_back(get<int>(phaseLoads[p+1][i]));
                            nestedVec.push_back(get<int>(phaseLoads[n+1][i]));
                            phaseLoads[p+1][i] = nestedVec;
                        }
                        
                        phaseLoads[n+1][i] = 0;
                        phaseNeeds[n] = phaseNeeds[n] + get<int>(phaseLoads[n+1][i]);
                        phaseNeeds[p] = phaseNeeds[p]-toGive;
                        
                    } else if(holds_alternative<vector<int>>(phaseLoads[n+1][i])) {

                        for(int element : get<vector<int>>(phaseLoads[n+1][i])) {

                            if(element == search) {
                                cout<<"Shift "<<element<<"A load from ";
                                if((n+1) == 1) cout<<"R Phase";
                                else if((n+1) == 2) cout<<"Y Phase";
                                else cout<<"B phase";
                                cout<<" to ";
                                if((p+1) == 1) cout<<"R Phase";
                                else if((p+1) == 2) cout<<"Y Phase";
                                else cout<<"B phase";
                                cout<<endl;

                                status = false;
                                if(m<toGive) {
                                    m+=search;
                                    search = toGive-m;
                                }  
                                if(holds_alternative<int>(phaseLoads[p+1][i])) {
                                    if(get<int>(phaseLoads[p+1][i]) == 0) {
                                        phaseLoads[p+1][i] = phaseLoads[n+1][i];
                                    } else {
                                        vector<int> nestedVec;
                                        nestedVec.push_back(get<int>(phaseLoads[p+1][i]));
                                        nestedVec.push_back(element);
                                        phaseLoads[p+1][i] = nestedVec;
                                    }
                                } else {
                                    get<vector<int>>(phaseLoads[p+1][i]).push_back(element);
                                }
                                auto &vec = get<vector<int>>(phaseLoads[n+1][i]);
                                vec.erase(remove(vec.begin(), vec.end(), element), vec.end());
                                phaseNeeds[n] = phaseNeeds[n] + element;
                                phaseNeeds[p] = phaseNeeds[p]-toGive;

                            }
                        }
                    } 
                }
                if(status){
                    search = search-1;
                }
                if(search<=0) {
                    state = false;
                } 
               
            }
           
        }
    }
    cout<<endl;
    printLoads(phaseLoads);

    return 0;
}
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include<bits/stdc++.h>
using namespace std;


map<string, vector<string>> readCirc(const string& circ) {
    ifstream file(circ);
    map<string, vector<string>> circuit;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;
            string net, eq;
            size_t eqpos = line.find('=');
            if (eqpos != string::npos) {
                net = line.substr(0, eqpos);
                eq = line.substr(eqpos + 1);
                net.erase(remove_if(net.begin(), net.end(), ::isspace), net.end());
                eq.erase(remove_if(eq.begin(), eq.end(), ::isspace), eq.end());
                circuit[net].push_back(eq);
            }
        }
        file.close();
    }

    return circuit;
}
pair<string, string> readFaul(const string& faul) {
    ifstream file(faul);
    string location, type;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;
            size_t eqpos = line.find('=');
            if (eqpos != string::npos) {
                string parameter = line.substr(0, eqpos);
                string value = line.substr(eqpos + 1);
                parameter.erase(remove_if(parameter.begin(), parameter.end(), ::isspace), parameter.end());
                value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
                if (parameter == "FAULT_AT")
                    location = value;
                else if (parameter == "FAULT_TYPE")
                    type = value;
            }
        }
        file.close();
    }

    return {location, type};
}
vector<vector<int>> sampleTests={ {0 , 0 , 0 , 0 } ,  {0 , 0 , 0 , 1} ,  {0 , 0 , 1 , 0 } ,  {0 , 0 , 1 , 1 } , 
                {0 , 1 , 0 , 0 } ,  {0 , 1 , 0 , 1 } ,  {0 , 1 , 1 , 0 } ,  {0 , 1 , 1 , 1 } ,  {1 , 0 , 0 , 0 } ,
                 {1 , 0 , 0 , 1 } ,  {1 , 0 , 1 , 0 } ,  {1 , 0 , 1 , 1 } ,  {1 , 1 , 0 , 0 },  {1 , 1 , 0 , 1 },  
                     {1 , 1 , 1 , 0 } ,  {1 , 1 , 1 , 1 } };

int calculate1(int x , int y , string opera){
    int output ;
    
    if(opera == "&"){
        output = x & y ;
    }
    else if(opera == "|"){
        output = x | y ;
    }
    else if(opera == "^"){
        output = x ^ y ;
    }
    return output ;
}
// changes can be here ..
void appendSolutionMap(map<string , pair<int, int>> &solution , map<string , vector<string>>circuit , vector<vector<int>> sampleTests , bool ideal , bool faulty, int i , string key,map<string,vector<string>>::iterator it){

                int c = -1 ;
                  int d = -1 ;
                  string op ;
            for(int j = 0 ; j < it->second.size() ; j++){
                string s="~";
                int pr;
                size_t found_=it->second[j].find(s);
                if (found_ != std::string::npos){
                    if(it->second[j][1]>=65 && it->second[j][1]<=68){
                        int index=it->second[j][1]-65;
                        pr=sampleTests[i][index];
                        if(pr==0){
                            pr=1;
                        }else{
                            pr=0;
                        }
                    }
                    else{
                        string key1= to_string(it->second[j][1]);
                        pr=solution[key1].first;
                        if(pr==0){
                            pr=1;
                        }else{
                            pr=0;
                        }
                    }

                }
                else{
                  for(int k = 0 ; k < it->second.at(j).length() ; k++){
                if(it->second[j][k] >= 65 && it->second[j][k] <= 68 ){
                    int index = it->second[j][k] - 65 ;
                    if(c == -1){
                        c = sampleTests[i][index] ;
                    }
                    else{
                        d =  sampleTests[i][index] ;
                    }
                    
                }
                else if(it->second[j][k]=='&'|| it->second[j][k]=='|' || it->second[j][k]=='^') {
                    op = it->second[j][k] ;
                }
                
                
            }
            if (found_ != std::string::npos){
                 if(ideal == true ){
                     solution[key].first = pr;
                }
                
                if(faulty == true){
                    if(pr==0){
                        solution[key].second = 1;
                        }
                        else{
                            solution[key].second = 0;
                            }
                }
                }else{
                if(ideal == true ){
                     solution[key].first = calculate1(c , d , op) ;
                }
                
                if(faulty == true){
                    solution[key].second = calculate1(c , d , op) ;
                }
            }
    }
   }
}
int main() {
    string circ = "C:\\Users\\hp\\OneDrive\\Desktop\\C++ comding ninjas\\test_1.txt";
    string faul = "C:\\Users\\hp\\OneDrive\\Desktop\\C++ comding ninjas\\fault.txt";
    map<string, vector<string>> circuitnew = readCirc(circ);
    pair<string, string> fault = readFaul(faul);
    string location = fault.first;
    string type = fault.second;
    /**cout << location << " " << type << endl;
    for(auto & it : circuit){
        for(auto & i : it.second){
            cout << it.first << " " << i << endl;
            
        }
    }  **/
    map<string,vector<string>>circuit;
    map<string,vector<string>>:: iterator it=circuitnew.begin();
    for(;it!=circuitnew.end();it++){
        
        string s="net_";
         
        
        size_t found = it->first.find(s);
        string updatedKey = it->first;
       if (found != std::string::npos) {
        updatedKey = it->first.substr(4);
        circuit[updatedKey]={};
    }
        for(int g=0;g<it->second.size();g++){
            bool found_=it->second[g].find(s);
            if(found_){
            for(int q=0;q<it->second[g].length();q++){
                  if(it->second[g][q] =='n' && it->second[g].substr(q ,4) == s){
                  it->second[g].erase(q ,  4) ;
                  
                }
            }

          }
          circuit[updatedKey].push_back(it->second[g]);
        }
        
    }
    for(auto it: circuit){
        cout<<it.first<<" "<<it.second[0]<<endl;;
        
    }
    
    pair<string , vector<string>>  ans ;


    int run = 0 ;
    
     // conditions running
    bool ideal = true ;
    bool  faulty = false ;
    
    int count = 0 ;

    // ans map .
    map<string , pair<int, int>> solution ;

    for(int i = 0 ; i < sampleTests.size() ;){
        map<string , vector<string>> :: iterator it = circuit.begin() ;
        while(it != circuit.end()){
        
         string key = it->first ;
         
         // just once .. not required for bith ideal and faulty ...
        
        if (run == 0){
        
         if(count == 0){
            string s="net_";
             
             ans.first = key ;
             for(int g=0;g<it->second.size();g++){
         size_t found_=it->second[g].find(s);
           if (found_ != std::string::npos){
                
            for(int q=0;q<it->second[g].length();q++){
                  if(it->second[g][q] =='n' && it->second[g].substr(q ,4) == s){
                  it->second[g].erase(q ,  4) ;
                  
                }
            }

          }
          }
            ans.second =  it->second ;
            count++ ;
            run = 1 ;
            cout<<ans.first<<" "<<ans.second[0]<<endl;
            it++ ;
            continue ;
            
        }
        
        else if(it == circuit.begin() && run == 1){
            it++ ;
            continue ;
        }
        }
        
        if(ideal){
               appendSolutionMap(solution , circuit , sampleTests , true , false , i , key, it) ;
               it++ ;
               continue ;
        }
        
        else{
            if(faulty){
                if(key == fault.first){
                    if(fault.second == "SA0"){
                        solution[key].second = 0 ;
                    }
                    else{
                        solution[key].second = 1 ;
                    }
                }
                
                else{
                    appendSolutionMap(solution , circuit , sampleTests , false , true , i , key,it) ;
                    
                }
                
                it++ ;
                continue ;
            }

        }

    }
    
    faulty = !faulty ;
    ideal = !ideal ;
    if(faulty == false ){
        
        int r = -1 ;
        int s = -1 ;
        string oper ;
    
    
        // for security reasons we decided to increase the time complexity of our code ...
        
        bool ideally = true ;
        bool foly = false ; 
        
        
        int ideal_output = -1 ; 
        int faulty_output = -1 ;
        
        for(int m = 0 ; m < ans.second.size() ; m++){
            for(int n = 0 ; n < ans.second[m].length() ; n++){
        
                if (solution.find(to_string(ans.second[m][n])) != solution.end()) {
                    if(r == -1){
                        if(ideally){
                            r = solution[to_string(ans.second[m][n])].first ;
                        }
                        else{
                            r = solution[to_string(ans.second[m][n])].second ;
                        }
                         continue ;
                    }
                    else if(s == -1){
                        if(ideally){
                            s = solution[to_string(ans.second[m][n])].first ;
                        }
                        else{
                            s = solution[to_string(ans.second[m][n])].second ;
                        }
                    }
                }
                 else{
                    oper = ans.second[m][n] ;
                    continue ;
                }
                
                if(ideally){
                    ideal_output = calculate1(r , s , oper) ;
                    r = ideal_output ;
                }
                else if(foly){
                    faulty_output = calculate1(r , s , oper) ;
                    r = faulty_output ;
                }
                
                s = -1 ;
                oper = '\0' ;
                
                if(n == ans.second[m].length() && ideally == true){
                    solution["Z"].first = ideal_output ;
                    n = 0 ;
                    faulty = true ;
                    ideally = false ;
                }
                else if(n == ans.second[m].length() && faulty == true){
                     solution["Z"].second = faulty_output ;
                     break ;
                }
            }
        }
        
        if(solution["Z"].first != solution["Z"].second){
        
            for(int var = 0 ; var < sampleTests[i].size() ; var++){
              cout << sampleTests[i][var] << " ";
             }
             break;
        }
        std::ofstream outputFile("output.txt");
        if (outputFile.is_open()) {
            for (int var = 0; var < sampleTests[i].size(); var++) {
                outputFile << sampleTests[i][var] << " ";
            }
            outputFile.close();
            std::cout << "Output stored in output.txt" << std::endl;

        } else {
            std::cerr << "Unable to open output.txt" << std::endl;
            break;
        }
        
    
    i++ ;
    }
    else{
        continue ;
    }
  }
}


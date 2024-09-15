/*
CREATOR   		Tanishka Gupta
ROLL NUMBER 	2201CS93
Declaration of Authorship and Genuinity
This file asm.cpp is part of the assignment of CS210 at the 
department of Computer Science and Engineering, IIT Patna . 
*/


#include <bits/stdc++.h>
using namespace std;

int pc;                                         // Program Counter
bool error = 0;                                 //for checking for error after 1st pass
map<string,int> mp_label;                       //map to store label and addresses
map<string,pair<string,int>> instrc;            //map for storing available instructions
vector<pair<string,int>> var;                   //temp var not yet in mp_label

string remove_comment(string line){
    int pos = line.find(';');
    if (pos != string::npos) {
        line.erase(pos);
    }
    return line;
}

string remove_spaces(string line){
    int startIndex = line.find_first_not_of(" \t\r\n\f\v");
    line = (startIndex == string::npos) ? "" : line.substr(startIndex);
    int end = line.find_last_not_of(" \t\r\n\f\v");
    return (end == string::npos) ? "" : line.substr(0, end + 1);
}

bool is_num(string str){

    if (str.empty() || (str[0] != '-' && str[0] != '+' && !isdigit(str[0]))) {
        return false;
    }

    for (size_t i = 1; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool is_hex(string word){
    if(!(word.length()>2 && word[0]=='0' && word[1]=='x')){
        return 0;
    }
    for(int i=2;i<word.length();i++){
        if(!( (word[i]<='9' && word[i]>='0') || (word[i]<='F' && word[i]>='A') )){
            return 0;
        }
        return 1;
    }
}

bool is_label(string label){
    if(!isalpha(label[0])){                                     
        return 0;
    }

    for(int i=0;i<label.size();i++){
        if(!isalnum(label[i])){
            return 0;
        }
    }
    return 1;
}

int hexToDecimal(const string& hexStr) {
    unsigned long decimalValue = stoul(hexStr, nullptr, 16);
    if (hexStr.front() >= '8') {
        decimalValue = decimalValue - (1UL << hexStr.length() * 4);
    }
    return static_cast<int>(decimalValue);
}

int has_label(string line){                     //return 0 - no error(with label)
    int label_index = line.find(":");           //return 1 err(invalid label)
    if(label_index !=string::npos){             //return 2  err(duplicate label)
                                                
        string label = line.substr(0,label_index);                  //return 3 label(valid data)
        line = line.substr(label_index);                            // return 4 invalid data
        
        if(!is_label(label)){                                       //-1 without label
            return 1;
        }
        istringstream words(line);
        string word;
        words >> word;
        words >> word;
        
        if(word == "data"){
            if(!(words >> word)){
                return 4;
            }
            // cout<<word<<endl;
            if(is_num(word)){
                string a;
                if(words >> a){
                    return 4;
                }
                mp_label[label]=stoi(word);
                return 3;
            }
            else if(is_hex(word)){
                string a;
                if(words >> a){
                    return 4;
                }
                mp_label[label]=hexToDecimal(word);
            }
            else{
                return 4;
            }
        }
        
        if(mp_label.find(label)==mp_label.end()){
            
            mp_label[label]=pc;
            if(word == "SET"){
                words >> word;
                if(is_num(word)){
                    string a;
                    if(words >> a){
                        return 4;
                    }
                    mp_label[label]=stoi(word);
                } 
                else if(is_hex(word)){
                    string a;
                    if(words >> a){
                        return 4;
                    }
                    mp_label[label]=hexToDecimal(word);
                }
                else{
                    return 5;
                }
            }
            return 0;
        }
        else{
            return 2;
        }
    }
    return -1;
}

void instructions(){                           //0 - no operand
    instrc["ldc"] = {"00000000", 1};           //1 - value
    instrc["adc"] = {"00000001", 1};           //2 - offset
    instrc["ldl"] = {"00000010", 2};           //3 - set
    instrc["stl"] = {"00000011", 2};           //4 - data
    instrc["ldnl"] = {"00000100", 2};          //5 - pc offset
    instrc["stnl"] = {"00000101", 2};
    instrc["add"] = {"00000110", 0};
    instrc["sub"] = {"00000111", 0};
    instrc["shl"] = {"00001000", 0};
    instrc["shr"] = {"00001001", 0};
    instrc["adj"] = {"00001010", 1};
    instrc["a2sp"] = {"00001011", 0};
    instrc["sp2a"] = {"00001100", 0};
    instrc["call"] = {"00001101", 5};
    instrc["return"] = {"00001110", 0};
    instrc["brz"] = {"00001111", 5};
    instrc["brlz"] = {"00010000", 5};
    instrc["br"] = {"00010001", 5};
    instrc["HALT"] = {"00010010", 0};
    instrc["data"] = {"10000000", 4};
    instrc["SET"] = {"10000001", 3};
}

string toTwosComplement(int num) {
    if (num >= 0) {
        string binaryStr = bitset<24>(num).to_string();
        return binaryStr.substr(binaryStr.size() - 24);
    } 
    else{
        int positiveEquivalent = (1 << 24) + num;
        string binaryStr = bitset<24>(positiveEquivalent).to_string();
        return binaryStr.substr(binaryStr.size() - 24);
    }
}

string binaryToHex(const string& binary) {
    bitset<32> bitset(binary);
    stringstream hexStream;
    hexStream << hex << uppercase << bitset.to_ulong();

    string hexResult = hexStream.str();
    hexResult = string(8 - hexStream.str().length(),'0') + hexStream.str();
    return hexResult;
}

int main(){

    ios_base ::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    
    instructions();
    ofstream log,obj,list;
    ifstream code_file;
    
    
    string filename = "test8.asm";


    code_file.open(filename,ios::in);                              //input code_file
    log.open("log.txt",ios::out);                                     //code's log file
    list.open("list.txt",ios::out);                                   //listings file
    obj.open("obj.dat",ios::out);                                     //obj file
    int line_no=0;

    if (!code_file.is_open()) {
        log << "Error opening code_file!"<< endl;
        return 1; 
    }

    string line;
    
    while (getline(code_file, line)){
        line_no++;
        line = remove_comment(line);
        
        line = remove_spaces(line);
        
        int label = has_label(line);
        if(label == 5){
            log << "Invalid allocation using SET on line: "<< line_no <<endl;
        }
        if(label != -1){
            if(label==1){
                log << "Invalid label on  Line " << line_no << endl;
                error = 1;
            }
            else if(label==2){
                log << "Duplicate label on Line " << line_no << endl;
                error = 1;
            }
            else if(label == 4){
                log << "Invalid input after data on line: "<<line_no<<endl;
                error = 1;
            }
        }
        if(label!=-1){
            int j =line.find(':');
            line=line.substr(j+1,line.length()-j+1);
        }

        line = remove_spaces(line);
        
        if(line.empty()){
            continue;
        }
        istringstream words(line);
        string word;
        words >> word;
        if(instrc.find(word)==instrc.end()){
            log << "Not a valid instruction on Line " << line_no << endl;
            error = 1;
            continue;
        }
        else{
            int code = instrc[word].second;
            if(code == 0){
                if(words >> word){
                    log << "Invalid Syntax on line: "<< line_no <<endl;
                    error = 1;
                }
            }
            else if(code == 1){
                if(!(words >> word)){
                    log << "No operand value present on line: "<< line_no <<endl;
                    error = 1;
                }
                else{
                    if(is_num(word)){}
                    else if(is_hex(word)){}
                    else if(!is_label(word)){
                        log << "Invalid Syntax on line: "<<line_no<<endl;
                        error = 1;
                    }
                    else{
                        var.push_back({word,line_no});
                    }
                }
            }
            else if(code == 2){
                if(!(words >> word)){
                    error = 1;
                    log << "No offset operand present on line: "<<line_no<<endl;
                }
                else{
                    if(is_num(word)){}
                    else if(is_hex(word)){}
                    else if(!is_label(word)){
                        log << "Invalid Syntax on line: "<<line_no<<endl;
                        error = 1;
                    }
                    else{
                        var.push_back({word,line_no});
                    }
                }
            }
            else if(code == 5){
                if(!(words >> word)){
                    error = 1;
                    log << "No offset operand present on line: "<<line_no<<endl;
                }
                else{
                    if(is_num(word)){}
                    else if(is_hex(word)){}
                    else if(!is_label(word)){
                        log << "Invalid Syntax on line: "<<line_no<<endl;
                        error = 1;
                    }
                    else{
                        var.push_back({word,line_no});
                    }
                }
            }
            else{
                continue;
            }
        }
        if(words >> word){
            error = 1;
            log << "Too many arguments on line: "<<line_no<<endl;
        }
        pc++;
    }
    for(auto i : var){
        if(mp_label.find(i.first)==mp_label.end() ){
            error =1;
            log << "Undeclared value accessed on line: "<<i.second<<endl;
        }
    }
    if(error == 0){
        log << "1st pass completed with no errors\n";
    }
    else{
        return 1;
    }
    
    pc=0;
    code_file.close();
    code_file.open(filename,ios::in);


    while(getline(code_file,line)){
        string original_line = line;
        line = remove_comment(line);
        line = remove_spaces(line);
        int j=-1;
        for(int i=0;i<line.length();i++){
            if(line[i]==':'){
                j=i;
                break;
            }
        }
        if(j!=-1){
            line=line.substr(j+1,line.length()-j+1);
        }
        line = remove_spaces(line);
        if(line.length()==0){continue;}
        istringstream words(line);
        string word1,word2;
        words >> word1;
        string ans = "";
        if(instrc[word1].second == 3 || instrc[word1].second == 4){
            continue;
        }
        if(instrc[word1].second == 5){
            int off = 0;
            words >> word2;
            if(is_num(word2)){
                int no = stoi(word2);
                ans += toTwosComplement(no);
            }
            else if(is_label(word2)){
                if(mp_label.find(word2)!=mp_label.end()){
                    ans += toTwosComplement(mp_label[word2]-pc-1);
                }
            }
        }
        else if(instrc[word1].second > 0){
            words >> word2;
            if(is_label(word2)){
                if(mp_label.find(word2)!=mp_label.end()){
                    ans += toTwosComplement(mp_label[word2]);
                }
            }
            else if(is_hex(word2)){
                ans+= toTwosComplement(hexToDecimal(word2));
            }
            else{
                int d=stoi(word2);
                ans += toTwosComplement(d);
            }
        }
        else{
            int d=0;
            ans += toTwosComplement(d);
        }
        ans+=instrc[word1].first;
        obj<<binaryToHex(ans)<<endl;
        list << binaryToHex(toTwosComplement(pc)) << " " <<binaryToHex(ans) << "      " << original_line << endl;
        pc++;  
    }
    // for(auto i:mp_label){
    //     cout<<i.first<<" "<<i.second<<endl;
    // }

    log << "Machine code Successfully generated "<<endl;
    
    log.close();
    obj.close();
    code_file.close();
    list.close();
    return 0;
}
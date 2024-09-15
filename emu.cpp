/*																														
CREATOR   		Tanishka Gupta
ROLL NUMBER 	2201CS93
Declaration of Authorship and Genuinity
This file asm.cpp is part of the assignment of CS210 at the 
department of Computer Science and Engineering, IIT Patna . 
*/


#include <bits/stdc++.h>
using namespace std;

map<int,string> mp_instrc;                                 //instruction memory
vector<int> mem(1e7);                                      //main memory

int hexCharToDecimal(char hexChar) {
    if (isdigit(hexChar)) {
        return hexChar - '0';
    } else {
        return tolower(hexChar) - 'a' + 10;
    }
}

int hexToDecimal(const string& hexString) {
    int result = 0;
    for (char hexChar : hexString) {
        result = result * 16 + hexCharToDecimal(hexChar);
    }
    return result;
}

int hexToDecimal2sComplement(const std::string& hexString, int bitLength) {
    int decimalValue = hexToDecimal(hexString);
    if (hexCharToDecimal(hexString[0]) >= 8) {
        int mask = (1 << bitLength) - 1;
        decimalValue = decimalValue - (1 << bitLength);
    }
    return decimalValue;
}

// string toTwosComplement(int num) {
//     if (num >= 0) {
//         string binaryStr = bitset<24>(num).to_string();
//         return binaryStr.substr(binaryStr.size() - 24);
//     } 
//     else{
//         int positiveEquivalent = (1 << 24) + num;
//         string binaryStr = bitset<24>(positiveEquivalent).to_string();
//         return binaryStr.substr(binaryStr.size() - 24);
//     }
// }

string toTwosComplement(int num) {
    
    bitset<32> binaryRepresentation(num & 0xFFFFFFFF);
    if(num<0){
        bitset<32> binaryRepresentation2(-1*num & 0xFFFFFFFF);
        binaryRepresentation2 = ~binaryRepresentation2;
        for (int i = 0; i <=31; i++) {
            if (binaryRepresentation2[i] == 0) {
                binaryRepresentation2[i] = 1;
                break;
            } else {
                binaryRepresentation2[i] = 0;
            }
        }
        stringstream ss;
        ss << binaryRepresentation2;
        return ss.str();
    }
    stringstream ss;
    ss << binaryRepresentation;
    return ss.str();
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
    ofstream dump,log;
    ifstream code_file;
    code_file.open("obj.dat",ios::in);                              //input code_file
    dump.open("dump.txt",ios::out);  
    string line;
    int line_no=0;

    if (!code_file.is_open()) {
        log << "Error opening code_file!"<< endl;
        return 1; 
    }

    while(getline(code_file, line)){
        mp_instrc[line_no]=line;
        line_no++;
    }
    code_file.close();

    int pc = 0, sp = 0, Reg_a = 0, Reg_b = 0;

    while(1){
        line  = mp_instrc[pc];
        if(line.empty()){
            return 0;
        }
        pc++;
        string op;
        for(int i=6;i<8;i++){
            op.push_back(line[i]);
        }
        string imm;
        for(int i=0;i<6;i++){
            imm.push_back(line[i]);
        }
        int opcode = hexToDecimal2sComplement(op,8);
        int value = hexToDecimal2sComplement(imm,24);
        int offset = value;
        
        switch (opcode){
            case 0:                                      //ldc
                Reg_b = Reg_a;
                Reg_a = value;
                break;
            case 1:                                      //adc
                Reg_a = Reg_a + value;
                break;
            case 2:                                      //ldl
                Reg_b = Reg_a;
                Reg_a = mem[sp + offset];
                break;
            case 3:                                      //stl
                mem[sp + offset] = Reg_a;
                Reg_a = Reg_b;
                break;
            case 4:                                      //ldnl
                Reg_a = mem[Reg_a + offset];
                break;
            case 5:                                      //stnl
                mem[Reg_a + offset] = Reg_b;
                break;
            case 6:                                      //add
                Reg_a = Reg_a + Reg_b;
                break;
            case 7:                                      //sub
                Reg_a = Reg_b - Reg_a;
                break;
            case 8:                                      //shl
                Reg_a = Reg_b << Reg_a;                  
                break;
            case 9:                                      //shr
                Reg_a = Reg_b >> Reg_a;
                break;
            case 10:                                     //adj
                sp = sp + value;
                break;
            case 11:                                     //a2sp
                sp = Reg_a;
                Reg_a = Reg_b;
                break;
            case 12:                                     //sp2a
                Reg_b = Reg_a;
                Reg_a = sp;
                break;
            case 13:                                     //call
                Reg_b = Reg_a;
                Reg_a = pc;
                pc = pc + offset;
                break;
            case 14:                                     //return
                pc = Reg_a;
                Reg_a = Reg_b;
                break;
            case 15:                                     //brz
                if(Reg_a == 0){
                    pc = pc + offset;
                }
                break;
            case 16:                                     //brlz
                if(Reg_a < 0){
                    pc = pc + offset;
                }
                break;
            case 17:                                     //br
                pc = pc + offset;
                break;
            case 18:                                     //HALT
                return 0;
                break;
            default:
                dump << "Unusual value found!";
                return 0;
        }
        string line;
        line += "PC = " + binaryToHex(toTwosComplement(pc)) + " SP = " + binaryToHex(toTwosComplement(sp)) + " Reg_A = " + binaryToHex(toTwosComplement(Reg_a)) + " Reg_B = " + binaryToHex(toTwosComplement(Reg_b));
        dump << line <<endl;
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

map<string, pair<string, string>> load_opcode()
{
    map<string, pair<string, string>> OPCODES;
    OPCODES["ADD"] = {"3/4", "18"};
    OPCODES["ADDF"] = {"3/4", "58"};
    OPCODES["ADDR"] = {"2", "90"};
    OPCODES["AND"] = {"3/4", "40"};
    OPCODES["CLEAR"] = {"2", "B4"};
    OPCODES["COMP"] = {"3/4", "28"};
    OPCODES["COMPF"] = {"3/4", "88"};
    OPCODES["COMPR"] = {"2", "A0"};
    OPCODES["DIV"] = {"3/4", "24"};
    OPCODES["DIVF"] = {"3/4", "64"};
    OPCODES["DIVR"] = {"2", "9C"};
    OPCODES["FIX"] = {"1", "C4"};
    OPCODES["FLOAT"] = {"1", "C0"};
    OPCODES["HIO"] = {"1", "F4"};
    OPCODES["J"] = {"3/4", "3C"};
    OPCODES["JEQ"] = {"3/4", "30"};
    OPCODES["JGT"] = {"3/4", "34"};
    OPCODES["JLT"] = {"3/4", "38"};
    OPCODES["JSUB"] = {"3/4", "48"};
    OPCODES["LDA"] = {"3/4", "00"};
    OPCODES["LDB"] = {"3/4", "68"};
    OPCODES["LDCH"] = {"3/4", "50"};
    OPCODES["LDF"] = {"3/4", "70"};
    OPCODES["LDL"] = {"3/4", "08"};
    OPCODES["LDS"] = {"3/4", "6C"};
    OPCODES["LDT"] = {"3/4", "74"};
    OPCODES["LDX"] = {"3/4", "04"};
    OPCODES["LPS"] = {"3/4", "D0"};
    OPCODES["MUL"] = {"3/4", "20"};
    OPCODES["MULF"] = {"3/4", "60"};
    OPCODES["MULR"] = {"2", "98"};
    OPCODES["NORM"] = {"1", "C8"};
    OPCODES["OR"] = {"3/4", "44"};
    OPCODES["RD"] = {"3/4", "D8"};
    OPCODES["RMO"] = {"2", "AC"};
    OPCODES["RSUB"] = {"3/4", "4C"};
    OPCODES["SHIFTL"] = {"2", "A4"};
    OPCODES["SHIFTR"] = {"2", "A8"};
    OPCODES["SIO"] = {"1", "F0"};
    OPCODES["SSK"] = {"3/4", "EC"};
    OPCODES["STA"] = {"3/4", "0C"};
    OPCODES["STB"] = {"3/4", "78"};
    OPCODES["STCH"] = {"3/4", "54"};
    OPCODES["STF"] = {"3/4", "80"};
    OPCODES["STI"] = {"3/4", "D4"};
    OPCODES["STL"] = {"3/4", "14"};
    OPCODES["STS"] = {"3/4", "7C"};
    OPCODES["STSW"] = {"3/4", "E8"};
    OPCODES["STT"] = {"3/4", "84"};
    OPCODES["STX"] = {"3/4", "10"};
    OPCODES["SUB"] = {"3/4", "1C"};
    OPCODES["SUBF"] = {"3/4", "5C"};
    OPCODES["SUBR"] = {"2", "94"};
    OPCODES["SVC"] = {"2", "B0"};
    OPCODES["TD"] = {"3/4", "E0"};
    OPCODES["TIO"] = {"1", "F8"};
    OPCODES["TIX"] = {"3/4", "2C"};
    OPCODES["TIXR"] = {"2", "B8"};
    OPCODES["WD"] = {"3/4", "DC"};

    return OPCODES;
}


map<char,char> load_register(){
    map<char,char> reg_number;
    reg_number['A']='0';
    reg_number['X']='1';
    reg_number['L']='2';
    reg_number['B']='3';
    reg_number['S']='4';
    reg_number['T']='5';
    reg_number['F']='6';
    return reg_number;
}






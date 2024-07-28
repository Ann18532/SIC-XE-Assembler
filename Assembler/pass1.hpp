#include<bits/stdc++.h>
#include"opcode_table.hpp"
using namespace std;

int lengthofprogram;

vector<string> break_line(string line)
{
    vector<string> parse_line;
    if (line[0] == ' ')
        parse_line.push_back(" ");

    int i;
    if (line[0] == ' ')
        i = 1;
    else
        i = 0;

    string part;
    while (i < line.length())
    {
        if (line[i] == ' ')
        {
            parse_line.push_back(part);
            part.clear();
        }
        else
            part += (line[i]);
        i++;
    }
    parse_line.push_back(part);

    return parse_line;
}

map<string, pair<char, pair<int, int>>> symbol_table_1;
map<string, pair<string, string>> opcode_table;
set<string> littab;
map<string, pair<int, int>> literal_tab;
map<string, int> programblocknumber;
map<int, int> prog_blk_ctr;
map<int, int> programblockaddress;

void insert_symbol_table_1(string label, int loc_ctr, int blk)
{
    symbol_table_1[label] = {'R', {loc_ctr, blk}};
}

int search_symbol_table_1(string label)
{
    if (symbol_table_1.find(label) != symbol_table_1.end())
        return 1;
    else
        return 0;
}

string search_opcode_table(string opcode)
{
    if (opcode_table.find(opcode) != opcode_table.end())
    {
        return opcode_table[opcode].first;
    }
    else
    {
        return "na";
    }
}
ofstream errorFile("error_pass1.txt");
int loc_ctr;
int evaluateExpression(string expression)
{
    stringstream ss(expression);
    string token;
    vector<string> tokens;

    while (getline(ss, token, '+'))
    {
        tokens.push_back(token);
    }

    int result = 0;
    for (const auto &token : tokens)
    {
        size_t pos = token.find('-');
        if (pos != string::npos)
        { 
            string var1 = token.substr(0, pos);
            string var2 = token.substr(pos + 1);
            if(symbol_table_1.find(var1)==symbol_table_1.end()){
                errorFile<<"Label should be defined before it is used as an operand field at location counter(in decimal): "<<loc_ctr<<endl;
                
            }
            if(symbol_table_1.find(var2)==symbol_table_1.end()){
                errorFile<<"Label should be defined before it is used as an operand field at location counter(in decimal): "<<loc_ctr<<endl;
                
            }
            int val1 = 0;
            int val2 = symbol_table_1.at(var2).second.first;
            if (symbol_table_1.at(var1).first == 'R')
            {
                val1 = symbol_table_1.at(var1).second.first;
            }
            result -= val2; 
            result += val1; 
        }
        else
        {                                            
            int val = symbol_table_1.at(token).second.first; 
            if (symbol_table_1.at(token).first == 'R')
            { 
                result += val;
            }
        }
    }

    return result;
}

void pass1()
{
    cout << "Inside pass1 function" << endl;

    ifstream inputFile("input.txt");
    ofstream intermediateFile("intermediate.txt"), symbol_table_1File("symbol_table_1.txt");
    
    if (!inputFile)
    {
        errorFile << "Error: Unable to open the input file!" << endl;
    }
    if (!intermediateFile)
    {
        errorFile << "Error: Unable to open the intermediate file!" << endl;
    }
    if (!errorFile)
    {
        errorFile << "Error: Unable to open the error file!" << endl;
    }
    if (!symbol_table_1File)
    {
        errorFile << "Error: Unable to open the symbol_table_1 file!" << endl;
    }

    
    int starting_address;
    opcode_table = load_opcode();
    string line;
    string blk_name = "default";
    int blocks = 1;
    while (getline(inputFile, line))
    {
        if (line[0] == '.')
        {
            intermediateFile << "-"
                             << " " << line << endl;
            continue;
        }
        vector<string> parse_line = break_line(line);
        string label = parse_line[0];
        string opcode = parse_line[1];
        string operand;
        if (parse_line.size() == 2 || parse_line[2] == "")
            operand = " ";
        else
            operand = parse_line[2];

        if (opcode == "USE")
        {
            if (operand == " ")
            {
                blk_name = "default";
                loc_ctr = prog_blk_ctr[1];
            }
            else
            {
                if (programblocknumber.find(operand) != programblocknumber.end())
                {
                    blk_name = operand;
                    loc_ctr = prog_blk_ctr[programblocknumber[blk_name]];
                }
                else
                {
                    blocks++;
                    blk_name = operand;
                    programblocknumber[blk_name] = blocks;
                    prog_blk_ctr[blocks] = 0;
                    loc_ctr = 0;
                }
            }
        }

        if (opcode == "START")
        {
            loc_ctr = stoi(operand);
            starting_address = loc_ctr;
            programblocknumber["default"] = 1;
            prog_blk_ctr[1] = 0;
            programblockaddress[1] = 0;
            intermediateFile << programblocknumber[blk_name] << " " << line << endl;
            continue;
        }

        if (opcode == "END")
        {
            intermediateFile << "-"
                             << " " << line << endl;
            for (auto literal : littab)
            {
                string literal_line = "* " + literal + " ";
                intermediateFile << programblocknumber[blk_name] << " " << literal_line << endl;
                literal_tab[literal] = {loc_ctr, programblocknumber[blk_name]};
                if (literal[1] == 'C')
                {
                    loc_ctr += (literal.length() - 4);
                }
                else
                {
                    loc_ctr += (literal.length() - 4) / 2;
                }
                prog_blk_ctr[programblocknumber[blk_name]] = loc_ctr;
            }
            lengthofprogram = 0;
            for (auto prog_blks : prog_blk_ctr)
            {
                lengthofprogram += (prog_blks.second);
            }
            break;
        }

        if (operand[0] == '=')
        {
            littab.insert(operand);

        }
        if (opcode == "LTORG")
        {
            intermediateFile << "-"
                             << " " << line << endl;
            for (auto literal : littab)
            {
                string literal_line = "* " + literal + " ";
                intermediateFile << programblocknumber[blk_name] << " " << literal_line << endl;
                literal_tab[literal] = {loc_ctr, programblocknumber[blk_name]};
                if (literal[1] == 'C')
                {
                    loc_ctr += (literal.length() - 4);
                }
                else
                {
                    loc_ctr += (literal.length() - 4) / 2;
                }
            }
            prog_blk_ctr[programblocknumber[blk_name]] = loc_ctr;
            littab.clear();

            continue;
        }
        if (opcode == "EQU")
        {
            if (operand == "*")
            {
                symbol_table_1[label] = {'R', {loc_ctr, programblocknumber[blk_name]}};
                intermediateFile << programblocknumber[blk_name] << " " << line << endl;
            }
            else
            {
                symbol_table_1[label] = {'A', {evaluateExpression(operand), -1}};
                intermediateFile << "-"
                                 << " " << line << endl;
            }

            continue;
        }

        if (label != " ")
        {
            int search = search_symbol_table_1(label);
            if (search == 0)
                insert_symbol_table_1(label, loc_ctr, programblocknumber[blk_name]);
            else
            {

                string error_line = "Error: Duplicate Symbol at location counter(in decimal): " + to_string(loc_ctr);
                errorFile << error_line << endl;
            }
        }

        string search;
        if (opcode[0] == '+')
        {
            search = search_opcode_table(opcode.substr(1, opcode.length() - 1));
        }
        else
            search = search_opcode_table(opcode);
        if (search != "na")
        {
            if (search == "1")
                loc_ctr += 1;
            else if (search == "2")
                loc_ctr += 2;
            else
            {
                if (opcode[0] == '+')
                    loc_ctr += 4;
                else
                    loc_ctr += 3;
            }
        }
        else if (opcode == "WORD")
        {
            loc_ctr += 3;
        }
        else if (opcode == "RESW")
        {
            loc_ctr += (3 * stoi(operand));
        }
        else if (opcode == "RESB")
        {
            loc_ctr += (stoi(operand));
        }
        else if (opcode == "BYTE")
        {
            if (operand[0] == 'C')
            {
                if (operand[1] != '\'' || operand[operand.length() - 1] != '\'')
                {
                    string error_line = "Error: Invalid operand value for BYTE type instructions at location counter: " + to_string(loc_ctr);
                    errorFile << error_line << endl;
                }
                else
                    loc_ctr += (operand.length() - 3);
            }
            else if (operand[0] == 'X')
            {
                if (operand[1] != '\'' || operand[operand.length() - 1] != '\'' || (operand.length() - 3) % 2 != 0)
                {
                    string error_line = "Error: Invalid operand value for BYTE type instructions at location counter: " + to_string(loc_ctr);
                    errorFile << error_line << endl;
                }
                else
                    loc_ctr += ((operand.length() - 3) / 2);
            }
            else
            {
                string error_line = "Error: Invalid format for BYTE type instructions at location counter: " + to_string(loc_ctr);
                errorFile << error_line << endl;
            }
        }
        else if (opcode == "BASE")
        {
            intermediateFile << programblocknumber[blk_name] << " " << line << endl;
            continue;
        }
        else if (opcode != "EQU" && opcode != "LTORG" && opcode != "USE")
        {
            string error_line = "Error: Invalid opcode at location counter: " + to_string(loc_ctr);
            errorFile << error_line << endl;
        }

        intermediateFile << programblocknumber[blk_name] << " " << line << endl;
        prog_blk_ctr[programblocknumber[blk_name]] = loc_ctr;
    }
    int previous = 0;
    for (auto len : prog_blk_ctr)
    {
        programblockaddress[len.first] = previous;
        previous += len.second;
    }

    string symbol_table_1_line = "SYMTAB";
    symbol_table_1File << symbol_table_1_line << endl;
    symbol_table_1_line = "---------------------";
    symbol_table_1File << symbol_table_1_line << endl;

    for (auto literal : literal_tab)
    {
        symbol_table_1[literal.first] = {'R', {literal.second.first, literal.second.second}};
    }
    for (auto i : symbol_table_1)
    {
        symbol_table_1_line = i.first + " " + i.second.first + " " + to_string(i.second.second.first) + " " + to_string(i.second.second.second);
        symbol_table_1File << symbol_table_1_line << endl;
    }
    symbol_table_1_line = "---------------------";
    symbol_table_1File << symbol_table_1_line << endl;
    symbol_table_1_line = "Program length: " + to_string(lengthofprogram);
    symbol_table_1File << symbol_table_1_line << endl;

    inputFile.close();
    intermediateFile.close();
    errorFile.close();
    symbol_table_1File.close();
}
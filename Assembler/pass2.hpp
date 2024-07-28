#include<bits/stdc++.h>
#include"pass1.hpp"
using namespace std;


map<string, pair<char, pair<int, int>>> read_symbol_table_2_file()
{
    map<string, pair<char, pair<int, int>>> symbolTable;
    ifstream symbol_table_2File("symbol_table_1.txt");
    if (!symbol_table_2File)
    {
        cout << "Error: Unable to open the symbol_table_2 file!" << endl;
    }
    string line;
    while (getline(symbol_table_2File, line))
    {
        istringstream iss(line);
        string symbol;
        char rel;
        int value;
        int blk_no;
        if (!(iss >> symbol >> rel >> value >> blk_no))
        {
            continue;
        }

        symbolTable[symbol] = {rel, {value, blk_no}};
    }
    return symbolTable;
}

vector<string> break_intermediateline(string line)
{
    vector<string> parse_line;
    if (line[0] == ' ')
        parse_line.push_back(" ");

    int i;
    if (line[0] == ' ')
        i = 1;
    else
        i = 0;

    string part = "";
    while (i < line.length())
    {
        if (line[i] == ' ')
        {
            parse_line.push_back(part);
            part = "";
        }
        else
            part += (line[i]);
        i++;
    }
    parse_line.push_back(part);

    return parse_line;
}

string intTo6Hex(int value)
{
    if (value < 0)
    {
        value = (1 << 24) + value;
    }

    std::stringstream stream;
    stream << uppercase << hex << setw(6) << setfill('0') << value;
    return stream.str();
}
string intToHex(int value, int digits)
{
    if (value < 0)
    {
        value = (1 << 4 * digits) + value;
    }

    std::stringstream stream;
    stream << uppercase << hex << setw(digits) << setfill('0') << value;
    return stream.str();
}

string pad_string(string name)
{
    int left = 6 - name.length();
    while (left > 0)
    {
        name += ' ';
        left--;
    }
    return name;
}

string begin_text_record(int loc_ctr)
{
    string text_record = "T^";
    text_record += (intTo6Hex(loc_ctr));
    text_record += "^  ^";
    return text_record;
}

map<string, pair<string, string>> opcode_table_pass2;
map<string, pair<char, pair<int, int>>> symbol_table_2;
map<char, char> registers;
vector<int> mod_record;
map<int, int> programblockcounter_2;
map<string, int> programblocknumber_2;

string search_opcode_table_pass2(string opcode)
{
    if (opcode_table_pass2.find(opcode) != opcode_table_pass2.end())
    {
        return opcode_table_pass2[opcode].first;
    }
    else
    {
        
        return "na";
    }
}

string binary_to_hex(string binary_str)
{
    int decimal_value = stoi(binary_str, nullptr, 2);

    stringstream ss;
    ss << uppercase << hex << decimal_value;

    return ss.str();
}

int hexStringToInt(string hex_str)
{
    return stoi(hex_str, nullptr, 16);
}

int search_symbol_table_2(string label)
{
    if (symbol_table_2.find(label) != symbol_table_2.end())
        return 1;
    else
        return 0;
}

string asciiToHex(string input)
{
    stringstream output;
    for (char c : input)
    {
        output << uppercase << hex << setw(2) << setfill('0') << (int)c;
    }
    return output.str();
}
string obj_code = "";
int search_symbol;
string list_line(40, ' ');
string list_line_copy(40, ' ');

void pass2()
{
    cout << "Inside pass2 function" << endl;

    ifstream intermediateFile("intermediate.txt");
    ofstream listingFile("listingFile.txt"), objectFile("objectFile.txt"), errorFile("error_pass2.txt");

    if (!intermediateFile)
    {
        errorFile << "Error: Unable to open the intermediate file!" << endl;
    }
    if (!listingFile)
    {
        errorFile << "Error: Unable to open the listing file!" << endl;
    }
    if (!objectFile)
    {
        errorFile << "Error: Unable to open the object file!" << endl;
    }
    if (!errorFile)
    {
        errorFile << "Error: Unable to open the error file!" << endl;
    }

    opcode_table_pass2 = load_opcode();
    registers = load_register();
    symbol_table_2 = read_symbol_table_2_file();

    vector<string> objectProgram;

    int location_counter_2 = 0;
    int starting_address;
    string line;
    int left = 60;
    string text_record = begin_text_record(location_counter_2);
    programblockcounter_2[1] = 0;
    int pc = 0;
    int num_blocks = 1;
    string block_name = "default";
    int base;
    int baseblock;
    int previouslocationcounter = 0;

    while (getline(intermediateFile, line))
    {
        list_line = list_line_copy;
        if (line[2] == '.')
        {

            list_line.replace(7, line.length() - 2, line.substr(2));
            listingFile << list_line << endl;
            continue;
        }

        vector<string> parse_line = break_intermediateline(line);
        int prog_block;
        if (parse_line[0] != "-")
            prog_block = stoi(parse_line[0]);

        string label = parse_line[1];
        string opcode = parse_line[2];
        string operand;
        if (parse_line[3] == "" || parse_line.size() == 3)
            operand = " ";
        else
            operand = parse_line[3];

        if (opcode == "USE")
        {

            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);
            if (operand == " ")
                block_name = "default";
            else
                block_name = operand;

            left = 60;
            if (operand == " ")
            {
                block_name = "default";
                location_counter_2 = programblockcounter_2[1];
                text_record = begin_text_record(programblockcounter_2[programblocknumber_2[block_name]] + programblockaddress[programblocknumber_2[block_name]]);
            }
            else
            {
                if (programblocknumber_2.find(operand) != programblocknumber_2.end())
                {
                    block_name = operand;
                    location_counter_2 = programblockcounter_2[programblocknumber_2[block_name]];
                    text_record = begin_text_record(programblockcounter_2[programblocknumber_2[block_name]] + programblockaddress[programblocknumber_2[block_name]]);
                }
                else
                {
                    num_blocks++;
                    block_name = operand;
                    programblocknumber_2[block_name] = num_blocks;
                    programblockcounter_2[num_blocks] = 0;
                    location_counter_2 = 0;
                    text_record = begin_text_record(programblockcounter_2[programblocknumber_2[block_name]] + programblockaddress[programblocknumber_2[block_name]]);
                }
            }
        }

        if (opcode == "BASE")
        {
            base = symbol_table_2[operand].second.first;
            baseblock = symbol_table_2[operand].second.second;
            list_line.replace(0, 4, intToHex(location_counter_2, 4));
            list_line.replace(5, 1, to_string(programblocknumber[block_name]));
            list_line.replace(7, 6, list_line_copy.substr(6));
            list_line.replace(14, opcode.length(), opcode);
            list_line.replace(21, operand.length(), operand);

            listingFile << list_line << endl;
            continue;
        }
        if (opcode == "RESW" || opcode == "RESB" || opcode == "LTORG" || opcode == "EQU" || opcode == "USE")
        {
            if (opcode == "RESW")
            {
                previouslocationcounter = location_counter_2;
                pc = location_counter_2 + (3 * stoi(operand));

                list_line.replace(0, 4, intToHex(location_counter_2, 4));
                list_line.replace(5, 1, to_string(programblocknumber[block_name]));
                list_line.replace(7, label.length(), label);
                list_line.replace(14, opcode.length(), opcode);
                list_line.replace(21, operand.length(), operand);
                listingFile << list_line << endl;
                location_counter_2 += (3 * stoi(operand));
                programblockcounter_2[programblocknumber_2[block_name]] = location_counter_2;
            }
            else if (opcode == "RESB")
            {
                previouslocationcounter = location_counter_2;
                pc = location_counter_2 + (stoi(operand));

                list_line.replace(0, 4, intToHex(location_counter_2, 4));
                list_line.replace(5, 1, to_string(programblocknumber[block_name]));
                list_line.replace(7, label.length(), label);
                list_line.replace(14, opcode.length(), opcode);
                list_line.replace(21, operand.length(), operand);
                listingFile << list_line << endl;
                location_counter_2 += (stoi(operand));
                programblockcounter_2[programblocknumber_2[block_name]] = location_counter_2;
            }
            else
            {

                if (opcode != "LTORG")
                {
                    list_line.replace(0, 4, intToHex(location_counter_2, 4));
                }

                list_line.replace(5, 1, to_string(programblocknumber[block_name]));
                list_line.replace(7, label.length(), label);
                list_line.replace(14, opcode.length(), opcode);
                list_line.replace(21, operand.length(), operand);
                if (opcode == "EQU")
                {
                    if (symbol_table_2[label].first == 'A')
                        list_line.replace(5, 1, " ");
                }
                listingFile << list_line << endl;
                programblockcounter_2[programblocknumber_2[block_name]] = location_counter_2;
                continue;
            }

            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);
            text_record = begin_text_record(location_counter_2 + programblockaddress[programblocknumber_2[block_name]]);
            left = 60;
            continue;
        }

        if (opcode == "START")
        {
            programblocknumber_2["default"] = 1;
            string header_record = "H^";
            header_record += (pad_string(label) + "^");
            header_record += (intTo6Hex(stoi(operand)) + "^");
            starting_address = stoi(operand);
            location_counter_2 = stoi(operand);
            header_record += intTo6Hex(lengthofprogram);
            objectProgram.push_back(header_record);
            list_line.replace(0, 4, intToHex(location_counter_2, 4));
            list_line.replace(5, 1, to_string(programblocknumber[block_name]));
            list_line.replace(7, label.size(), label);
            list_line.replace(14, opcode.length(), opcode);
            list_line.replace(21, operand.length(), operand);
            listingFile << list_line << endl;
            continue;
        }

        if (opcode == "END")
        {
            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);

            list_line.replace(14, opcode.length(), opcode);
            list_line.replace(21, operand.length(), operand);
            listingFile << list_line << endl;

            string end_record = "E^";
            end_record += intTo6Hex((symbol_table_1[operand].second.first));
            objectProgram.push_back(end_record);
            break;
        }

        string search;
        if (opcode[0] == '+')
        {
            search = search_opcode_table_pass2(opcode.substr(1, opcode.length() - 1));
        }
        else
            search = search_opcode_table_pass2(opcode);

        int ni;
        string xbpe = "0000";
        if (search != "na")
        {
            if (search == "1")
                pc = location_counter_2 + 1;
            else if (search == "2")
                pc = location_counter_2 + 2;
            else
            {
                if (opcode[0] == '+')
                {
                    pc = location_counter_2 + 4;
                    xbpe[3] = '1';
                }
                else
                    pc = location_counter_2 + 3;
            }

            if (search == "1")
            {
                obj_code = opcode_table_pass2[opcode].second;
            }
            else if (search == "2")
            {
                if (operand.length() == 1)
                {
                    string registers_str(1, registers[operand[0]]);
                    obj_code = opcode_table_pass2[opcode].second + registers_str + "0";
                }
                else
                {
                    string registers_str1(1, registers[operand[0]]);
                    string registers_str2(1, registers[operand[2]]);
                    obj_code = opcode_table_pass2[opcode].second + registers_str1 + registers_str2;
                }
            }
            else
            {
                if (opcode[0] == '+')
                {
                    if (operand != " ")
                    {
                        if (operand.substr(operand.length() - 2, 2) == ",X")
                        {
                            xbpe[0] = '1';
                            operand = operand.substr(0, operand.length() - 2);
                        }

                        if (operand[0] == '@')
                        {
                            ni = 2;
                            search_symbol = search_symbol_table_2(operand.substr(1, operand.length() - 1));
                        }
                        else if (operand[0] == '#')
                        {
                            ni = 1;
                            search_symbol = search_symbol_table_2(operand.substr(1, operand.length() - 1));
                        }
                        else if (operand[0] != '=')
                        {
                            ni = 3;
                            search_symbol = search_symbol_table_2(operand);
                        }
                        if (search_symbol == 1)
                        {
                            int symbol_address = symbol_table_2[operand].second.first + programblockaddress[symbol_table_2[operand].second.second];
                            char symbol_type = symbol_table_2[operand].first;

                            string hex_xbpe = binary_to_hex(xbpe);
      
                            obj_code = intToHex(hexStringToInt(opcode_table_pass2[opcode.substr(1, opcode.length() - 1)].second) + ni, 2) + hex_xbpe + intToHex(symbol_address, 5);
                        }
                        else
                        {
                            int difference;
                            if (operand[0] == '@' || operand[0] == '#')
                            {
                                difference = stoi(operand.substr(1, operand.length() - 1));
                            }
                            else
                                difference = stoi(operand);
                            string hex_xbpe = binary_to_hex(xbpe);
                            obj_code = intToHex(hexStringToInt(opcode_table_pass2[opcode.substr(1, opcode.length() - 1)].second) + ni, 2) + hex_xbpe + intToHex(difference, 5);
                        }
                    }
                    else
                    {
                        obj_code = intToHex(hexStringToInt(opcode_table_pass2[opcode.substr(1, opcode.length() - 1)].second) + 3, 2) + "000000";
                    }
                }
                else
                {
                    if (operand != " ")
                    {
                        if (operand.substr(operand.length() - 2, 2) == ",X")
                        {
                            xbpe[0] = '1';
                            operand = operand.substr(0, operand.length() - 2);
                        }

                        if (operand[0] == '@')
                        {
                            ni = 2;
                            search_symbol = search_symbol_table_2(operand.substr(1, operand.length() - 1));
                            operand = operand.substr(1);
                        }
                        else if (operand[0] == '#')
                        {
                            ni = 1;
                            search_symbol = search_symbol_table_2(operand.substr(1, operand.length() - 1));
                            operand = operand.substr(1);
                        }
                        else if (operand[0] != '=')
                        {
                            ni = 3;
                            search_symbol = search_symbol_table_2(operand);
                        }

                        if (search_symbol == 1)
                        {
                            int symbol_address = symbol_table_2[operand].second.first + programblockaddress[symbol_table_2[operand].second.second];
                            char symbol_type = symbol_table_2[operand].first;
                            pc = location_counter_2 + 3;
                            if (symbol_type == 'R')
                            {
                                int difference = symbol_address - (pc + programblockaddress[programblocknumber[block_name]]);
                                if (difference >= -2048 && difference < 2047)
                                {
                                    xbpe[2] = '1';
                                }
                                else if(difference<=4095 && difference>=0)
                                {
                                    if(symbol_table_2.find("BASE")==symbol_table_2.end()){
                                        errorFile<<"Error: Assembler directive BASE not found for handling large displacement"<<endl;
                                    }
                                    xbpe[1] = '1';
                                    difference = symbol_address - (base + programblockaddress[baseblock]);
                                }
                                else{
                                    errorFile<<"Error: For relative addressing using BASE, the displacement is out of bounds"<<endl;
                                }

                                string hex_xbpe = binary_to_hex(xbpe);
                                obj_code = intToHex(hexStringToInt(opcode_table_pass2[opcode].second) + ni, 2) + hex_xbpe + intToHex(difference, 3);
                            }
                            else
                            {
                                string hex_xbpe = binary_to_hex(xbpe);
                                obj_code = intToHex(hexStringToInt(opcode_table_pass2[opcode].second) + ni, 2) + hex_xbpe + intToHex(symbol_address, 3);
                            }
                        }
                        else
                        {
                            int difference;
                            if (operand[0] == '@' || operand[0] == '#')
                            {
                                difference = stoi(operand.substr(1, operand.length() - 1));
                            }
                            else
                                difference = stoi(operand);
                            string hex_xbpe = binary_to_hex(xbpe);
                            obj_code = intToHex(hexStringToInt(opcode_table_pass2[opcode].second) + ni, 2) + hex_xbpe + intToHex(difference, 3);
                        }
                    }
                    else
                    {
                        obj_code = intToHex(hexStringToInt(opcode_table_pass2[opcode].second) + 3, 2) + "0000";
                    }
                }
            }
            previouslocationcounter = location_counter_2;
            if (search == "1")
                location_counter_2 += 1;
            else if (search == "2")
                location_counter_2 += 2;
            else
            {
                if (opcode[0] == '+')
                    location_counter_2 += 4;
                else
                    location_counter_2 += 3;
            }
        }
        else if (opcode == "BYTE")
        {
            if (operand[0] == 'C')
            {
                previouslocationcounter = location_counter_2;
                pc = location_counter_2 + (operand.length() - 3);
                obj_code = asciiToHex(operand.substr(2, operand.length() - 3));
                location_counter_2 += (operand.length() - 3);
            }
            else
            {
                previouslocationcounter = location_counter_2;
                pc = location_counter_2 + (operand.length() - 3) / 2;
                obj_code = operand.substr(2, operand.length() - 3);
                location_counter_2 += (operand.length() - 3) / 2;
            }
        }
        else if (opcode == "WORD")
        {
            obj_code = intToHex(stoi(operand), 2);
        }
        else if (opcode[0] == '=')
        {
            if (opcode[1] == 'C')
            {
                previouslocationcounter = location_counter_2;
                pc = location_counter_2 + (opcode.length() - 4);
                obj_code = asciiToHex(opcode.substr(3, opcode.length() - 4));
                location_counter_2 += (opcode.length() - 4);
            }
            else
            {
                previouslocationcounter = location_counter_2;
                pc = location_counter_2 + (opcode.length() - 4) / 2;
                obj_code = opcode.substr(3, opcode.length() - 4);
                location_counter_2 += (opcode.length() - 4) / 2;
            }
        }
        else{
            errorFile<<"Opcode not found at location counter(in dec): "<<location_counter_2<<endl;
        }

        list_line.replace(0, 4, intToHex(previouslocationcounter, 4));
        list_line.replace(5, 1, to_string(programblocknumber[block_name]));
        list_line.replace(7, label.length(), label);
        list_line.replace(14, parse_line[2].length(), parse_line[2]);
        list_line.replace(21, parse_line[3].length(), parse_line[3]);
        list_line.replace(32, obj_code.length(), obj_code);
        listingFile << list_line << endl;

        if (opcode[0] == '+' && search_symbol == 1)
        {
            mod_record.push_back(location_counter_2 - 4);
        }

        if (obj_code == "")
            continue;
        if (obj_code.length() > left)
        {

            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);
            if (search == "1")
            {
                text_record = begin_text_record(location_counter_2 - 1 + programblockaddress[programblocknumber_2[block_name]]);
            }
            else if (search == "2")
            {
                text_record = begin_text_record(location_counter_2 - 2 + programblockaddress[programblocknumber_2[block_name]]);
            }
            else
            {
                if (opcode[0] == '+')
                {
                    text_record = begin_text_record(location_counter_2 - 4 + programblockaddress[programblocknumber_2[block_name]]);
                }
                else
                {
                    text_record = begin_text_record(location_counter_2 - 3 + programblockaddress[programblocknumber_2[block_name]]);
                }
            }

            text_record += (obj_code + "^");
            left = 60 - obj_code.length();
        }
        else
        {
            if (left == 60)
                text_record = begin_text_record(previouslocationcounter + programblockaddress[programblocknumber_2[block_name]]);
            text_record += (obj_code + "^");
            left -= obj_code.length();
        }
        programblockcounter_2[programblocknumber_2[block_name]] = location_counter_2;
    }
    for (auto record : objectProgram)
    {
        if (record[0] == 'H' || record[0] == 'E' || record.substr(9, 2) != "00")
            objectFile << record << endl;
    }
}
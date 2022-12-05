#include "csv.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <limits>

#define NONE -1

using namespace std;

string chr2str(const char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

char *str2chr(string str){
    int size = str.length();
    char *chr = new char[size];
    for (int i = 0; i < size; i++) {
        chr[i] = str[i];
    }
    chr[size] = '\x00';
    return chr;
}

void CSV::gnol() {
    string line;
    while (getline(_file, line))
        ++nol;
}

fstream& GotoLine(fstream& file, unsigned int num){
    file.seekg(ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return file;
}


CSV::CSV(string file_name, const char* sep=","): file_name(file_name), sep(sep) {
    try {
        _file.open(((const char *)(str2chr(file_name))));
        cout << ((const char *)(str2chr(file_name))) << endl;
        if(_file.is_open()) {
            //this->gnol();
            bool c_cont = true;
            string line;
            getline(_file,line);
            this->set_columns(line);
        }
        else {
            throw(0);
        }
    }
    catch (...) {
        cout << "[ERROR] - File is not opened!" << endl;
    }
}

CSV::CSV(const char* file_name, const char* sep): sep(sep) {
    try {
        int size = strlen(file_name);
        this->file_name = chr2str(file_name, size);
        _file.open(file_name);
        if(_file.is_open()) {
            //this->gnol();
            string line;
            getline(_file,line);
            this->set_columns(line);
        }
        else {
            throw(0);
        }
    }
    catch (...) {
        cout << "[ERROR] - File is not opened!" << endl;
    }
}


void CSV::set_columns(string line) {
    int size = line.length();
    string name;
    for (int i =0; i< size; i++) {
        if(line[i] != '"') {
            if (line[i] != *(this->sep)){
                name = name + line[i];
            }
            else {
                this->columns.push_back(name);
                name = "";
            }
        }
        if (i == (size-1) && line[i] != *(this->sep)) {
            this->columns.push_back(name);
            name = "";
        }
    }
    
}


vector<string> CSV::operator[](int i) {
    GotoLine((fstream&)(_file), i+2);
    string line;
    getline(_file,line);
    cout << line << endl;

    vector<string> _data;
    int size = line.length();
    string name;
    for (int i =0; i< size; i++) {
        if(line[i] != '"') {
            if (line[i] != *(this->sep)){
                name = name + line[i];
            }
            else {
                _data.push_back(name);
                name = "";
            }
        }
    }

    return _data;
}


vector<vector<string>> CSV::query(int column, string value, int limit = NONE, int cmp = EQUAL) {
    if (control_columns(column)) {
        string line;
        
        vector<vector<string>> _data;
        cout<<limit<<endl;
        if (limit == NONE) {
            cout<<"unlimit"<<endl;
            int size;
            while (getline(_file, line)) {
                vector<string> _current;
                string val;
                size = line.length();
                
                for (int i =0; i< size; i++) {
                    if(line[i] != '"') {
                        if (line[i] != *(this->sep)){
                            val = val + line[i];
                        }
                        else {
                            _current.push_back(val);
                            val = "";
                        }
                    }
                    if (i == size - 1) {
                        _current.push_back(val);
                        val = "";
                    }
                }

                if (_current.size() != 0) {
                    if (cmp == EQUAL){
                        if(!strcmp(str2chr(_current[column]), str2chr(value))) {
                            _data.push_back(_current);
                            cout<<line<<endl;
                        }
                    }
                    else if(cmp == INCLUDE) {
                        const char* _column = str2chr(_current[column]);
                        const char* _value = str2chr(value);

                        
                        if(_current[column].find(_value) != string::npos) {
                            _data.push_back(_current);
                            cout<<line<<endl;
                        }
                    }
                    
                }
                else
                    break;
            }
            
        }

        else {
            cout<<"limit"<<endl;
            int size;
            int a = 0;
            while (a != limit) {
                getline(_file, line);
                vector<string> _current;
                string val;
                size = line.length();
                
                for (int i =0; i< size; i++) {
                    if(line[i] != '"') {
                        if (line[i] != *(this->sep)){
                            val = val + line[i];
                        }
                        else {
                            _current.push_back(val);
                            val = "";
                        }
                    }
                }
                
                if (_current.size() != 0) {
                    if (cmp == EQUAL){
                        if(!strcmp(str2chr(_current[column]), str2chr(value))) {
                            _data.push_back(_current);
                            cout<<line<<endl;
                            a++;
                        }
                    }
                    else if(cmp == INCLUDE) {
                        if(_current[column].find(value) != string::npos) {
                            _data.push_back(_current);
                            cout<<line<<endl;
                            a++;
                        }
                    }
                    
                }
                else
                    break;
                
            }
        }

        return _data;
    }
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(int column, const char* value, int limit = NONE, int cmp = EQUAL) {
    if(control_columns(column)) {
        return query(column, chr2str(value,strlen(value)), limit, cmp);
    }
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(string column, const char* value, int limit = NONE, int cmp = EQUAL) {
    if (control_columns(column)) 
        return this->query(this->get_column(column),chr2str(value,strlen(value)),limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(const char* column, const char* value, int limit = NONE, int cmp = EQUAL){
    if (control_columns(column)) 
        return this->query(this->get_column(column), chr2str(value, strlen(value)), limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(string column, string value, int limit = NONE, int cmp = EQUAL) {
    if(control_columns(column))
        return this->query(this->get_column(column), value, limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(const char* column, string value, int limit = NONE, int cmp = EQUAL) {
    if(control_columns(column)) 
        return this->query(this->get_column(column), value, limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}



vector<vector<string>> CSV::query(vector<vector<string>> query, int column, string value, int limit, int cmp) {
    if(control_columns(column)) {
        vector<vector<string>> _data;
        int a;
        for (auto v : query) {
            if (cmp == EQUAL) {
                if(!strcmp(str2chr(v[column]), str2chr(value))) {
                    _data.push_back(v);
                    if (limit != NONE)
                        a++;
                }
            }
            else if (cmp == INCLUDE) {
                if(v.size() != 0) {
                    if (v[column].find(value) != string::npos) {
                        _data.push_back(v);
                        if (limit != NONE)
                        a++;
                    }
                }
                
            }

            if(a==limit)
                break;
            
        }

        return _data;
    }
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(vector<vector<string>> query, int column, const char* value, int limit, int cmp) {
    if(control_columns(column)) 
        return this->query(query, column, chr2str(value, strlen(value)), limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(vector<vector<string>> query, string column, const char* value, int limit, int cmp) {
    if(control_columns(column)) 
        return this->query(query, get_column(column), chr2str(value, strlen(value)), limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(vector<vector<string>> query, const char* column, const char* value, int limit, int cmp) {
    if(control_columns(column)) 
        return this->query(query, get_column(column), chr2str(value, strlen(value)), limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(vector<vector<string>> query, string column, string value, int limit, int cmp) {
    if(control_columns(column)) 
        return this->query(query, get_column(column), value, limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}

vector<vector<string>> CSV::query(vector<vector<string>> query, const char* column, string value, int limit, int cmp) {
    if(control_columns(column)) 
        return this->query(query, get_column(column), value, limit, cmp);
    else {
        vector<vector<string>> a;
        vector<string> b;
        string c = "";
        b.push_back(c);
        a.push_back(b);
        return a;
    }
}




int CSV::control_columns(const char* col) {
    for (auto & c : this->columns) {
        if(!strcmp(str2chr(c),col))
            return 1;
    }
    return 0;
}

int CSV::control_columns(string col) {
    for (auto & c : this->columns) {
        if(!strcmp(str2chr(c),str2chr(col)))
            return 1;
    }
    return 0;
}

int CSV::control_columns(int col) {
    if (col <= (this->columns).size())
        return 1;
    return 0;
}


int CSV::get_column(string column) {
    int col = 0;
    for (auto & c : this->columns) {
        if(!strcmp(str2chr(c),str2chr(column)))
            break;
        col++;
    }
    return col;
}

int CSV::get_column(const char* column) {
    int col = 0;
    for (auto & c : this->columns) {
        if(!strcmp(str2chr(c),column))
            break;
        col++;
    }
    return col;
}
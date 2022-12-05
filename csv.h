#ifndef _CSV_
#define _CSV_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace csv
{
    #define NONE -1
    #define EQUAL 0
    #define INCLUDE 1
} // namespace csv


class CSV {

    private:
        string file_name;
        const char *sep = new char();

        ifstream _file;

        vector<string> columns;
        vector<vector<string>> data;

        int nol;

        void set_columns(string line);
        void gnol();
        int control_columns(const char* col);
        int control_columns(string col);
        int control_columns(int col);
        int get_column(string column);
        int get_column(const char* column);

    public:
        CSV(string file_name, const char* sep);
        CSV(const char* file_name, const char* sep);

        vector<string> operator[](int i);

        vector<vector<string>> query(int column, string value, int limit, int cmp);
        vector<vector<string>> query(int column, const char* value, int limit, int cmp);

        vector<vector<string>> query(string column, const char* value, int limit, int cmp);
        vector<vector<string>> query(const char* column, const char* value, int limit, int cmp);

        vector<vector<string>> query(string column, string value, int limit, int cmp);
        vector<vector<string>> query(const char* column, string value, int limit, int cmp);

        

        vector<vector<string>> query(vector<vector<string>> query, int column, string value, int limit, int cmp);
        vector<vector<string>> query(vector<vector<string>> query, int column, const char* value, int limit, int cmp);

        vector<vector<string>> query(vector<vector<string>> query, string column, const char* value, int limit, int cmp);
        vector<vector<string>> query(vector<vector<string>> query, const char* column, const char* value, int limit, int cmp);

        vector<vector<string>> query(vector<vector<string>> query, string column, string value, int limit, int cmp);
        vector<vector<string>> query(vector<vector<string>> query, const char* column, string value, int limit, int cmp);

};



#endif
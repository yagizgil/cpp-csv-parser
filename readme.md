<h1>C++ CSV Parser</h1>

<h3>Documentation</h3>

Example CSV:
```perl
"id","name","price"
"1","bryan","45"
"2","paul","142"
"3","mark","58"
"4","daniel","79"
"5","emma","128"
"6","zack","235"
"7","zack","330"
"8","bryan","235"
"9","paul","842"
"10","darwin","147"
"11","bryan","45"
```

maic.cpp
```c++
#include <iostream>
#include <string>
#include <vector>
#include "csv.h"

using namespace std;

int main() {
  CSV my_file("file.csv",",");
  
  my_file[2]; // return "3","mark","58" as vector<string> 
  
  my_file.query("name","bryan");  // return 3 elements as vector<vector<string>>
  /*
  "1","bryan","45"
  "8","bryan","235"
  "11","bryan","45"
  */
  
  my_file.query(my_file.query("name","bryan"), "price", "45"); // return 2 elements as vector<vector<string>>
  /*
  "1","bryan","45"
  "11","bryan","45"
  */
  
  return 0;
}
```

query functions
```c++
query(int column, string value, int limit, int cmp);
query(int column, const char* value, int limit, int cmp);

query(string column, const char* value, int limit, int cmp);
query(const char* column, const char* value, int limit, int cmp);

query(string column, string value, int limit, int cmp);
query(const char* column, string value, int limit, int cmp);

        

query(vector<vector<string>> query, int column, string value, int limit, int cmp);
query(vector<vector<string>> query, int column, const char* value, int limit, int cmp);

query(vector<vector<string>> query, string column, const char* value, int limit, int cmp);
query(vector<vector<string>> query, const char* column, const char* value, int limit, int cmp);

query(vector<vector<string>> query, string column, string value, int limit, int cmp);
query(vector<vector<string>> query, const char* column, string value, int limit, int cmp);
```

#pragma once
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

struct expression {
    int firstRow;
    int firstColumn;
    int secondRow;
    int secondColumn;
    int resultRow;
    int resultColumn;
    char operation;
    int failCount = 0;
    bool wasRowsInit = false;
};

struct ParsedTable {
    string header;
    map<string, int> headerMap;
    map<int, int> rowMap;
    vector<vector<int>> values;
    queue<expression> expressionsQueue;
};

ParsedTable *parse(const string& file);
void printTable(ParsedTable *table);
string removeEmptyStart(string line);
vector<string> splitByCommas(string line);
bool isNumber(string s);
int makeOperation(int a, int b, char op);
char parseOperator(string &expr);
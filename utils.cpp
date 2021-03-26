#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

bool isOnlyFilePassed(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of parameters! Pass only the path to the file or directory!\n");
        return false;
    }
    if (access(argv[1], F_OK) != 0) {
        printf("No such file or directory: %s\n", argv[1]);
        return false;
    }
    return true;
}

bool isCsvFilePassed(const string& file) {
    if (!file.ends_with(".csv")) {
        printf("Not a csv file was passed!\n");
        return false;
    }
    return true;
}

string removeEmptyStart(string line) {
    int start = 0;
    while (start < line.size() && line[start] < 33) {
        ++start;
    }
    line.erase(0, start);
    return line;
}

vector<string> splitByCommas(string line) {
    vector<string> result;
    int comma;
    line = removeEmptyStart(line);
    while ((comma = line.find(',')) != string::npos) {
        string value = line.substr(0, comma);
        if (!value.empty()) {
            result.push_back(value);
        }
        line.erase(0, comma + 1);
    }
    result.push_back(line);
    return result;
}

bool isNumber(string s) {
    for (char c: s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int makeOperation(int a, int b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        default:
            if (b == 0) {
                throw "Division by zero!";
            }
            return a / b;
    }
}

char parseOperator(string &expr) {
    char op = expr.substr(0, 1)[0];
    if (op == '+' || op == '-' || op == '*' || op == '/') {
        expr.erase(0, 1);
        return op;
    }
    throw "Unknown operation sign!";
}
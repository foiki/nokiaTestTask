#include "parsedTable.h"

map<string, int> initHeaderMap(string line) {
    vector<string> headerSplit = splitByCommas(move(line));
    map<string, int> result;
    int i = 0;
    for (const string& a: headerSplit) {
        result[a] = i;
        ++i;
    }
    return result;
}

void printIfIncorrectTable() {
    printf("%s", "Incorrect table!");
}

/**
 * Parse correct column number from expression
 * @param headerMap - collection that maps a column to a table to a column in an array
 * @param expr - expression to find column number
 * @return number of row in matrix
 */
int initColumnNumber(const map<string, int>& headerMap, string &expr) {
    if (expr.length() == 0) {
        throw "Incorrect expression!";
    }
    int columnLength = 0;
    while (columnLength < expr.length() && !isdigit(expr[columnLength])) {
        ++columnLength;
    }
    if (columnLength == 0) {
        throw "Incorrect expression!";
    }
    string column = expr.substr(0, columnLength);
    expr.erase(0, columnLength);
    if (!headerMap.contains(column)) {
        throw "Wrong column number!";
    }
    return headerMap.at(column);
}

/**
 * Parse correct row number from expression
 * @param rowMap - collection that maps a row to a table to a row in an array
 * @param expr - expression to find row number
 * @return number of row in table
 */
int parseRowNumber(const map<int, int>& rowMap, string &expr) {
    if (expr.length() == 0) {
        throw "Incorrect expression!";
    }
    int rowLength = 0;
    while (rowLength < expr.length() && isdigit(expr[rowLength])) {
        ++rowLength;
    }
    if (rowLength == 0) {
        throw "Incorrect expression!";
    }
    int row = stoi(expr.substr(0, rowLength));
    expr.erase(0, rowLength);
    return row;
}

/**
 * Calculates all expressions in the queue
 * @param table - table with values
 */
void calculateExpressions(ParsedTable *table) {
    unsigned long maxFailsCount = table->headerMap.size() * table->rowMap.size();
    while (!table->expressionsQueue.empty() && table->expressionsQueue.front().failCount < maxFailsCount) {
        expression expr = table->expressionsQueue.front();
        table->expressionsQueue.pop();
        if (!expr.wasRowsInit) {
            if (!table->rowMap.contains(expr.firstRow) || !table->rowMap.contains(expr.secondRow)) {
                throw "Wrong row index!";
            }
            expr.firstRow = table->rowMap.at(expr.firstRow);
            expr.secondRow = table->rowMap.at(expr.secondRow);
            expr.wasRowsInit = true;
        }
        if (table->values[expr.firstRow][expr.firstColumn] == -1 || table->values[expr.secondRow][expr.secondColumn] == -1) {
            expr.failCount++;
            table->expressionsQueue.push(expr);
        } else {
            table->values[expr.resultRow][expr.resultColumn] = makeOperation(table->values[expr.firstRow][expr.firstColumn], table->values[expr.secondRow][expr.secondColumn], expr.operation);
        }
    }
    if (!table->expressionsQueue.empty()) {
        throw "Expressions infinity cycle!";
    }
}

/**
 * Adds new expression to the queue
 * @param table - table with values
 * @param line - string with expression
 * @param rowCounter - the row where the expression is located
 * @param columnCounter - the column where the expression is located
 */
void addExpression(ParsedTable *table, string line, int rowCounter, int columnCounter) {
    expression newExpression = expression();
    line.erase(0, 1);
    newExpression.resultRow = rowCounter;
    newExpression.resultColumn = columnCounter;
    newExpression.firstColumn = initColumnNumber(table->headerMap, line);
    newExpression.firstRow = parseRowNumber(table->rowMap, line);
    newExpression.operation = parseOperator(line);
    newExpression.secondColumn = initColumnNumber(table->headerMap, line);
    newExpression.secondRow = parseRowNumber(table->rowMap, line);
    table->expressionsQueue.push(newExpression);
}

/**
 * Puts the numbers directly in the table, and the expressions in the queue for calculation
 * @param table - table with values
 * @param lineSplit - vector with expressions and numbers
 * @param rowCounter - row number
 */
void parseLineIntoTable(ParsedTable *table, const vector<string>& lineSplit, int rowCounter) {
    vector<int> newLine;
    int columnCounter = 0;
    for (auto & i : lineSplit) {
        if (isNumber(i)) {
            newLine.push_back(stoi(i));
        } else if (i.starts_with("=")) {
            addExpression(table, i, rowCounter, columnCounter);
            newLine.push_back(-1);
        } else {
            throw "Wrong expression!";
        }
        ++columnCounter;
    }
    table->values.push_back(newLine);
}

void initParsedTable(ParsedTable *table, ifstream &fin) {
    int rowCounter = 0;
    while (!fin.eof()) {
        if (fin.eof()) {
            break;
        }
        string newLine;
        fin >> newLine;
        if (newLine.empty()) {
            break;
        }
        vector<string> lineSplit = splitByCommas(newLine);
        if (lineSplit.size() != table->headerMap.size() + 1) {
            throw "Wrong column size!";
        }
        table->rowMap[stoi(lineSplit.front())] = rowCounter;
        lineSplit.erase(lineSplit.begin());
        parseLineIntoTable(table, lineSplit, rowCounter);
        ++rowCounter;
    }
    calculateExpressions(table);
}

/**
 * Parse table from csv file and calculates simple expressions in it
 * @param file to take table
 * @return calculated table
 */
ParsedTable *parse(const string& file) {
    ifstream fin(file);
    if (fin.eof()) {
        return nullptr;
    }
    auto *table = new ParsedTable();
    fin >> table->header;
    if (fin.eof()) {
        return nullptr;
    }
    table->headerMap = initHeaderMap(table->header);
    try {
        initParsedTable(table, fin);
    } catch (invalid_argument &e) {
        printIfIncorrectTable();
        return nullptr;
    } catch (const char* msg) {
        printf("%s\n", msg);
        printIfIncorrectTable();
        return nullptr;
    }
    return table;
}

void printTable(ParsedTable *table) {
    if (table == nullptr) {
        return;
    }
    for (const auto& [key, value] : table->headerMap) {
        printf(",%s", key.data());
    }
    printf("\n");
    auto rowIterator = table->rowMap.begin();
    for (int i = 0; i < table->values.size(); ++i) {
        printf("%d", rowIterator->first);
        rowIterator++;
        for (int j = 0; j < table->values[i].size(); ++j) {
            printf(",%d", table->values[i][j]);
        }
        printf("\n");
    }
}
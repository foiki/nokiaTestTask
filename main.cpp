#include <algorithm>
#include "parsedTable.h"

using namespace std;

bool isOnlyFilePassed(int argc, char *argv[]);
bool isCsvFilePassed(const string& file);

int main(int argc, char *argv[]) {
    if (!isOnlyFilePassed(argc, argv) || !isCsvFilePassed(argv[1])) {
        return 0;
    }
    ParsedTable *table = parse(argv[1]);
    printTable(table);
    delete table;
    return 0;
}

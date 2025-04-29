#include "dataStruct.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <limits>

using namespace std;

int main() {
    vector<DataStruct> data;

    while (!cin.eof()) {
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        copy(
            istream_iterator<DataStruct>(cin),
            istream_iterator<DataStruct>(),
            back_inserter(data)
        );
    }

    sort(data.begin(), data.end(), compareData);

    copy(
        data.begin(),
        data.end(),
        ostream_iterator<DataStruct>(cout, "\n")
    );

    return 0;
}
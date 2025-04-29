#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <complex>
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

struct DataStruct {
    unsigned long long key1;
    complex<double> key2;
    string key3;
};

struct DelimiterIO {
    char expected;
};

struct BinaryIO {
    unsigned long long& value;
};

struct ComplexIO {
    complex<double>& value;
};

struct StringIO {
    string& value;
};

bool compareData(const DataStruct& a, const DataStruct& b);

class FormatGuard {
public:
    explicit FormatGuard(ios& stream);
    ~FormatGuard();

private:
    ios& stream_;
    streamsize width_;
    char fill_;
    streamsize precision_;
    ios::fmtflags flags_;
};

istream& operator>>(istream& in, DelimiterIO&& delim);
istream& operator>>(istream& in, BinaryIO&& bin);
istream& operator>>(istream& in, ComplexIO&& cmp);
istream& operator>>(istream& in, StringIO&& str);
istream& operator>>(istream& in, DataStruct& ds);
ostream& operator<<(ostream& out, const DataStruct& ds);

#endif
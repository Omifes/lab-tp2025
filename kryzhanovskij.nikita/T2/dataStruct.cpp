#include "dataStruct.h"
#include <cmath>
#include <sstream>

using namespace std;

FormatGuard::FormatGuard(ios& stream) :
    stream_(stream),
    width_(stream.width()),
    fill_(stream.fill()),
    precision_(stream.precision()),
    flags_(stream.flags()) {
}

FormatGuard::~FormatGuard() {
    stream_.width(width_);
    stream_.fill(fill_);
    stream_.precision(precision_);
    stream_.flags(flags_);
}

bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    const double a_mag = abs(a.key2);
    const double b_mag = abs(b.key2);
    if (a_mag != b_mag) return a_mag < b_mag;
    return a.key3.length() < b.key3.length();
}

istream& operator>>(istream& in, DelimiterIO&& delim) {
    istream::sentry sentry(in);
    if (sentry) {
        char c;
        if (in >> c && c != delim.expected) {
            in.setstate(ios::failbit);
        }
    }
    return in;
}

istream& operator>>(istream& in, BinaryIO&& bin) {
    istream::sentry sentry(in);
    if (sentry) {
        in >> DelimiterIO{ '0' };
        char base;
        in >> base;
        if (base != 'b' && base != 'B') {
            in.setstate(ios::failbit);
            return in;
        }

        string bits;
        char bit;
        while (in.get(bit) && (bit == '0' || bit == '1')) {
            bits += bit;
        }
        in.unget();

        if (!bits.empty()) {
            bin.value = stoull(bits, nullptr, 2);
        }
        else {
            in.setstate(ios::failbit);
        }
    }
    return in;
}

istream& operator>>(istream& in, ComplexIO&& cmp) {
    istream::sentry sentry(in);
    if (sentry) {
        double real, imag;
        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' }
        >> real >> imag >> DelimiterIO{ ')' };
        if (in) {
            cmp.value = complex<double>(real, imag);
        }
    }
    return in;
}

istream& operator>>(istream& in, StringIO&& str) {
    istream::sentry sentry(in);
    if (sentry) {
        getline(in >> DelimiterIO{ '"' }, str.value, '"');
    }
    return in;
}

istream& operator>>(istream& in, DataStruct& ds) {
    istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

    while (true) {
        string key;
        in >> key;
        if (!in) break;

        if (key == "key1") {
            in >> BinaryIO{ temp.key1 };
            hasKey1 = true;
        }
        else if (key == "key2") {
            in >> ComplexIO{ temp.key2 };
            hasKey2 = true;
        }
        else if (key == "key3") {
            in >> StringIO{ temp.key3 };
            hasKey3 = true;
        }
        else {
            in.setstate(ios::failbit);
            break;
        }

        if (in >> DelimiterIO{ ':' } && in.peek() == ')') break;
    }

    in >> DelimiterIO{ ')' };

    if (hasKey1 && hasKey2 && hasKey3) {
        ds = move(temp);
    }
    else {
        in.setstate(ios::failbit);
    }

    return in;
}

ostream& operator<<(ostream& out, const DataStruct& ds) {
    ostream::sentry sentry(out);
    if (!sentry) return out;
    FormatGuard guard(out);

    out << "(:key1 0b";
    if (ds.key1 == 0) {
        out << '0';
    }
    else {
        string binary = bitset<64>(ds.key1).to_string();
        binary.erase(0, binary.find_first_not_of('0'));
        out << binary;
    }

    out << ":key2 #c(" << ds.key2.real() << " " << ds.key2.imag() << ")";
    out << ":key3 \"" << ds.key3 << "\":)";

    return out;
}
// std::pair<CWideString,long>::pair(const pair&) -- copies the wide string, then the long value.
// Retail 0x005b12f4, 27 bytes; xref 0x005b17c6 passes pair<CWideString,long> as the second value of a _Pair_base
// and calls this copy constructor. The propagated CComboMultiplierData catalog label is therefore not used here.
#include "fable_string.h"
namespace std {
template<class A, class B> struct pair;
template<> struct pair<CWideString, long> {
    CWideString first;
    long second;
    pair(const pair& other);
};
pair<CWideString, long>::pair(const pair& other)
    : first(other.first), second(other.second) {}
}
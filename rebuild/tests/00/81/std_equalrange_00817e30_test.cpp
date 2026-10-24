// behaviour test for 0x00817e30 (std::__equal_range over THitLocationIntersectionElement)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
struct CHitLocation;
struct C3DVector { float X, Y, Z; };                // 12 bytes
namespace {
struct THitLocationIntersectionElement {        // 32 bytes
    long          key;                          // +0x00
    CHitLocation* hit_location;                 // +0x04
    C3DVector     intersection_pos;             // +0x08
    C3DVector     outward_normal;               // +0x14
};
struct THitLocationIntersectionElementPred {
    bool operator()(const THitLocationIntersectionElement& a, long b) const { return a.key < b; }
    bool operator()(long a, const THitLocationIntersectionElement& b) const { return a < b.key; }
};
}
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class ForwardIter, class Tp, class Compare, class Distance>
ForwardIter __fastcall __lower_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = last - first;
    while (len > 0) {
        Distance half = len >> 1;
        ForwardIter middle = first + half;
        if (comp(*middle, val)) { first = middle + 1; len = len - half - 1; }
        else len = half;
    }
    return first;
}
template <class ForwardIter, class Tp, class Compare, class Distance>
ForwardIter __fastcall __upper_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = last - first;
    while (len > 0) {
        Distance half = len >> 1;
        ForwardIter middle = first + half;
        if (comp(val, *middle)) len = half;
        else { first = middle + 1; len = len - half - 1; }
    }
    return first;
}
template <class ForwardIter, class Tp, class Compare, class Distance>
pair<ForwardIter, ForwardIter> __fastcall
__equal_range(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = last - first;
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        middle += half;
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else if (comp(val, *middle))
            len = half;
        else {
            return pair<ForwardIter, ForwardIter>(__lower_bound(first, middle, val, comp, (Distance*)0),
                                                  __upper_bound(middle + 1, first + len, val, comp, (Distance*)0));
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}
template pair<const THitLocationIntersectionElement*, const THitLocationIntersectionElement*> __fastcall
__equal_range(const THitLocationIntersectionElement*, const THitLocationIntersectionElement*, const long&,
              THitLocationIntersectionElementPred, int*);
}

int main()
{
    THitLocationIntersectionElement e[7];
    long keys[7] = { 1, 3, 3, 3, 5, 8, 8 };
    for (int i = 0; i < 7; ++i) { e[i].key = keys[i]; e[i].hit_location = 0; }
    typedef const THitLocationIntersectionElement* It;
    std::pair<It, It> r3 = std::__equal_range((It)e, (It)(e + 7), 3L, THitLocationIntersectionElementPred(), (int*)0);
    std::pair<It, It> r8 = std::__equal_range((It)e, (It)(e + 7), 8L, THitLocationIntersectionElementPred(), (int*)0);
    std::pair<It, It> r4 = std::__equal_range((It)e, (It)(e + 7), 4L, THitLocationIntersectionElementPred(), (int*)0);
    std::pair<It, It> r1 = std::__equal_range((It)e, (It)(e + 7), 1L, THitLocationIntersectionElementPred(), (int*)0);
    std::pair<It, It> r0 = std::__equal_range((It)e, (It)e, 1L, THitLocationIntersectionElementPred(), (int*)0);
    std::pair<It, It> r9 = std::__equal_range((It)e, (It)(e + 7), 9L, THitLocationIntersectionElementPred(), (int*)0);
    int ok = r3.first == e + 1 && r3.second == e + 4 && r8.first == e + 5 && r8.second == e + 7
          && r4.first == e + 4 && r4.second == e + 4 && r1.first == e && r1.second == e + 1
          && r0.first == e && r0.second == e && r9.first == e + 7 && r9.second == e + 7;
    if (!ok) { printf("00817e30_TEST FAIL\n"); return 1; }
    printf("00817e30_TEST PASS\n");
    return 0;
}
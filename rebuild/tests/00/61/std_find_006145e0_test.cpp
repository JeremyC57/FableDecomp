// 006145e0 behaviour test for std::__find over a list of CUnprovenPair8_006145e0
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
struct CUnprovenPair8_006145e0 { long First; long Second; };
inline bool operator==(const CUnprovenPair8_006145e0& x, const CUnprovenPair8_006145e0& y) { return x.First == y.First && x.Second == y.Second; }
struct input_iterator_tag { };
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
struct Node : NodeBase { CUnprovenPair8_006145e0 Data; };
struct Iterator {
    NodeBase* N;
    Iterator(NodeBase* n) : N(n) { }
    bool operator!=(const Iterator& o) const { return N != o.N; }
    CUnprovenPair8_006145e0& operator*() const { return ((Node*)N)->Data; }
    Iterator& operator++() { N = N->Next; return *this; }
};
template <class II, class T> II __fastcall Find(II first, II last, const T& val, const input_iterator_tag&)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
static void run()
{
    Node header, a, b, c;
    header.Next = &a; a.Next = &b; b.Next = &c; c.Next = &header;
    a.Data.First = 1; a.Data.Second = 2; b.Data.First = 3; b.Data.Second = 4; c.Data.First = 3; c.Data.Second = 5;
    CUnprovenPair8_006145e0 v1 = { 3, 5 }, v2 = { 3, 6 }, v3 = { 1, 2 };
    CHECK(Find(Iterator(header.Next), Iterator(&header), v1, input_iterator_tag()).N == &c);
    CHECK(Find(Iterator(header.Next), Iterator(&header), v2, input_iterator_tag()).N == &header);
    CHECK(Find(Iterator(header.Next), Iterator(&header), v3, input_iterator_tag()).N == &a);
    CHECK(Find(Iterator(&header), Iterator(&header), v3, input_iterator_tag()).N == &header);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("006145e0_TEST FAIL\n");
        return 1;
    }
    std::printf("006145e0_TEST PASS\n");
    return 0;
}
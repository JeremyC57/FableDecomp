// 005f82bc behaviour test for std::pair<CUnprovenKey_005f82bc, CIVCountedPointer<CUnprovenPointee_005f82bc> >::pair
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
struct CUnprovenKey_005f82bc { long Word; };
class CIVCountedPointeeBase {
public:
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
};
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(CIVCountedPointeeBase* o) : Object(o) { }
    CIVCountedPointer(const CIVCountedPointer<T>& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};
class CUnprovenPointee_005f82bc : public CIVCountedPointeeBase { };
template <class T1, class T2> struct Pair {
    T1 first;
    T2 second;
    Pair(const T1& a, const T2& b) : first(a), second(b) { }
};
static void run()
{
    CUnprovenPointee_005f82bc pointee;
    CUnprovenKey_005f82bc key = { 42 };
    CIVCountedPointer<CUnprovenPointee_005f82bc> ptr(&pointee);
    Pair<CUnprovenKey_005f82bc, CIVCountedPointer<CUnprovenPointee_005f82bc> > p(key, ptr);
    CHECK(p.first.Word == 42);
    CHECK(p.second.Object == &pointee);
    CHECK(pointee.IVRefCount == 2);
    CIVCountedPointer<CUnprovenPointee_005f82bc> none(0);
    Pair<CUnprovenKey_005f82bc, CIVCountedPointer<CUnprovenPointee_005f82bc> > q(key, none);
    CHECK(q.second.Object == 0);
    CHECK(pointee.IVRefCount == 2);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("005f82bc_TEST FAIL\n");
        return 1;
    }
    std::printf("005f82bc_TEST PASS\n");
    return 0;
}
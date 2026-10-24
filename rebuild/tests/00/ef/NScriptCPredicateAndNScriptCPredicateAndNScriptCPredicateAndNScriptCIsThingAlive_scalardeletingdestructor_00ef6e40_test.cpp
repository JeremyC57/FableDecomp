#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "00ef6e40"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

static int g_leafDtors = 0, g_andDtors = 0, g_baseDtors = 0;
struct CScriptThing { void* m_vtable; unsigned char m_data[8]; };
template <class T> struct CPredicate {
    virtual ~CPredicate() { ++g_baseDtors; }
    virtual bool operator()(const T& thing) const = 0;
};
struct CIsThingAlive : CPredicate<CScriptThing> {
    CScriptThing m_thing;
    ~CIsThingAlive() { ++g_leafDtors; }
    bool operator()(const CScriptThing&) const { return true; }
};
template <class A, class B> struct CPredicate_And : CPredicate<CScriptThing> {
    A m_left;
    B m_right;
    ~CPredicate_And() { ++g_andDtors; }
    bool operator()(const CScriptThing& t) const { return m_left(t) && m_right(t); }
};
typedef CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive> TOuter;
int main()
{
    CPredicate<CScriptThing>* p = new TOuter;
    delete p;   // the scalar deleting destructor through the vtable
    CHECK(g_leafDtors == 4 && g_andDtors == 3 && g_baseDtors == 7);
    {
        TOuter local;   // no delete: destructor chain only
    }
    CHECK(g_leafDtors == 8 && g_andDtors == 6 && g_baseDtors == 14);
    std::printf("00ef6e40_TEST PASS\n");
    return 0;
}
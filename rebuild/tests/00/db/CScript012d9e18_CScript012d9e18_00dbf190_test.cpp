#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <string.h>
#include <new>
class CBaseClass { public: virtual ~CBaseClass() { } CBaseClass() { } };
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; CCountedPointer() : Data(0), Info(0) { } };
class CScriptThing : public CBaseClass { public: CCountedPointer<CScriptThing> PImp; CScriptThing() { } };
static int g_scriptBases = 0;
class CScriptBase { public: virtual ~CScriptBase() { } CScriptBase() { ++g_scriptBases; } };
template <class T> struct CAllocator { CAllocator() { } };
template <class T> struct CAllocProxy : CAllocator<T> {
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {
    T* m_pStart; T* m_pFinish; CAllocProxy<T> m_endOfStorage;
    CVectorBase(const CAllocator<T>& alloc) : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0) { }
};
template <class T> struct CSTLportVector : CVectorBase<T> {
    explicit CSTLportVector(const CAllocator<T>& alloc = CAllocator<T>()) : CVectorBase<T>(alloc) { }
};
template <class T> class CArray : public CSTLportVector<T> { public: };
struct CScriptPairBase : CScriptBase {
    long  _pad[15];
    void* m_second;
    void* m_first;
    CScriptPairBase(void* first, void* second) : m_second(second), m_first(first) { }
};
struct CScript_012d9e18 : CScriptPairBase {
    long          _pad48;
    CArray<void*> m_list;
    long          _pad58[3];
    CScriptThing  m_thing;
    CScript_012d9e18(void* first, void* second) : CScriptPairBase(first, second) { }
};
int main()
{
    int a = 0, b = 0;
    unsigned char raw[sizeof(CScript_012d9e18)]; memset(raw, 0xcd, sizeof(raw));
    CScript_012d9e18* s = new (raw) CScript_012d9e18(&a, &b);
    if (g_scriptBases != 1 || s->m_first != &a || s->m_second != &b || s->m_list.m_pStart || s->m_list.m_pFinish || s->m_list.m_endOfStorage.m_data
        || s->m_thing.PImp.Data || s->m_thing.PImp.Info) { std::printf("00dbf190_TEST FAIL\n"); return 1; }
    std::printf("00dbf190_TEST PASS\n");
    return 0;
}
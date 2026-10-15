#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <string.h>
#include <new>
static int g_strings = 0;
struct CCharString { const char* m_pData; CCharString() : m_pData("") { ++g_strings; } };
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
struct SNamedList { CCharString m_name; CSTLportVector<void*> m_items; };
struct SNamedListPair { SNamedList m_first; SNamedList m_second; SNamedListPair() { } };
int main()
{
    unsigned char raw[sizeof(SNamedListPair)]; memset(raw, 0xcd, sizeof(raw));
    SNamedListPair* p = new (raw) SNamedListPair;
    if (g_strings != 2 || p->m_first.m_items.m_pStart || p->m_first.m_items.m_pFinish || p->m_first.m_items.m_endOfStorage.m_data
        || p->m_second.m_items.m_pStart || p->m_second.m_items.m_endOfStorage.m_data || p->m_second.m_name.m_pData[0] != 0) { std::printf("00a46f30_TEST FAIL\n"); return 1; }
    std::printf("00a46f30_TEST PASS\n");
    return 0;
}
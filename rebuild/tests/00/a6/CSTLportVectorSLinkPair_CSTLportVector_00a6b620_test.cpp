#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <new>
#include <stdlib.h>
template <class T> struct CAllocator {
    CAllocator() { }
    T* allocate(unsigned long count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};
template <class T> struct CAllocProxy : CAllocator<T> {
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {
    T* m_pStart; T* m_pFinish; CAllocProxy<T> m_endOfStorage;
    CVectorBase(unsigned long count, const CAllocator<T>& alloc) : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0)
    {
        m_pStart = m_endOfStorage.allocate(count);
        m_pFinish = m_pStart;
        m_endOfStorage.m_data = m_pStart + count;
    }
};
template <class T> T* UninitializedFillN(T* first, unsigned long count, const T& value)
{
    T* cursor = first;
    for (; count > 0; --count, ++cursor) new (cursor) T(value);
    return cursor;
}
template <class T> T* UninitializedConstructN(T* first, unsigned long count)
{
    T* cursor = first;
    for (; count > 0; --count, ++cursor) new (cursor) T;
    return cursor;
}
struct SLinkPair { long m_first; long m_second; SLinkPair() : m_first(0), m_second(0) { } };
template <class T> struct CSTLportVector : CVectorBase<T> {
    explicit CSTLportVector(unsigned long count) : CVectorBase<T>(count, CAllocator<T>())
    { this->m_pFinish = UninitializedConstructN(this->m_pStart, count); }
};
int main()
{
    CSTLportVector<SLinkPair> v(3);
    if (v.m_pFinish != v.m_pStart + 3 || v.m_endOfStorage.m_data != v.m_pStart + 3) { std::printf("00a6b620_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (v.m_pStart[i].m_first != 0 || v.m_pStart[i].m_second != 0) { std::printf("00a6b620_TEST FAIL\n"); return 1; }
    free(v.m_pStart);
    CSTLportVector<SLinkPair> e(0);
    if (e.m_pStart != 0 || e.m_pFinish != 0 || e.m_endOfStorage.m_data != 0) { std::printf("00a6b620_TEST FAIL\n"); return 1; }
    std::printf("00a6b620_TEST PASS\n");
    return 0;
}
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
struct SHandle { long m_value; SHandle() : m_value(0) { } };
template <class T> struct CSTLportVector : CVectorBase<T> {
    explicit CSTLportVector(unsigned long count) : CVectorBase<T>(count, CAllocator<T>())
    { this->m_pFinish = UninitializedFillN(this->m_pStart, count, T()); }
};
int main()
{
    CSTLportVector<SHandle> v(7);
    if (v.m_pFinish != v.m_pStart + 7 || v.m_endOfStorage.m_data != v.m_pStart + 7) { std::printf("00b2ec80_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (v.m_pStart[i].m_value != 0) { std::printf("00b2ec80_TEST FAIL\n"); return 1; }
    free(v.m_pStart);
    CSTLportVector<SHandle> e(0);
    if (e.m_pStart != 0 || e.m_pFinish != 0 || e.m_endOfStorage.m_data != 0) { std::printf("00b2ec80_TEST FAIL\n"); return 1; }
    std::printf("00b2ec80_TEST PASS\n");
    return 0;
}
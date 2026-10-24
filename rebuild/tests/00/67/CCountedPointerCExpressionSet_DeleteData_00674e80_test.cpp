// Behaviour test for 0x00674E80 CCountedPointer<CExpressionSet>::DeleteData: a null object is ignored; otherwise the
// entry vector's block is freed (only when there is one) and the set itself goes to operator delete.
#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

static int g_freed = 0, g_deleted = 0;
static void* g_lastFreed = 0;
static void CountingFree(void* p) { ++g_freed; g_lastFreed = p; std::free(p); }   // models free 0x00BFEA14

enum EExpressionType { EXPRESSION_TYPE_NONE = -1, EXPRESSION_TYPE_0 = 0 };

namespace nav {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) CountingFree(__p); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    struct : _Alloc { _Tp* _M_data; } _M_end_of_storage;
    ~vector()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
}

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    nav::vector<nav::pair<K, V> > Entries;
    C    Compare;
    bool Dirty;
};

class CExpressionSet {
public:
    class CEntry { public: EExpressionType Type; int AnimIndex; };
    CVectorMap<EExpressionType, CEntry, CKeyPairCompareLess<EExpressionType, CEntry> > Expressions;
    static void* operator new(std::size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++g_deleted; std::free(p); }       // models operator delete 0x00BFE9BC
};

template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data) { delete static_cast<T*>(data); }
};

int main()
{
    CCountedPointer<CExpressionSet>::DeleteData(0);
    if (g_freed != 0 || g_deleted != 0) { std::printf("00674e80_TEST FAIL\n"); return 1; }

    CExpressionSet* set = new CExpressionSet;
    typedef nav::pair<EExpressionType, CExpressionSet::CEntry> Entry;
    Entry* block = (Entry*)std::malloc(4 * sizeof(Entry));
    set->Expressions.Entries._M_start = block;
    set->Expressions.Entries._M_finish = block + 2;
    set->Expressions.Entries._M_end_of_storage._M_data = block + 4;
    CCountedPointer<CExpressionSet>::DeleteData(set);
    if (g_freed != 1 || g_lastFreed != block || g_deleted != 1) { std::printf("00674e80_TEST FAIL\n"); return 1; }

    CExpressionSet* empty = new CExpressionSet;
    empty->Expressions.Entries._M_start = empty->Expressions.Entries._M_finish = 0;
    empty->Expressions.Entries._M_end_of_storage._M_data = 0;
    CCountedPointer<CExpressionSet>::DeleteData(empty);
    if (g_freed != 1 || g_deleted != 2) { std::printf("00674e80_TEST FAIL\n"); return 1; }
    std::printf("00674e80_TEST PASS\n");
    return 0;
}
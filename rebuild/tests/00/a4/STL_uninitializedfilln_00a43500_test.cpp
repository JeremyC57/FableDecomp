// Behaviour test for 0x00A43500: std::__uninitialized_fill_n<NGameText::CDataBankTextEntry*> (placement copy n entries, add-ref each)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
}

namespace NGameText {
struct CTextEntry {
    void* __vftable;
    long  RefCount;
};
}
template <class T>
struct CIVCountedPointer {
    T* P;
    CIVCountedPointer(T* p) : P(p) { if (P) ++P->RefCount; }
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
    ~CIVCountedPointer() { if (P) --P->RefCount; }
    CIVCountedPointer& operator=(const CIVCountedPointer& x)
    {
        if (x.P) ++x.P->RefCount;
        if (P) --P->RefCount;
        P = x.P;
        return *this;
    }
};
namespace NGameText {
struct CDataBankTextEntry {
    CIVCountedPointer<CTextEntry> PText;
    long                          NarratorIndex;
    CDataBankTextEntry(CTextEntry* t, long n) : PText(t), NarratorIndex(n) { }
};
}
using NGameText::CTextEntry;
using NGameText::CDataBankTextEntry;

namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
}

int main()
{
    CTextEntry text;
    text.__vftable = 0;
    text.RefCount = 0;
    {
        CDataBankTextEntry x(&text, 42);
        Check(text.RefCount == 1);
        CDataBankTextEntry* raw = (CDataBankTextEntry*)malloc(4 * sizeof(CDataBankTextEntry));
        CDataBankTextEntry* end = std::__uninitialized_fill_n(raw, 4u, x, std::__false_type());
        Check(end == raw + 4 && text.RefCount == 5);
        for (int i = 0; i < 4; ++i)
            Check(raw[i].PText.P == &text && raw[i].NarratorIndex == 42);
        Check(std::__uninitialized_fill_n(raw, 0u, x, std::__false_type()) == raw && text.RefCount == 5);
        for (int j = 0; j < 4; ++j)
            raw[j].~CDataBankTextEntry();
        free(raw);
        Check(text.RefCount == 1);
    }
    Check(text.RefCount == 0);
    if (g_fail) {
        printf("00a43500_TEST FAIL\n");
        return 1;
    }
    printf("00a43500_TEST PASS\n");
    return 0;
}
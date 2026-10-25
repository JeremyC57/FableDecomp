// Behaviour test for 0x00A43B20: std::vector<NGameText::CDataBankTextEntry>::_M_insert_overflow (grow: copy the head, fill, copy the tail, release the old block)
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
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    ~vector() { _M_clear(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start));
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}

int main()
{
    CTextEntry t1, t2, t3;
    t1.__vftable = t2.__vftable = t3.__vftable = 0;
    t1.RefCount = t2.RefCount = t3.RefCount = 0;
    {
        std::vector<CDataBankTextEntry> v;
        CDataBankTextEntry e1(&t1, 1), e2(&t2, 2), e3(&t3, 3);
        v._M_insert_overflow(v._M_finish, e1, std::__false_type(), 1u, true);
        Check(v.size() == 1 && v._M_end_of_storage._M_data == v._M_start + 1 && t1.RefCount == 2);
        v._M_insert_overflow(v._M_finish, e2, std::__false_type(), 1u, true);
        Check(v.size() == 2 && v._M_end_of_storage._M_data == v._M_start + 2 && t1.RefCount == 2 && t2.RefCount == 2);
        // fill three copies in front of the second entry
        v._M_insert_overflow(v._M_start + 1, e3, std::__false_type(), 3u, false);
        Check(v.size() == 5 && v._M_end_of_storage._M_data == v._M_start + 5);
        Check(v._M_start[0].PText.P == &t1 && v._M_start[0].NarratorIndex == 1);
        for (int i = 1; i < 4; ++i)
            Check(v._M_start[i].PText.P == &t3 && v._M_start[i].NarratorIndex == 3);
        Check(v._M_start[4].PText.P == &t2 && v._M_start[4].NarratorIndex == 2);
        Check(t1.RefCount == 2 && t2.RefCount == 2 && t3.RefCount == 4);
    }
    Check(t1.RefCount == 0 && t2.RefCount == 0 && t3.RefCount == 0);
    if (g_fail) {
        printf("00a43b20_TEST FAIL\n");
        return 1;
    }
    printf("00a43b20_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00617FD0: std::vector::_M_fill_insert(unsigned __int64* position, unsigned int n, const unsigned __int64& x) -- STLport insert of n copies of a 64-bit id: memmove the tail inside the spare capacity, else grow through the __true_type _M_insert_overflow
// A copy of the source model and logic (compiled separately from src/00617fd0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

namespace std {
struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __true_type&)
{
    return (_ForwardIter)__copy_trivial(__first, __last, __result);
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

#pragma auto_inline(off)
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x006149B0
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x00613260
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __true_type _TrivialAss;
    typedef __true_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00617370
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<unsigned __int64>::_M_fill_insert(unsigned __int64*, unsigned int, const unsigned __int64&);
}

// ---- test stand-ins ----
static int g_overflowCalls = 0;
static unsigned int g_overflowN = 0;
static void* g_overflowPos = 0;
namespace std {
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp&, const __true_type&, size_type __fill_len, bool)
{
    ++g_overflowCalls; g_overflowPos = (void*)__position; g_overflowN = __fill_len;
}
}

typedef unsigned __int64 u64;
struct TestVector : public std::vector<u64> {
    void Init(u64* storage, unsigned int count, unsigned int capacity)
    {
        this->_M_start = storage; this->_M_finish = storage + count; this->_M_end_of_storage._M_data = storage + capacity;
    }
    u64* Begin() { return this->_M_start; }
    unsigned int Size() { return (unsigned int)(this->_M_finish - this->_M_start); }
};

static int Fail() { std::printf("00617fd0_TEST FAIL\n"); return 1; }
static u64 Id(unsigned int k) { return ((u64)k << 40) | (u64)(k * 7 + 1); }
static bool Same(TestVector& v, const unsigned int* want, unsigned int n)
{
    if (v.Size() != n) return false;
    for (unsigned int i = 0; i < n; ++i)
        if (v.Begin()[i] != Id(want[i])) return false;
    return true;
}

int main()
{
    u64 storage[16];
    for (unsigned int i = 0; i < 16; ++i) storage[i] = (i < 6) ? Id(i) : 0;
    const u64 x = Id(99);
    TestVector v; v.Init(storage, 6, 14);
    v._M_fill_insert(v.Begin() + 2, 3, x);                  // elems_after 4 > 3: memmove tail, fill the gap
    const unsigned int k1[] = { 0, 1, 99, 99, 99, 2, 3, 4, 5 };
    if (!Same(v, k1, 9)) return Fail();
    v._M_fill_insert(v.Begin() + 7, 4, Id(50));             // elems_after 2 <= 4: fill_n the excess, copy, fill
    const unsigned int k2[] = { 0, 1, 99, 99, 99, 2, 3, 50, 50, 50, 50, 4, 5 };
    if (!Same(v, k2, 13)) return Fail();
    v._M_fill_insert(v.Begin(), 0, x);                      // n == 0
    if (!Same(v, k2, 13) || g_overflowCalls != 0) return Fail();
    v._M_fill_insert(v.Begin() + 13, 1, x);                 // exactly fills the capacity, at the end
    const unsigned int k3[] = { 0, 1, 99, 99, 99, 2, 3, 50, 50, 50, 50, 4, 5, 99 };
    if (!Same(v, k3, 14) || g_overflowCalls != 0) return Fail();
    v._M_fill_insert(v.Begin() + 1, 2, x);                  // no spare room: _M_insert_overflow
    if (g_overflowCalls != 1 || g_overflowN != 2 || g_overflowPos != (void*)(v.Begin() + 1) || !Same(v, k3, 14)) return Fail();
    if (storage[14] != 0 || storage[15] != 0) return Fail();
    std::printf("00617fd0_TEST PASS\n");
    return 0;
}
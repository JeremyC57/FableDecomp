// Behaviour test for 0x00618990: std::vector::_M_fill_insert(CTCInventoryMap::CUIRegionInfo* position, unsigned int n, const CUIRegionInfo& x) -- STLport insert of n copies of a 40-byte region info record: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// A copy of the source model and logic (compiled separately from src/00618990.cpp) with test stand-ins for the callees.
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

class CTCInventoryMap {
public:
    class CUIRegionInfo {
    public:
        long Region;                                                             // +0x00
        long Shops;                                                              // +0x04
        long BarberShops;                                                        // +0x08
        long TitleShops;                                                         // +0x0C
        long TatooShops;                                                         // +0x10
        long ShopsForSale;                                                       // +0x14
        long HousesForSale;                                                      // +0x18
        long ShopsOwned;                                                         // +0x1C
        long HousesOwned;                                                        // +0x20
        long Taverns;                                                            // +0x24
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                 // 0x006127D0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp>
__declspec(noinline) _Tp* __fastcall __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{                                                                                 // 0x00614970
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x00613210
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x006131E0
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

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
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00618420
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

template void vector<CTCInventoryMap::CUIRegionInfo>::_M_fill_insert(CTCInventoryMap::CUIRegionInfo*, unsigned int, const CTCInventoryMap::CUIRegionInfo&);
}

// ---- test stand-ins ----
typedef CTCInventoryMap::CUIRegionInfo Elem;
static void Set(Elem& r, long k)
{
    r.Region = k; r.Shops = k + 1; r.BarberShops = k + 2; r.TitleShops = k + 3; r.TatooShops = k + 4;
    r.ShopsForSale = k + 5; r.HousesForSale = k + 6; r.ShopsOwned = k + 7; r.HousesOwned = k + 8; r.Taverns = k * 3;
}
static bool Is(const Elem& r, long k)
{
    return r.Region == k && r.Shops == k + 1 && r.BarberShops == k + 2 && r.TitleShops == k + 3 && r.TatooShops == k + 4 &&
           r.ShopsForSale == k + 5 && r.HousesForSale == k + 6 && r.ShopsOwned == k + 7 && r.HousesOwned == k + 8 && r.Taverns == k * 3;
}

static int g_overflowCalls = 0;
static unsigned int g_overflowN = 0;
static void* g_overflowPos = 0;
namespace std {
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp&, const __false_type&, size_type __fill_len, bool)
{
    ++g_overflowCalls; g_overflowPos = (void*)__position; g_overflowN = __fill_len;
}
}

struct TestVector : public std::vector<Elem> {
    void Init(Elem* storage, unsigned int count, unsigned int capacity)
    {
        this->_M_start = storage; this->_M_finish = storage + count; this->_M_end_of_storage._M_data = storage + capacity;
    }
    Elem* Begin() { return this->_M_start; }
    unsigned int Size() { return (unsigned int)(this->_M_finish - this->_M_start); }
};

static int Fail() { std::printf("00618990_TEST FAIL\n"); return 1; }
static bool Same(TestVector& v, const long* want, unsigned int n)
{
    if (v.Size() != n) return false;
    for (unsigned int i = 0; i < n; ++i)
        if (!Is(v.Begin()[i], want[i])) return false;
    return true;
}

int main()
{
    Elem* storage = (Elem*)std::malloc(16 * sizeof(Elem));
    for (long k = 0; k < 6; ++k) Set(storage[k], k);
    Elem* x = (Elem*)std::malloc(sizeof(Elem));
    Set(*x, 99);
    TestVector v; v.Init(storage, 6, 14);
    
    v._M_fill_insert(v.Begin() + 2, 3, *x);                 // elems_after 4 > 3: copy the tail up, copy_backward, fill
    const long k1[] = { 0, 1, 99, 99, 99, 2, 3, 4, 5 };
    if (!Same(v, k1, 9)) return Fail();
    Set(*x, 50);
    v._M_fill_insert(v.Begin() + 7, 4, *x);                 // elems_after 2 <= 4: fill_n the excess, copy, fill
    const long k2[] = { 0, 1, 99, 99, 99, 2, 3, 50, 50, 50, 50, 4, 5 };
    if (!Same(v, k2, 13)) return Fail();
    v._M_fill_insert(v.Begin(), 0, *x);                     // n == 0
    if (!Same(v, k2, 13) || g_overflowCalls != 0) return Fail();
    v._M_fill_insert(v.Begin() + 13, 1, *x);                // exactly fills the capacity
    const long k3[] = { 0, 1, 99, 99, 99, 2, 3, 50, 50, 50, 50, 4, 5, 50 };
    if (!Same(v, k3, 14) || g_overflowCalls != 0) return Fail();
    v._M_fill_insert(v.Begin() + 1, 2, *x);                 // no spare room: _M_insert_overflow(pos, x, tag, n)
    if (g_overflowCalls != 1 || g_overflowN != 2 || g_overflowPos != (void*)(v.Begin() + 1) || !Same(v, k3, 14)) return Fail();
    
    std::free(x); std::free(storage);
    std::printf("00618990_TEST PASS\n");
    return 0;
}

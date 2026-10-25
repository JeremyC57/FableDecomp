// Behaviour test for 0x005E32F0: std::vector::_M_fill_insert(pair<unsigned long, CAnimationEntry>* position, unsigned int n, const pair& x) -- STLport insert of n copies of a CRC-keyed animation entry: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// A copy of the source model and logic (compiled separately from src/005e32f0.cpp) with test stand-ins for the callees.
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

struct CAnimComponentMap {                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> struct CIVCountedPointer {              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct CDefString {                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

struct CAnimationEntry {                                   // Ego_r.pdb, 0x10 bytes
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
};

namespace std {
template <class _T1, class _T2> struct pair {              // STLport 4.6 pair: explicit memberwise copy constructor
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                 // 0x005E16F0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                 // 0x005E1740
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x005E0E70
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x005E1820
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

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
                            bool __atend = false);                               // 0x005E31F0
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

template void vector<pair<unsigned long, CAnimationEntry> >::_M_fill_insert(pair<unsigned long, CAnimationEntry>*, unsigned int, const pair<unsigned long, CAnimationEntry>&);
}

// ---- test stand-ins ----
static int g_deletes = 0, g_overflowCalls = 0;
static unsigned int g_overflowN = 0;
static void* g_overflowPos = 0;
void CAnimComponentMap::v00() { }
void CAnimComponentMap::Delete() { ++g_deletes; }

namespace std {
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp&, const __false_type&, size_type __fill_len, bool)
{
    ++g_overflowCalls; g_overflowPos = (void*)__position; g_overflowN = __fill_len;
}
}

typedef std::pair<unsigned long, CAnimationEntry> CAnimPair;
struct TestVector : public std::vector<CAnimPair> {
    void Init(CAnimPair* storage, unsigned int count, unsigned int capacity)
    {
        this->_M_start = storage; this->_M_finish = storage + count; this->_M_end_of_storage._M_data = storage + capacity;
    }
    CAnimPair* Begin() { return this->_M_start; }
    unsigned int Size() { return (unsigned int)(this->_M_finish - this->_M_start); }
};

static int Fail() { std::printf("005e32f0_TEST FAIL\n"); return 1; }
static bool Keys(TestVector& v, const unsigned long* want, unsigned int n)
{
    if (v.Size() != n) return false;
    for (unsigned int i = 0; i < n; ++i)
        if (v.Begin()[i].first != want[i] || v.Begin()[i].second.BankIndex != (long)want[i] * 10) return false;
    return true;
}

int main()
{
    CAnimComponentMap comp; comp.IVRefCount = 1;
    CAnimPair* storage = (CAnimPair*)std::malloc(12 * sizeof(CAnimPair));
    for (unsigned int i = 0; i < 5; ++i) {
        storage[i].first = i; storage[i].second.BankIndex = (long)i * 10;
        storage[i].second.AnimName.TablePos = 1; storage[i].second.GroupName.TablePos = 2;
        storage[i].second.Components.Object = 0;
    }
    CAnimPair* x = (CAnimPair*)std::malloc(sizeof(CAnimPair));
    x->first = 99; x->second.BankIndex = 990; x->second.AnimName.TablePos = 7; x->second.GroupName.TablePos = 8;
    x->second.Components.Object = &comp; ++comp.IVRefCount;

    TestVector v; v.Init(storage, 5, 12);
    v._M_fill_insert(v.Begin() + 1, 2, *x);                 // elems_after 4 > n 2: copy_backward + fill
    const unsigned long k1[] = { 0, 99, 99, 1, 2, 3, 4 };
    if (!Keys(v, k1, 7) || comp.IVRefCount != 4 || v.Begin()[1].second.Components.Object != &comp) return Fail();
    v._M_fill_insert(v.Begin() + 5, 3, *x);                 // elems_after 2 <= n 3: fill_n + copy + fill
    const unsigned long k2[] = { 0, 99, 99, 1, 2, 99, 99, 99, 3, 4 };
    if (!Keys(v, k2, 10) || comp.IVRefCount != 7 || v.Begin()[9].second.Components.Object != 0) return Fail();
    if (v.Begin()[7].second.AnimName.TablePos != 7 || v.Begin()[8].second.GroupName.TablePos != 2) return Fail();
    v._M_fill_insert(v.Begin() + 3, 0, *x);                 // n == 0: nothing
    if (!Keys(v, k2, 10) || g_overflowCalls != 0) return Fail();
    v._M_fill_insert(v.Begin() + 2, 3, *x);                 // 2 spare < 3: grow through _M_insert_overflow
    if (g_overflowCalls != 1 || g_overflowN != 3 || g_overflowPos != (void*)(v.Begin() + 2) || !Keys(v, k2, 10)) return Fail();
    if (g_deletes != 0 || comp.IVRefCount != 7) return Fail();
    std::free(x); std::free(storage);
    std::printf("005e32f0_TEST PASS\n");
    return 0;
}
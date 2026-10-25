// Behaviour test for 0x00674520: std::vector::operator=(const vector<pair<EBodyLocation, CWoundMorphs::CEntry> >& x) -- STLport copy assignment of a wound-morph map's entries: reallocate and copy when x outgrows the capacity, else assign over the live prefix and copy-construct the rest
// A copy of the source model and logic (compiled separately from src/00674520.cpp) with test stand-ins for the callees.
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
template <class _T1, class _T2> struct pair {                                       // STLport 4.6 pair: memberwise copy ctor
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

enum EBodyLocation { EBodyLocation_Force32 = 0x7fffffff };

class CWoundMorphs {
public:
    class CEntry {                                                                   // Ego_r.pdb, 40 bytes
    public:
        EBodyLocation Location;                                                      // +0x00
        long          ReplacingTextureIndex;                                         // +0x04
        long          ReplacingBumpMapIndex;                                         // +0x08
        long          WoundTextureIndex;                                             // +0x0C
        long          WoundBumpMapIndex;                                             // +0x10
        long          ScarTextureIndex;                                              // +0x14
        long          ScarBumpMapIndex;                                              // +0x18
        long          FadeToScarDuration;                                            // +0x1C
        long          TotalDuration;                                                 // +0x20
        float         PermanentScarStrength;                                         // +0x24
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                     // 0x00673D30
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{                                                                                     // 0x00673760
    for (int __n = int(__last - __first); __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
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
    typedef const _Tp* const_pointer;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    vector& operator=(const vector& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<pair<EBodyLocation, CWoundMorphs::CEntry> >& vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::operator=(const vector<pair<EBodyLocation, CWoundMorphs::CEntry> >&);
}

// ---- test stand-ins ----
typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> Elem;
static void Make(Elem& e, long k)
{
    e.first = (EBodyLocation)k; CWoundMorphs::CEntry& w = e.second;
    w.Location = (EBodyLocation)(k + 1); w.ReplacingTextureIndex = k + 2; w.ReplacingBumpMapIndex = k + 3; w.WoundTextureIndex = k + 4;
    w.WoundBumpMapIndex = k + 5; w.ScarTextureIndex = k + 6; w.ScarBumpMapIndex = k + 7; w.FadeToScarDuration = k + 8;
    w.TotalDuration = k + 9; w.PermanentScarStrength = 0.25f * (float)k;
}
static bool Is(const Elem& e, long k)
{
    const CWoundMorphs::CEntry& w = e.second;
    return e.first == (EBodyLocation)k && w.Location == (EBodyLocation)(k + 1) && w.ReplacingTextureIndex == k + 2 &&
           w.ReplacingBumpMapIndex == k + 3 && w.WoundTextureIndex == k + 4 && w.WoundBumpMapIndex == k + 5 &&
           w.ScarTextureIndex == k + 6 && w.ScarBumpMapIndex == k + 7 && w.FadeToScarDuration == k + 8 &&
           w.TotalDuration == k + 9 && w.PermanentScarStrength == 0.25f * (float)k;
}

struct TestVector : public std::vector<Elem> {
    void Init(unsigned int count, unsigned int capacity, long base)
    {
        Elem* s = (Elem*)std::malloc(capacity * sizeof(Elem));
        for (unsigned int i = 0; i < count; ++i) Make(s[i], base + (long)i);
        this->_M_start = s; this->_M_finish = s + count; this->_M_end_of_storage._M_data = s + capacity;
    }
    Elem* Begin() { return this->_M_start; }
    unsigned int Size() { return this->size(); }
    unsigned int Cap() { return this->capacity(); }
    void Release() { this->_M_clear(); }
    bool Holds(unsigned int n, long base)
    {
        if (Size() != n) return false;
        for (unsigned int i = 0; i < n; ++i) if (!Is(Begin()[i], base + (long)i)) return false;
        return true;
    }
};

static int Fail() { std::printf("00674520_TEST FAIL\n"); return 1; }

int main()
{
    TestVector a, b, c, d, e;
    a.Init(2, 2, 100); b.Init(5, 5, 200); c.Init(3, 3, 300); d.Init(4, 4, 400); e.Init(0, 0, 0);
    a = b;                                                   // x outgrows the capacity: allocate, copy, free the old block
    if (!a.Holds(5, 200) || a.Cap() != 5 || a.Begin() == b.Begin() || !b.Holds(5, 200)) return Fail();
    a = c;                                                   // shrink: assign the prefix, destroy the rest, keep capacity
    if (!a.Holds(3, 300) || a.Cap() != 5) return Fail();
    Elem* keep = a.Begin();
    a = d;                                                   // grow inside capacity: assign 3, copy-construct 1
    if (!a.Holds(4, 400) || a.Cap() != 5 || a.Begin() != keep) return Fail();
    a = a;                                                   // self-assignment is a no-op
    if (!a.Holds(4, 400) || a.Begin() != keep) return Fail();
    a = e;                                                   // assign an empty vector
    if (a.Size() != 0 || a.Cap() != 5) return Fail();
    e = c;                                                   // into an empty, unallocated vector
    if (!e.Holds(3, 300) || e.Cap() != 3) return Fail();
    
    a.Release(); b.Release(); c.Release(); d.Release(); e.Release();
    
    std::printf("00674520_TEST PASS\n");
    return 0;
}
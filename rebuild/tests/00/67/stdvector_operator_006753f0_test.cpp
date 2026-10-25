// Behaviour test for 0x006753F0: std::vector::operator=(const vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> >& x) -- STLport copy assignment of the per-body-part texture morph maps: reallocate and copy when x outgrows the capacity, else assign over the live prefix and destroy or copy-construct the rest
// A copy of the source model and logic (compiled separately from src/006753f0.cpp) with test stand-ins for the callees.
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
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

class CRandomAppearanceMorph {
public:
    class CTextureMorph {
    public:
        unsigned long NewTexture;                                                    // +0x00
        unsigned long GroupID;                                                       // +0x04
    };
};

template <class K, class V> class CKeyPairCompareLess { };

template <class T> class CArray {                                                    // retail CArray<T>: an STLport vector
public:
    T* m_first;
    T* m_last;
    T* m_end;
    CArray(const CArray& other);
    CArray& operator=(const CArray& other);
    ~CArray() { if (m_first) free(m_first); }
};

template <class K, class V, class C> class CVectorMap {                              // retail: 16 bytes
public:
    CArray<std::pair<K, V> > Vector;                                                 // +0x00
    C                        Compare;                                                // +0x0C (empty)
    bool                     Sorted;                                                 // +0x0D
};

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                     // 0x00674CE0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                     // 0x00674A40
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{                                                                                     // 0x00675310
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
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
    __declspec(noinline) void _M_clear()                                             // 0x00675340
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

template vector<CTextureMorphMap>& vector<CTextureMorphMap>::operator=(const vector<CTextureMorphMap>&);
}

// ---- test stand-ins ----
static int g_blocks = 0;                                     // live CArray blocks made by the test stand-ins
template <class T> CArray<T>::CArray(const CArray& o)
{
    unsigned int n = (unsigned int)(o.m_last - o.m_first);
    m_first = n ? (T*)std::malloc(n * sizeof(T)) : 0;
    if (n) { std::memcpy(m_first, o.m_first, n * sizeof(T)); ++g_blocks; }
    m_last = m_end = m_first + n;
}
template <class T> CArray<T>& CArray<T>::operator=(const CArray& o)
{
    if (this != &o) {
        if (m_first) { std::free(m_first); --g_blocks; }
        unsigned int n = (unsigned int)(o.m_last - o.m_first);
        m_first = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        if (n) { std::memcpy(m_first, o.m_first, n * sizeof(T)); ++g_blocks; }
        m_last = m_end = m_first + n;
    }
    return *this;
}
typedef CTextureMorphMap Elem;
typedef std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> MorphPair;
static void Make(Elem& e, long k)
{
    unsigned int n = (unsigned int)(k % 3) + 1;
    e.Vector.m_first = (MorphPair*)std::malloc(n * sizeof(MorphPair)); ++g_blocks;
    for (unsigned int j = 0; j < n; ++j) {
        e.Vector.m_first[j].first = (unsigned long)(k * 10 + (long)j);
        e.Vector.m_first[j].second.NewTexture = (unsigned long)k; e.Vector.m_first[j].second.GroupID = j;
    }
    e.Vector.m_last = e.Vector.m_end = e.Vector.m_first + n;
    e.Sorted = (k & 1) != 0;
}
static bool Is(const Elem& e, long k)
{
    unsigned int n = (unsigned int)(k % 3) + 1;
    if ((unsigned int)(e.Vector.m_last - e.Vector.m_first) != n || e.Sorted != ((k & 1) != 0)) return false;
    for (unsigned int j = 0; j < n; ++j)
        if (e.Vector.m_first[j].first != (unsigned long)(k * 10 + (long)j) || e.Vector.m_first[j].second.NewTexture != (unsigned long)k ||
            e.Vector.m_first[j].second.GroupID != j) return false;
    return true;
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

static int Fail() { std::printf("006753f0_TEST FAIL\n"); return 1; }

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
    if (g_blocks != 14 + 5 + 1 + 3) return Fail();
    a.Release(); b.Release(); c.Release(); d.Release(); e.Release();
    
    std::printf("006753f0_TEST PASS\n");
    return 0;
}
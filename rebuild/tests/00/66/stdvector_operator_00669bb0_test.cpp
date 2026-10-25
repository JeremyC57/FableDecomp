// Behaviour test for 0x00669BB0: std::vector::operator=(const vector<const CAnimationEntry*>& x) -- STLport copy assignment of an animation-entry pointer vector: reallocate and copy when x outgrows the capacity, else memmove over the live prefix (and the uninitialised tail)
// A copy of the source model and logic (compiled separately from src/00669bb0.cpp) with test stand-ins for the callees.
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

class CAnimationEntry;

namespace std {
struct __true_type { __true_type() { } };

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

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

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
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __true_type());
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __true_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __true_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __true_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<const CAnimationEntry*>& vector<const CAnimationEntry*>::operator=(const vector<const CAnimationEntry*>&);
}

// ---- test stand-ins ----
typedef const CAnimationEntry* Elem;
static char g_entries[1024];
static void Make(Elem& e, long k) { e = (const CAnimationEntry*)&g_entries[k]; }
static bool Is(const Elem& e, long k) { return e == (const CAnimationEntry*)&g_entries[k]; }

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

static int Fail() { std::printf("00669bb0_TEST FAIL\n"); return 1; }

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
    
    std::printf("00669bb0_TEST PASS\n");
    return 0;
}
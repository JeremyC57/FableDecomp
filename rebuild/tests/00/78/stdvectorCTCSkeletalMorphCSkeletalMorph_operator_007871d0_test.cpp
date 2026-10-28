// std::vector<CSkeletalMorph>::operator=(const vector& x) -- STLport copy assignment of the skeletal-morph component's Morphs vector (12-byte {ID, Name, Strength} entries): reallocate and copy-construct when x outgrows the capacity, else assign over the live prefix (destroying the surplus) or assign the prefix and copy-construct the tail
// Leaf: ??4?$vector@VCSkeletalMorph@CTCSkeletalMorph@@V?$allocator@VCSkeletalMorph@CTCSkeletalMorph@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007871D0, 307 bytes, thiscall, `ret 4`.
// Identity (sweep2, 2026-10-25): CTCSkeletalMorph vtable 0x0126B6F4 slot 12 (0x00786650 = CTCSkeletalMorph::Copy,
//   Ego_r slot order) sets Changed (+0xC) and tail-jumps here with this+0x10 / other+0x10: Ego_r.pdb
//   CTCSkeletalMorph::Morphs +0x10 is CArray<CTCSkeletalMorph::CSkeletalMorph>. The element helpers copy-construct
//   +4 through the CCharString copy ctor 0x0099EC30 between dwords +0/+8 = Ego_r.pdb CSkeletalMorph {long ID,
//   CCharString Name, float Strength}, 12 bytes. Ego_r.pdb name set:
//   ??4?$vector@VCSkeletalMorph@CTCSkeletalMorph@@V?$allocator@VCSkeletalMorph@CTCSkeletalMorph@@@std@@@std@@QAEAAV01@ABV01@@Z.
//   The landed name CCombatWheel_Assign was propagated; the model was a nested VectorAssignment_007871d0::std over
//   `unsigned[3]` (owner 2026-10-25: top-level std).
// Callees: __uninitialized_copy @ 0x00786E40, __copy @ 0x00786BA0, _Destroy @ 0x00787120, _M_clear @ 0x00787190, malloc @ 0x00BFEA0E.
// Shape: STLport 4.6 vector::operator= over a class element (__type_traits all __false_type): _M_allocate_and_copy
//   inlined (allocate + __uninitialized_copy), __copy_ptrs inlined to its random-access __copy call.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E

class CCharString {
public:
    void* PStringData;                                      // +0x0
    CCharString();
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
};

class CTCSkeletalMorph {
public:
    class CSkeletalMorph {                                  // Ego_r.pdb, 12 bytes
    public:
        long        ID;                                     // +0x0
        CCharString Name;                                   // +0x4
        float       Strength;                               // +0x8
    };
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                  // out of line
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // out of line

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                    // out of line

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
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear();                                                                           // out of line
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CTCSkeletalMorph::CSkeletalMorph>& vector<CTCSkeletalMorph::CSkeletalMorph>::operator=(const vector<CTCSkeletalMorph::CSkeletalMorph>&);

}   // namespace std

// ---- test harness: out-of-line helper bodies + behaviour test ----
#include <stdio.h>
#include <stdlib.h>
#include <new>

CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { }
CCharString::~CCharString() { }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
static int g_tags[16];
static CTCSkeletalMorph::CSkeletalMorph make(int i)
{
    CTCSkeletalMorph::CSkeletalMorph m;
    m.ID = 100 + i; *(void**)&m.Name = &g_tags[i]; m.Strength = 0.25f * i;
    return m;
}
static bool same(const CTCSkeletalMorph::CSkeletalMorph& a, const CTCSkeletalMorph::CSkeletalMorph& b)
{
    return a.ID == b.ID && a.Name.PStringData == b.Name.PStringData && a.Strength == b.Strength;
}

namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
    return __result;
}
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result) new (__result) CTCSkeletalMorph::CSkeletalMorph(*__first);
    return __result;
}
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last) { for ( ; __first != __last; ++__first) __first->~CSkeletalMorph(); }
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    free(this->_M_start);
}
}
struct VecView { CTCSkeletalMorph::CSkeletalMorph* s; CTCSkeletalMorph::CSkeletalMorph* f; CTCSkeletalMorph::CSkeletalMorph* e; };
static VecView& W(std::vector<CTCSkeletalMorph::CSkeletalMorph>& v) { return *(VecView*)&v; }
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { printf("fail line %d\n", line); g_fail = 1; } }
static bool same_range(const CTCSkeletalMorph::CSkeletalMorph* a, const CTCSkeletalMorph::CSkeletalMorph* b, int n) { for (int i = 0; i < n; ++i) if (!same(a[i], b[i])) return false; return true; }
int main()
{
    CTCSkeletalMorph::CSkeletalMorph data[5];
    for (int i = 0; i < 5; ++i) data[i] = make(i);
    std::vector<CTCSkeletalMorph::CSkeletalMorph> a, b;
    W(a).s = W(a).f = W(a).e = 0;
    W(b).s = data; W(b).f = data + 3; W(b).e = data + 5;
    a = b;                                              // grow: allocate + copy-construct
    check(W(a).f - W(a).s == 3 && W(a).e - W(a).s == 3 && same_range(W(a).s, data, 3), __LINE__);
    CTCSkeletalMorph::CSkeletalMorph* storage = W(a).s;
    a = a;                                              // self-assignment: no-op
    check(W(a).s == storage && W(a).f == storage + 3, __LINE__);
    W(b).f = data + 1; a = b;                           // shrink: assign prefix, destroy the rest
    check(W(a).s == storage && W(a).f == storage + 1 && same_range(W(a).s, data, 1), __LINE__);
    data[1] = make(7); data[2] = make(8); W(b).f = data + 3; a = b;   // within capacity: assign + construct tail
    check(W(a).s == storage && W(a).f == storage + 3 && same_range(W(a).s, data, 3), __LINE__);
    W(b).f = data + 5; a = b;                           // reallocate
    check(W(a).f - W(a).s == 5 && W(a).e - W(a).s == 5 && same_range(W(a).s, data, 5), __LINE__);
    W(b).f = data; a = b;
    check(W(a).f == W(a).s, __LINE__);
    if (!g_fail) puts("007871d0_TEST PASS");
    return g_fail;
}

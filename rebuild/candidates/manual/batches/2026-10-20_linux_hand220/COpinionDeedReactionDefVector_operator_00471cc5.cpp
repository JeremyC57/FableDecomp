// COpinionDeedReactionDefVector::operator=(const COpinionDeedReactionDefVector& __x) -- STLport vector<COpinionDeedReactionDef> assignment: reallocate past the capacity, else assign over the live elements (destroying the surplus) or assign and copy-construct the tail; returns *this
// Leaf: ??4COpinionDeedReactionDefVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471CC5, length 303 bytes; size-optimised def TU.
// Pragma: #pragma optimize("s", on) -- ebp frame, `push 50h ; pop edi` constants, `lea [x+x*4] ; shl 4` scaling.
// Callees: _M_allocate_and_copy @ 0x0047204B, _M_clear @ 0x00471DF4, COpinionDeedReactionDef::operator=
//          @ 0x00471E17, COpinionDeedReactionDef copy ctor @ 0x00471F60, __destroy_aux @ 0x00450AEA
//          (__fastcall first/last in ecx/edx + the __false_type tag by reference: an LTCG-converted helper).
// Identity: the manifest label "CActiveFile::AssignOpinionDeeds" is propagated. The only caller is
// COpinionDeedEffectsDef::operator= @ 0x00471A77 (slot 19 of vtable 0x01233E54), which assigns the
// Reaction vector (PDB +0x60); retail COpinionDeedReactionDef is 0x50 bytes (PDB 88, see the landed
// operator= 0x00471E17: 0x3C-byte def base + 12-byte vector + two words).
// Shape: STLport 4.x `vector::operator=` with a non-trivial element: the copy loops call the element
// operator=, the surplus goes through `_Destroy(__i, _M_finish)` -> `__destroy_aux(..., __false_type)`,
// the tail through placement-new copy construction (null test). The tag must be a NAMED local: the
// `__false_type()` temporary is zero-filled (`stosb`), retail passes the uninitialised dead `__x` slot.
// The copy helpers are __forceinline (the size TU would emit them out of line).
#pragma optimize("s", on)
#include <new>

class COpinionDeedReactionDef {                 // retail 0x50 bytes (PDB 88): vptr + def base + attitudes + names
public:
    virtual ~COpinionDeedReactionDef();
    unsigned char _body[0x4c];
    COpinionDeedReactionDef(const COpinionDeedReactionDef& other);              // 0x00471F60
    COpinionDeedReactionDef& operator=(const COpinionDeedReactionDef& other);   // 0x00471E17
};

struct random_access_iterator_tag {};
struct __false_type {};

void __fastcall __destroy_aux(COpinionDeedReactionDef* __first, COpinionDeedReactionDef* __last, const __false_type&);   // 0x00450AEA

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                 _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        new (&*__cur) COpinionDeedReactionDef(*__first);
    return __cur;
}

__forceinline void _Destroy(COpinionDeedReactionDef* __first, COpinionDeedReactionDef* __last)
{
    __false_type __tag;
    __destroy_aux(__first, __last, __tag);
}

struct COpinionDeedReactionDefVector {
    typedef COpinionDeedReactionDef* pointer;
    typedef const COpinionDeedReactionDef* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x0047204B
    void _M_clear();                                                                              // 0x00471DF4
    COpinionDeedReactionDefVector& operator=(const COpinionDeedReactionDefVector& __x);
};

COpinionDeedReactionDefVector& COpinionDeedReactionDefVector::operator=(const COpinionDeedReactionDefVector& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            _M_start = __tmp;
            _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, _M_start, random_access_iterator_tag(), (int*)0);
            _Destroy(__i, _M_finish);
        }
        else {
            __copy((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), _M_start, random_access_iterator_tag(), (int*)0);
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, _M_finish);
        }
        _M_finish = _M_start + __xlen;
    }
    return *this;
}

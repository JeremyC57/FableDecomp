// CEnvironmentThemeDefVector::operator=(const CEnvironmentThemeDefVector& __x) -- STLport vector<CEnvironmentThemeDef> assignment: reallocate past the capacity, else assign over the live elements (destroying the surplus) or assign and copy-construct the tail; returns *this
// Leaf: ??4CEnvironmentThemeDefVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00472EDA, length 327 bytes (`ret 4`); size-optimised def TU (same TU as the landed
//          COpinionDeedReactionDefVector::operator= @ 0x00471CC5, whose model this is).
// Pragma: #pragma optimize("s", on) -- ebp frame, `mov esi,0C4h` element size kept in esi, `imul 0C4h` scaling.
// Callees: _M_allocate_and_copy @ 0x004734B7 (manifest "EnvironmentTheme_AllocCopy"), _M_clear @ 0x00473023 (manifest
//          "CActiveFile::OnReadFinished" is propagated), CEnvironmentThemeDef::operator= @ 0x00473046, the CEnvironmentThemeDef
//          copy ctor @ 0x0047318D, __destroy_aux @ 0x00450B1B (__fastcall first/last in ecx/edx + the __false_type tag by
//          reference; manifest "vector<CIntelligentPointer<CThing const>>::_Destroy" is a byte twin).
// Identity: the manifest label "CActiveFile::AssignEnvironmentThemes" is propagated; the element size 0xc4 and the callees
//          name CEnvironmentThemeDef (its copy ctor is landed as 0x0047318D).
// Shape: STLport 4.x `vector::operator=` with a non-trivial element: the copy loops call the element operator=, the surplus
//          goes through `_Destroy(__i, _M_finish)` -> `__destroy_aux(..., __false_type)` with a NAMED tag local (homed in the
//          dead `__x` slot), the tail through placement-new copy construction (null test); the helpers are __forceinline.
#pragma optimize("s", on)
#include <new>

class CEnvironmentThemeDef {                 // retail 0xc4 bytes (PDB 200): vptr + def base + the theme fields
public:
    virtual ~CEnvironmentThemeDef();
    unsigned char _body[0xc0];
    CEnvironmentThemeDef(const CEnvironmentThemeDef& other);              // 0x0047318D
    CEnvironmentThemeDef& operator=(const CEnvironmentThemeDef& other);   // 0x00473046
};

struct random_access_iterator_tag {};
struct __false_type {};

void __fastcall __destroy_aux(CEnvironmentThemeDef* __first, CEnvironmentThemeDef* __last, const __false_type&);   // 0x00450B1B

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
        new (&*__cur) CEnvironmentThemeDef(*__first);
    return __cur;
}

__forceinline void _Destroy(CEnvironmentThemeDef* __first, CEnvironmentThemeDef* __last)
{
    __false_type __tag;
    __destroy_aux(__first, __last, __tag);
}

struct CEnvironmentThemeDefVector {
    typedef CEnvironmentThemeDef* pointer;
    typedef const CEnvironmentThemeDef* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x004734B7
    void _M_clear();                                                                              // 0x00473023
    CEnvironmentThemeDefVector& operator=(const CEnvironmentThemeDefVector& __x);
};

CEnvironmentThemeDefVector& CEnvironmentThemeDefVector::operator=(const CEnvironmentThemeDefVector& __x)
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
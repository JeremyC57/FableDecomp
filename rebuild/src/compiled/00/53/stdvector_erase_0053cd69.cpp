// std::vector::erase(iterator first, iterator last) -- (vector<std::map<unsigned long, NUISystem::CUIState> >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy the vacated tail through the out-of-line __destroy_aux, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@V?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CD69, 55 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 8`, returns first.
// Identity: std::vector<std::map<unsigned long, NUISystem::CUIState> >::erase(iterator, iterator) -- proven; evidence: its callers are the landed NUISystem::CList::DoRecomputeOffsets 0x0053C332, which erases [begin, end) of the vector at +0x17C, and the clear() 0x0053CEEB; +0x17C is CList::ScrollingChildrenStates (Ego_r.pdb +0x1B4, vector<map<unsigned long, NUISystem::CUIState> >; the landed push_back 0x0053CFC2 and SetDefaultChild 0x005362AD agree, and PositionOffset +0x188 is written just before); the 12-byte element's operator= 0x0042CE7A is the _Rb_tree assignment (self test, clear 0x0042ABE1, node count 0, empty-root header relink) and its destructor 0x0042B755 clears and frees the header node; the PDB name sets hold vector<map<unsigned long, NUISystem::CUIState> >::erase
//   Family G072 (seven byte-identical bodies, one COMDAT per T: retail has no ICF). Catalogue label "CArray_BodyPartMesh::ReplaceRange" is propagated/behavioural.
// Callees: std::__copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x0053CC38 (`__fastcall`: first in
//   ecx, last in edx, result / tag reference / null distance pointer on the stack, `ret 0Ch`; 0xC-byte elements, the element's
//   operator= 0x0042CE7A); std::__destroy_aux<T*>(first, last, const __false_type&) 0x0053CC6A (`__fastcall`, `ret 4`; the map's ~_Rb_tree 0x0042B755 on each element).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the family G067 body with __copy left out of line: both callees are same-TU noinline
//   `__fastcall` bodies that never touch their tag, so VC7.1 homes both tags in the live `first` slot ([ebp+0Bh]) as retail does.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
class C2DVector { public: float X; float Y; };                // 8 bytes
namespace NUISystem {
class CUIState {                                             // 0x24 bytes (Ego_r.pdb)
public:
    unsigned long StateChangeFlag;                           // +0x00
    long          StateChangeType;                           // +0x04 EStateChangeType
    C2DVector     Position;                                  // +0x08
    C2DVector     Zoom;                                      // +0x10
    unsigned long Colour;                                    // +0x18 CRGBColour
    float         UpdateTime;                                // +0x1C
    bool          LinearChange;                              // +0x20
};
}

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {                                             // 12 bytes: header node pointer, node count, comparator
public:
    void*        _M_header;                                  // +0x0
    unsigned int _M_node_count;                              // +0x4
    _Compare     _M_key_compare;                             // +0x8
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x);  // 0x0042CE7A
    ~_Rb_tree();                                             // 0x0042B755 (clear 0x0042ABE1, then the header node is freed)
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};

#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>                   // 0x0053CC38 (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                  const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>                                                        // 0x0053CC6A (same-TU body)
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

typedef map<unsigned long, NUISystem::CUIState> _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);

} // namespace std
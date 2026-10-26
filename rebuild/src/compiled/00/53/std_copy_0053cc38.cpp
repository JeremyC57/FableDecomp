// std::__copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*) -- (std::map<unsigned long, NUISystem::CUIState>*, std::map<unsigned long, NUISystem::CUIState>*, int) STLport random-access copy: n = last - first assignments through the element's out-of-line operator=, return the result's end
// Flags: /Gr
// Leaf: ??$__copy@PAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@PAV12@H@std@@YIPAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@0@PAV10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CC38, 50 bytes (size-optimised TU), __fastcall (/Gr: first ecx, last edx;
//   result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<std::map<unsigned long, NUISystem::CUIState>*, std::map<unsigned long, NUISystem::CUIState>*, int>; evidence: its operator= callee 0x0042CE7A is the _Rb_tree assignment (self test, clear 0x0042ABE1, node count 0, empty-root header relink) of a 12-byte element; its callers are agent951's landed vector<map<unsigned long, NUISystem::CUIState> >::erase 0x0053CD69 (NUISystem::CList::ScrollingChildrenStates +0x17C), the second erase 0x0053CD05 and the __copy_ptrs forwarder 0x0053CC90
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*,CArray<class_CRandomAppearanceMo" is a propagated / behavioural label (family J070: five 50-byte copy loops, each its own instantiation; retail has no identical-code folding).
// Callees: std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::operator= 0x0042CE7A (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy verbatim; the count is the exact pointer difference (`idiv 0xC`, the
//   divisor by push/pop under /Os) and the result pointer is advanced in its own stack slot.
// Element: std::map<unsigned long, NUISystem::CUIState> (STLport, 12 bytes); its implicit operator= is the _Rb_tree assignment, declared as agent951 landed it
#pragma optimize("s", on)
class C2DVector {                                        // Ego_r.pdb 8 bytes
public:
    float X;
    float Y;
};
namespace NUISystem {
class CUIState {                                         // Ego_r.pdb 0x24 bytes
public:
    unsigned long StateChangeFlag;                       // +0x00
    long          StateChangeType;                       // +0x04 EStateChangeType
    C2DVector     Position;                              // +0x08
    C2DVector     Zoom;                                  // +0x10
    unsigned long Colour;                                // +0x18 CRGBColour
    float         UpdateTime;                            // +0x1C
    bool          LinearChange;                          // +0x20
};
}
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {                                         // 12 bytes: header node pointer, node count, comparator
public:
    void*        _M_header;                              // +0x0
    unsigned int _M_node_count;                          // +0x4
    _Compare     _M_key_compare;                         // +0x8
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x);   // 0x0042CE7A
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                              // STLport, 12 bytes; its implicit operator= assigns _M_t
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template std::map<unsigned long, NUISystem::CUIState>* __copy<std::map<unsigned long, NUISystem::CUIState>*, std::map<unsigned long, NUISystem::CUIState>*, int>(std::map<unsigned long, NUISystem::CUIState>*, std::map<unsigned long, NUISystem::CUIState>*, std::map<unsigned long, NUISystem::CUIState>*, const random_access_iterator_tag&, int*);
}
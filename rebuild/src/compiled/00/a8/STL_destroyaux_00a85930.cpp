// _STL::__destroy_aux(CNavigationLayer* first, CNavigationLayer* last, const __false_type&) -- destroy a range of 40-byte navigation layers: per layer free the switchable line-list map, the line lists and the node array
// Leaf: ??$__destroy_aux@PAVCNavigationLayer@CNavQuadTree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85930, 108 bytes, /Gr `__fastcall` (first ecx -> esi, last edx -> edi, the
//   tag reference on the stack), `ret 4`.
// Identity: 12 call sites in the navigation quad-tree TU: the CNavQuadTree destructor 0x00A7A7F0, Initialise 0x00A7A8D0,
//   LoadFromFile 0x00A7AF60 and the vector<CNavQuadTree::CNavigationLayer> erase/assign/insert bodies 0x00A859A0..0x00A85F07
//   (`_Destroy(first, last)` of the 40-byte layer). The catalog's "_Copy_impl<CNavigationPosition*, ...>" is propagated.
// Layout: Ego_r CNavQuadTree::CNavigationLayer with retail STLport containers (see the landed ClearNodes 0x00A792E0):
//   CArray<CANavQuadTreeNode*> Nodes +0x00, vector<list<C2DLineF> > LineLists +0x0C, CVectorMap<unsigned __int64,
//   list<C2DLineF> > SwitchableLineLists +0x18 (vector +0x18, Compare +0x24, Dirty +0x25) = 0x28 bytes.
// Callees: __destroy_aux over the map's (key, list) pairs 0x00A83420 and over the line lists 0x00A827D0 (both
//   `__fastcall`, the same frame-byte tag), free 0x00BFEA14. Retry pass 3: both callees get same-TU noinline stand-in
//   bodies that never read the tag; VC7.1 then packs both tag temporaries into one frame byte ([esp+0Bh]) as retail.
// Shape: STLport `for (; first != last; ++first) _Destroy(&*first);` with the layer's implicit destructor inline: members
//   in reverse order, each ~vector = `_Destroy(start, finish)` (an out-of-line __destroy_aux for the two non-trivial
//   element types, nothing for the node pointers) then `_Vector_base` deallocation (`if (start) free(start)`, start
//   re-read after the call). The two out-of-line instantiations are declared as explicit specialisations.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class C2DLineF;
class CANavQuadTreeNode;

namespace _STL {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }

template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    __destroy_aux(__first, __last, typename __type_traits<_Tp>::has_trivial_destructor());
}

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
};

template <class _Tp> class list {
public:
    void* _M_node;                                      // +0x00 sentinel
    ~list();                                            // 0x005127A0
};

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

#pragma auto_inline(off)
// Same-TU stand-ins for the two out-of-line instances (never read the tag), so VC7.1 packs both tag temporaries
// into one frame byte as retail does.
template <> __declspec(noinline) void __fastcall __destroy_aux<pair<unsigned __int64, list<C2DLineF> >*>(
    pair<unsigned __int64, list<C2DLineF> >* __first, pair<unsigned __int64, list<C2DLineF> >* __last, const __false_type&)   // 0x00A83420
{
    for (; __first != __last; ++__first)
        __first->second.~list();
}
template <> __declspec(noinline) void __fastcall __destroy_aux<list<C2DLineF>*>(
    list<C2DLineF>* __first, list<C2DLineF>* __last, const __false_type&)                                                     // 0x00A827D0
{
    for (; __first != __last; ++__first)
        __first->~list();
}
#pragma auto_inline(on)
}

template <class T> class CArray : public _STL::vector<T> { };   // retail CArray<T>: the 12-byte STLport vector

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C> class CVectorMap : public _STL::vector<_STL::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
};

class CNavQuadTree {
public:
    class CNavigationLayer {                            // 0x28 bytes
    public:
        CArray<CANavQuadTreeNode*>             Nodes;                // +0x00
        _STL::vector<_STL::list<C2DLineF> >    LineLists;            // +0x0c
        CVectorMap<unsigned __int64, _STL::list<C2DLineF>,
                   CKeyPairCompareLess<unsigned __int64, _STL::list<C2DLineF> > > SwitchableLineLists;   // +0x18
    };
};

namespace _STL {
template void __fastcall __destroy_aux<CNavQuadTree::CNavigationLayer*>(
    CNavQuadTree::CNavigationLayer* __first, CNavQuadTree::CNavigationLayer* __last, const __false_type&);
}
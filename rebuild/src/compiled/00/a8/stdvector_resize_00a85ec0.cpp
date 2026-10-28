// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CNavQuadTree::CNavigationLayer >) STLport resize: shrink by erasing [begin() + new_size, end()) (copy down, destroy the stale tail), or grow by inserting new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@VCNavigationLayer@CNavQuadTree@@V?$allocator@VCNavigationLayer@CNavQuadTree@@@std@@@std@@QAEXIABVCNavigationLayer@CNavQuadTree@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85EC0, 105 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<CNavQuadTree::CNavigationLayer >::resize(unsigned int, const CNavQuadTree::CNavigationLayer&); evidence: its only caller is resize(n) 0x00A85F30 (a zeroed 40-byte layer -- the CVectorMap's Dirty byte at +0x25 -- passed as x and destroyed afterwards through the layer destructor 0x00A851E0); the destroy callee 0x00A85930 is the landed __destroy_aux over CNavQuadTree::CNavigationLayer (Nodes +0x00, LineLists +0x0C, SwitchableLineLists +0x18, 0x28 bytes in retail; Ego_r.pdb CNavQuadTree::NavigationLayers), and FableWin.pdb holds ?resize@?$vector@VCNavigationLayer@CNavQuadTree@@...@std@@QAEXI@Z.
// Label: the manifest's "resize" is right in kind but propagated from a byte-identical body elsewhere (family J087: the
//   105-byte resize(n, x) of 20/24/40-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00A85870 (erase's __copy_ptrs,
//   __fastcall, `ret 0Ch`); __destroy_aux<T*>(first, last, const __false_type&) 0x00A85930 (landed, ??$__destroy_aux@PAVCNavigationLayer@CNavQuadTree@@... ; the bindings' ?_Destroy_range@@YIXPAUCNavigationLayer@@... is wrong) (erase's _Destroy, __fastcall,
//   `ret 4`); _M_fill_insert(pos, n, x) 0x00A85CC0 (insert(end(), n, x), thiscall, `ret 0Ch`).
// Shape: STLport 4.6 _vector.h `resize(n, x)` with erase(first, last) inline; the tag temporaries live in the dead new_size slot.
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
protected:
    public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // out of line
    protected:
};
} // namespace std
namespace std {
template <class _Tp> struct _List_node;
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                               // 4 bytes in retail
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // +0x00
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class T> class CArray : public std::vector<T> {  // 12 bytes in retail (16 in Ego_r.pdb)
};
template <class K, class V> class CKeyPairCompareLess {
};
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                         // +0x00
    TCompare Compare;                                      // +0x0C
    bool Dirty;                                            // +0x0D
};
class C2DLineF;
class CANavQuadTreeNode;
class CNavQuadTree {                                       // partial view: only the nested type is used here
public:
    class CNavigationLayer {                               // 40 bytes in retail (52 in Ego_r.pdb)
    public:
        CArray<CANavQuadTreeNode*> Nodes;                  // +0x00
        std::vector<std::list<C2DLineF> > LineLists;       // +0x0C
        CVectorMap<unsigned __int64, std::list<C2DLineF>, CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> > > SwitchableLineLists;   // +0x18
    };
};
typedef CNavQuadTree::CNavigationLayer TElement_00a85ec0;
template void std::vector<TElement_00a85ec0 >::resize(unsigned int, const TElement_00a85ec0&);
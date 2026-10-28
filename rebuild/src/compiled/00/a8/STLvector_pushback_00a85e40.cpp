// std::vector::push_back(const CNavQuadTree::CNavigationLayer& x) -- append a copy of a 40-byte navigation layer (node array, line lists, switchable line-list map) to the quad tree's layer array
// Leaf: ?push_back@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85E40, 105 bytes, thiscall `ret 4`.
// Identity: the only call site 0x00A7AA36 is CNavQuadTree::Initialise 0x00A7A8D0 (NavigationLayers +0x1C); the overflow
//   0x00A85B90 is this vector's _M_insert_overflow (it calls the layer __destroy_aux 0x00A85930). The catalog's
//   "Vector_AssignCopy_NavQuadLayer" is descriptive.
// Callees: the out-of-line STLport copy constructors of vector<CANavQuadTreeNode*> 0x00A80B80, vector<list<C2DLineF> >
//   0x00A83170 and vector<pair<unsigned __int64, list<C2DLineF> > > 0x00A83A30; _M_insert_overflow(pos, x, tag, 1, true)
//   0x00A85B90.
// Layout: CNavigationLayer (0x28) = Nodes +0x00, LineLists +0x0C, SwitchableLineLists +0x18 (CVectorMap: vector +0x18,
//   Compare +0x24, Dirty +0x25), as in the layer __destroy_aux 0x00A85930.
// Shape: STLport push_back inline: `_Construct(_M_finish, x)` = the layer's implicit copy constructor under the
//   placement-new null test (three vector copy calls, nothing for the empty comparator, the Dirty byte through cl),
//   then `++_M_finish` re-read after the calls; the full arm passes the empty tag homed in the dead `x` slot.
inline void* operator new(unsigned int, void* where) { return where; }

class C2DLineF;
class CANavQuadTreeNode;

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _T1, class _T2> class pair {
public:
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
};

template <class _Tp> class list {
public:
    void* _M_node;                                      // +0x00 sentinel
};

template <class _Tp> class allocator {
public:
    allocator() { }
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
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector(const vector& __x);                          // 0x00A80B80 / 0x00A83170 / 0x00A83A30 (element types)
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A85B90
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class T> class CArray : public std::vector<T> { };   // retail CArray<T>: the 12-byte STLport vector

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
};

class CNavQuadTree {
public:
    class CNavigationLayer {                            // 0x28 bytes
    public:
        CArray<CANavQuadTreeNode*>             Nodes;                // +0x00
        std::vector<std::list<C2DLineF> >    LineLists;            // +0x0c
        CVectorMap<unsigned __int64, std::list<C2DLineF>,
                   CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> > > SwitchableLineLists;   // +0x18
    };
};

template void std::vector<CNavQuadTree::CNavigationLayer>::push_back(const CNavQuadTree::CNavigationLayer& __x);
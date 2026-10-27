// CNavigatorManager::SaveNavigationMapToFile(long, CAFile&) const -- writes the quad tree of one navigation map to a file
// Retail: Fable.exe 0x00A14E50, 46 bytes: walks the QuadTrees list (sentinel at +0x24), compares each element's Index
// (node +0x10) with the argument and calls CNavQuadTree::SaveToFile(file) on its PQuadTree (node +8); `ret 8`.
// Identity: Ego_r.pdb ?SaveNavigationMapToFile@CNavigatorManager@@QBEXJAAVCAFile@@@Z; the callee 0x00A78B20 is
// ?SaveToFile@CNavQuadTree@@QBEXAAVCAFile@@@Z (catalog label). It sits right before the landed
// IsNavigationMapLoaded 0x00A14E80, which walks the same list the same way. The only caller 0x0050B2D4 passes
// (map index, file) with ecx = the world's navigator manager.
// CNavigatorManager comes from engine/CNavigatorManager.h (rename pattern; QuadTrees at the retail +0x24 is opaque
// bytes there), viewed as the STLport 4.6 std::list<CNavQuadTreeMap> (4 bytes: the sentinel pointer).
// CNavQuadTreeMap (Ego_r.pdb): PQuadTree CCountedPointer<CNavQuadTree> +0, Index +8, MapArea +0xC, SectionName +0x1C.
// Leaf: ?SaveNavigationMapToFile@CNavigatorManager@@QBEXJAAVCAFile@@@Z
class CAFile;
class CCPPointerInfo;
class CCharString { public: void* PStringData; };
class C2DBoxI { public: long X0, Y0, X1, Y1; };

#define CNavQuadTree DonorCNavQuadTree
#include "engine/CNavQuadTree.h"
#undef CNavQuadTree
class CNavQuadTree : public DonorCNavQuadTree {
public:
    void SaveToFile(CAFile& file) const;          // 0x00A78B20
};

template <class T> class CCountedPointer {
public:
    T*              Data;                          // +0x00
    CCPPointerInfo* Info;                          // +0x04
    T* operator->() const { return Data; }
};

class CNavQuadTreeMap {
public:
    CCountedPointer<CNavQuadTree> PQuadTree;       // +0x00
    long                          Index;           // +0x08
    C2DBoxI                       MapArea;         // +0x0C
    CCharString                   SectionName;     // +0x1C
};

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };

    struct _List_node_base {
        _List_node_base* _M_next;                  // +0x0
        _List_node_base* _M_prev;                  // +0x4
    };
    template <class _Tp> struct _List_node : public _List_node_base {
        _Tp _M_data;                               // +0x8
    };
    struct _List_iterator_base {
        _List_node_base* _M_node;
        _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
        void _M_incr() { _M_node = _M_node->_M_next; }
        bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
    };
    template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
    template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
        typedef _List_iterator<_Tp, _Traits> _Self;
        typedef _List_node<_Tp> _Node;
        typedef typename _Traits::reference reference;
        typedef typename _Traits::pointer pointer;
        _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
        _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
        reference operator*() const { return ((_Node*)this->_M_node)->_M_data; }
        pointer operator->() const { return &(operator*()); }
        _Self& operator++() { this->_M_incr(); return *this; }
    };
    template <class _Tp, class _Alloc> class _List_base {
    public:
        typedef _List_node<_Tp> _Node;
        _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
    };
    template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    public:
        typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
        const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
        const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    };
}

#define CNavigatorManager DonorCNavigatorManager
#include "engine/CNavigatorManager.h"
#undef CNavigatorManager

class CNavigatorManager : public DonorCNavigatorManager {
public:
    void SaveNavigationMapToFile(long index, CAFile& file) const;
    const std::list<CNavQuadTreeMap>& QuadTreeList() const { return *reinterpret_cast<const std::list<CNavQuadTreeMap>*>(QuadTrees); }
};

void CNavigatorManager::SaveNavigationMapToFile(long index, CAFile& file) const
{
    for (std::list<CNavQuadTreeMap>::const_iterator it = QuadTreeList().begin(); it != QuadTreeList().end(); ++it)
    {
        if (it->Index == index)
        {
            it->PQuadTree->SaveToFile(file);
            return;
        }
    }
}
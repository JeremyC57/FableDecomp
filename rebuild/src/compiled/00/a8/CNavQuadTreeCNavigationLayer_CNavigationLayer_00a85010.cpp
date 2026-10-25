// CNavQuadTree::CNavigationLayer::CNavigationLayer(const CNavigationLayer& other) -- copy constructor of a navigation layer: node array, line lists, then the switchable line-list map
// Leaf: ??0CNavigationLayer@CNavQuadTree@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A85010, 55 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>" is
//   propagated. The same three copy constructors and the +0xD byte appear inlined in the TU's
//   `_Construct<CNavQuadTree::CNavigationLayer>` (0x00A85360) next to CNavQuadTree::CopyAllLayers (0x00A85420).
//   Ego_r.pdb CNavQuadTree::CNavigationLayer: Nodes (CArray<CANavQuadTreeNode*>), LineLists
//   (vector<list<C2DLineF>>), SwitchableLineLists (CVectorMap<unsigned __int64, list<C2DLineF>, CKeyPairCompareLess<..>>);
//   with TLC's 12-byte STLport vectors they sit at +0, +0xC, +0x18, and the map's Dirty flag at +0x18+0xD.
// Callees (STLport vector copy constructors, reached through CArray's implicit copy): vector<CANavQuadTreeNode*> @ 0x00A80B80,
//   vector<list<C2DLineF>> @ 0x00A83170, vector<pair<unsigned __int64, list<C2DLineF>>> @ 0x00A83A30.
// Shape: memberwise copy; the CVectorMap's implicit copy constructor is inlined on its own `this` (ebx), so its data
//   vector is copied out of line, its empty comparator costs nothing and Dirty is a byte move through cl.
class CANavQuadTreeNode;
struct C2DLineF { float X0, Y0, X1, Y1; };

namespace std {
template <class _Tp> class allocator {};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                      // +0x00
    list(const list<_Tp, _Alloc>& __x);
    ~list();
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    vector(const vector<_Tp, _Alloc>& __x);
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    ~vector();
};
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

template <class K, class V> class CKeyPairCompareLess {};

template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                     // +0x00
    TCompare                  Compare;                  // +0x0C (empty comparator)
    bool                      Dirty;                    // +0x0D
};

class CNavQuadTree {
public:
    class CNavigationLayer {
    public:
        CArray<CANavQuadTreeNode*>                    Nodes;               // +0x00
        std::vector<std::list<C2DLineF> >           LineLists;           // +0x0C
        CVectorMap<unsigned __int64, std::list<C2DLineF>,
                   CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> > > SwitchableLineLists; // +0x18
        CNavigationLayer(const CNavigationLayer& other);
    };
};

CNavQuadTree::CNavigationLayer::CNavigationLayer(const CNavigationLayer& other)
    : Nodes(other.Nodes), LineLists(other.LineLists), SwitchableLineLists(other.SwitchableLineLists)
{
}
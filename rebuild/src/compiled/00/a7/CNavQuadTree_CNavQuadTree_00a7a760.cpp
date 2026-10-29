// CNavQuadTree::CNavQuadTree() -- empty navigation quad tree: not initialised/activated/initialising, empty layer/transition/topology/action-point/detailed-area/region-seed vectors, empty switchable-node and connector-node maps, empty region list
// Leaf: ??0CNavQuadTree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7A760, length 136 bytes (padded to 144).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, malloc @ 0x00BFEA0E (the region list's head node, 0x14 bytes).
// Identity: the catalog label "CPSCOrbit::CPSCOrbit" is wrong: the vtable stored is 0x0129DE64 = CNavQuadTree
// (rtti-vtables.tsv), and the caller 0x00A15748 allocates 0x88 bytes for it.
// Layout: PDB CNavQuadTree is 0xb0 bytes; retail is 0x88 (12-byte vectors, a 4-byte STLport list, 0x10-byte vector
// maps): Initialised +4, Activated +5, Initialising +6, MapPos +8, MapWidth +0x10, MapHeight +0x14, MapIndex +0x18
// (left unset), NavigationLayers +0x1c, TransitionPoints +0x28, TopologyWeights +0x34, SwitchableNodes +0x40
// (sorted flag +0x4d), ActionPoints +0x50, DetailedAreas +0x5c, RegionSeeds +0x68, Regions +0x74,
// ConnectorNodes +0x78 (flag +0x85). The engine header carries the PDB layout, so the class is modelled locally.
// Shape: bool init-list members after the vtable store, then each member's inline constructor; the region list is
// STLport's alloc-proxy form (the head word is zeroed, then replaced by the malloc'd self-linked head). The vectors
// must be the STLport _Vector_base with an _STLP_alloc_proxy end-of-storage member: with three plain pointers the
// malloc's `add esp,4` is scheduled above the connector map's zero stores.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                // 0x0099A2F0
};
class C2DVector { public:
    float X, Y;
    C2DVector() {}
};
template <class T> struct CStorageProxy {  // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CStorageProxy(T* p) : _M_data(p) {}
};
template <class T> struct CRetailVector {   // STLport vector (_Vector_base, 12 bytes)
    T*               _M_start;
    T*               _M_finish;
    CStorageProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class K, class V> class CVectorMap { public:     // retail CVectorMap (0x10 bytes)
    struct SEntry { K first; V second; };
    CRetailVector<SEntry> m_entries;        // +0x00
    unsigned char         m_compare;        // +0x0c (empty comparator)
    bool                  m_dirty;          // +0x0d
    CVectorMap() : m_dirty(false) {}
};
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CNodeAllocProxy { // STLport _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> >
    SListNode<T>* _M_data;
    CNodeAllocProxy(SListNode<T>* p) : _M_data(p) {}
    SListNode<T>* allocate(unsigned int n) { return n != 0 ? (SListNode<T>*)malloc(n * sizeof(SListNode<T>)) : 0; }
};
template <class T> struct CRetailList {     // STLport list (4 bytes: the head node)
    CNodeAllocProxy<T> _M_node;
    CRetailList() : _M_node(0)
    {
        SListNode<T>* head = _M_node.allocate(1);
        head->_M_next = head;
        head->_M_prev = head;
        _M_node._M_data = head;
    }
};

struct CNavigationLayer;
class CNavigationTransitionPoint;
class CNavigationPosition { public: unsigned char _body[8]; };
class C2DBoxF { public: float m[4]; };
class CANavQuadTreeNode;
struct CNavConnectorNodePointer { void* Data; void* Info; };
class CNavRegion { public: unsigned char _body[0xc]; };

class CNavQuadTree : public CBaseClassNonCopyable {         // vtable 0x0129DE64
public:
    bool                                CInitialised;           // +0x04 Initialised
    bool                                Activated;              // +0x05
    bool                                Initialising;           // +0x06
    C2DVector                           MapPos;                 // +0x08
    float                               MapWidth;               // +0x10
    float                               MapHeight;              // +0x14
    long                                MapIndex;               // +0x18
    CRetailVector<CNavigationLayer*>    NavigationLayers;       // +0x1c
    CRetailVector<CNavigationTransitionPoint*> TransitionPoints;// +0x28
    CRetailVector<unsigned char>        TopologyWeights;        // +0x34
    CVectorMap<unsigned long, CANavQuadTreeNode*> SwitchableNodes;  // +0x40 (key is a 64-bit id in the PDB)
    CRetailVector<CNavigationPosition>  ActionPoints;           // +0x50
    CRetailVector<C2DBoxF>              DetailedAreas;          // +0x5c
    CRetailVector<CNavigationPosition>  RegionSeeds;            // +0x68
    CRetailList<CNavRegion>             Regions;                // +0x74
    CVectorMap<long, CNavConnectorNodePointer> ConnectorNodes;  // +0x78
    CNavQuadTree();
};

CNavQuadTree::CNavQuadTree()
    : CInitialised(false), Activated(false), Initialising(false)
{
}
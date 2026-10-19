// CNavQuadTree::~CNavQuadTree() -- free the quad-tree nodes, then tear down the navigation containers in reverse declaration order and the CBaseClassNonCopyable base
// Leaf: ??1CNavQuadTree
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7A7F0, 212 bytes (+ int3 padding). Vtable 0x0129DE64 (CNavQuadTree, RTTI); called
//   only from the scalar deleting destructor 0x00A85E20 (slot 0). The catalogued name "~CPatchTesselationEdgeStrip" is a
//   propagated label.
// Callees: CNavQuadTree::ClearNodes 0x00A792E0, _Destroy_range for the connector-node pairs 0x00A80E40 and for the
//   navigation layers 0x00A85930 (both `__fastcall` first, last, plus an empty STLport `__false_type` tag by reference),
//   CRetailList_ListOfCountedPointer::clear 0x00A84110 (the region list's clear, landed under that name), free 0x00BFEA14,
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Shape: the AI navigation library's containers allocate with malloc/free (retail STLport-style 12-byte vectors, a
//   4-byte list holding its head node, CVectorMap = vector + 4-byte comparator). Retail layout (the PDB/header layout
//   is the debug one with 16-byte vectors, so this is a local model): NavigationLayers +0x1C, TransitionPoints +0x28,
//   TopologyWeights +0x34, SwitchableNodes +0x40, ActionPoints +0x50, DetailedAreas +0x5C, RegionSeeds +0x68,
//   Regions +0x74, ConnectorNodes +0x78. The user destructor stores the vtable and calls ClearNodes; each member's
//   inline destructor runs (destroy range with the tag temporary in the `push ecx` frame byte, null-tested free of the
//   start; the list clears and frees its head node, re-read through edi).
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

struct __false_type { __false_type() {} };

class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();               // 0x0099A300
};

struct CNavigationLayer;                            // CNavQuadTree::CNavigationLayer (non-trivial destructor)
struct CNavigationTransitionPoint;
struct CNavigationPosition;
struct C2DBoxF;
struct CANavQuadTreeNode;
struct CNavConnectorNodePair;                       // pair<long, CCountedPointer<CNavConnectorNode> >
struct SwitchableNodePair;                          // pair<unsigned __int64, CANavQuadTreeNode*>

void __fastcall _Destroy_range(CNavigationLayer* first, CNavigationLayer* last, const __false_type& tag);             // 0x00A85930
void __fastcall _Destroy_range(CNavConnectorNodePair* first, CNavConnectorNodePair* last, const __false_type& tag);   // 0x00A80E40

template <class T> struct vector {                  // elements with a destructor
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy_range(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
};
template <class T> struct pod_vector {              // trivially destructible elements
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~pod_vector()
    {
        if (_M_start)
            free(_M_start);
    }
};
template <class V> struct CVectorMap {
    V    Data;
    long Compare;                                   // empty comparator, padded
};

struct CRetailList_ListOfCountedPointer {           // list<CNavRegion>: one head-node pointer
    void* head;
    void clear();                                   // 0x00A84110
    ~CRetailList_ListOfCountedPointer()
    {
        clear();
        if (head)
            free(head);
    }
};

class CNavQuadTree : public CBaseClassNonCopyable { // vtable 0x0129DE64
public:
    bool  Initialised;                              // +0x04
    bool  Activated;                                // +0x05
    bool  Initialising;                             // +0x06
    float MapPos[2];                                // +0x08 C2DVector
    float MapWidth;                                 // +0x10
    float MapHeight;                                // +0x14
    long  MapIndex;                                 // +0x18
    vector<CNavigationLayer>                    NavigationLayers;   // +0x1C
    pod_vector<CNavigationTransitionPoint>      TransitionPoints;   // +0x28
    pod_vector<unsigned char>                   TopologyWeights;    // +0x34
    CVectorMap<pod_vector<SwitchableNodePair> > SwitchableNodes;    // +0x40
    pod_vector<CNavigationPosition>             ActionPoints;       // +0x50
    pod_vector<C2DBoxF>                         DetailedAreas;      // +0x5C
    pod_vector<CNavigationPosition>             RegionSeeds;        // +0x68
    CRetailList_ListOfCountedPointer            Regions;            // +0x74
    CVectorMap<vector<CNavConnectorNodePair> >  ConnectorNodes;     // +0x78
    void ClearNodes();                              // 0x00A792E0
    virtual ~CNavQuadTree();
};

CNavQuadTree::~CNavQuadTree()
{
    ClearNodes();
}

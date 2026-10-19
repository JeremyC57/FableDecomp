// CNavigatorManager::ActivateNavigationMap(long) -- activate the quad-tree map of the map index (first time only: mark it active and link it to the neighbouring maps) and, with the debug focus flag on, point the debug view at the map's first node centre
// Leaf: ActivateNavigationMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A16050, 165 bytes (`ret 4`).
// Callees: CNavQuadTree::Activate 0x00A762E0 (`mov byte ptr [ecx+5],1`: Activated = true; manifest label "CXMVPlaybackInfo" is
//   propagated), CNavQuadTree::LinkToNeighbouringMaps(const CNavigatorManager&) 0x00A7A650, the static half-cell size
//   0x00A76320 (returns 0.5f, called without `this`), the debug-focus call 0x00A15890 on the manager; debug flag bytes
//   0x013D277D (focus on activation) and 0x013D277C (its bool argument).
// Layout: Ego_r.pdb CNavigatorManager with STLport containers: QuadTrees (list<CNavQuadTreeMap>) at retail +0x24;
//   CNavQuadTreeMap {PQuadTree +0, Index +8, MapArea +0xc}; CNavQuadTree Activated +0x05.
// Shape: the miss returns from the scan loop; the counted pointer is re-read for each call; the focus point is
//   C2DVector(MapArea top-left C2DCoordI temporary) + C2DVector(half, half) (the second half-size call is the X operand, as
//   arguments are evaluated right to left), the origin's X staying on the FPU.
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DCoordI& c) : X((float)c.X), Y((float)c.Y) { }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DCoordI GetTopLeft() const { return C2DCoordI(TLX, TLY); }
};
struct CCharString { void* m_pData; };
class CNavigatorManager;
class CNavQuadTree {                               // 0x88 bytes
public:
    void* __vftable;                               // +0x00
    bool  Initialised;                             // +0x04
    bool  Activated;                               // +0x05
    bool  Initialising;                            // +0x06
    void Activate();                               // 0x00A762E0
    void LinkToNeighbouringMaps(const CNavigatorManager& manager);   // 0x00A7A650
    static float GetHalfCellSize_00a76320();       // 0x00A76320 (0.5f)
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct CNavQuadTreeMap {                           // 32 bytes
    CCountedPointer<CNavQuadTree> PQuadTree;       // +0x00
    long                          Index;           // +0x08
    C2DBoxI                       MapArea;         // +0x0c
    CCharString                   SectionName;     // +0x1c
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;                           // +0x08
};
template <class T> struct list {                   // STLport list, 4 bytes
    _List_node<T>* _M_node;
};
extern bool g_NavDebugFocusOnActivate_013d277d;    // 0x013D277D
extern bool g_NavDebugFocusFlag_013d277c;          // 0x013D277C
class CNavigatorManager {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _pad04[0x20];
    list<CNavQuadTreeMap> QuadTrees;               // +0x24
    void DebugFocusOn_00a15890(const C2DVector& pos, bool flag);   // 0x00A15890
    void ActivateNavigationMap(long mapIndex);
};

void CNavigatorManager::ActivateNavigationMap(long mapIndex)
{
    for (_List_node<CNavQuadTreeMap>* it = QuadTrees._M_node->_M_next; it != QuadTrees._M_node; it = it->_M_next) {
        if (it->_M_data.Index == mapIndex) {
            if (!it->_M_data.PQuadTree->Activated) {
                it->_M_data.PQuadTree->Activate();
                it->_M_data.PQuadTree->LinkToNeighbouringMaps(*this);
            }
            if (g_NavDebugFocusOnActivate_013d277d) {
                C2DVector pos = C2DVector(it->_M_data.MapArea.GetTopLeft()) +
                                C2DVector(CNavQuadTree::GetHalfCellSize_00a76320(), CNavQuadTree::GetHalfCellSize_00a76320());
                DebugFocusOn_00a15890(pos, g_NavDebugFocusFlag_013d277c);
            }
            return;
        }
    }
}

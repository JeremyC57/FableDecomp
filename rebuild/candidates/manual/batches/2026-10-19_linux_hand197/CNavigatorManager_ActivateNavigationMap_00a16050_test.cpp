#include <cstdio>
// Behaviour test for 00a16050 CNavigatorManager::ActivateNavigationMap: only the map with the index is touched, activation and
// neighbour linking happen once, the debug focus (when enabled) goes to top-left + (0.5, 0.5) with the flag byte.
class CNavigatorManager;
static int g_activates = 0, g_links = 0, g_focus = 0; static const CNavigatorManager* g_linkedWith = 0;
static float g_fx = 0, g_fy = 0; static bool g_fflag = false;
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
    void Activate() { Activated = true; ++g_activates; }
    void LinkToNeighbouringMaps(const CNavigatorManager& manager) { g_linkedWith = &manager; ++g_links; }
    static float GetHalfCellSize_00a76320() { return 0.5f; }
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
bool g_NavDebugFocusOnActivate_013d277d;
bool g_NavDebugFocusFlag_013d277c;
class CNavigatorManager {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _pad04[0x20];
    list<CNavQuadTreeMap> QuadTrees;               // +0x24
    void DebugFocusOn_00a15890(const C2DVector& pos, bool flag) { g_fx = pos.X; g_fy = pos.Y; g_fflag = flag; ++g_focus; }
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

typedef _List_node<CNavQuadTreeMap> Node;
int main()
{
    static CNavigatorManager m;
    static CNavQuadTree t1, t2;
    t1.Activated = false; t2.Activated = false;
    Node header, n1, n2;
    n1._M_data.Index = 3; n1._M_data.PQuadTree.Data = &t1; n1._M_data.MapArea.TLX = 100; n1._M_data.MapArea.TLY = -40;
    n2._M_data.Index = 7; n2._M_data.PQuadTree.Data = &t2; n2._M_data.MapArea.TLX = 4; n2._M_data.MapArea.TLY = 8;
    header._M_next = &n1; n1._M_next = &n2; n2._M_next = &header;
    m.QuadTrees._M_node = &header;
    m.ActivateNavigationMap(7);
    bool ok = t2.Activated && !t1.Activated && g_activates == 1 && g_links == 1 && g_linkedWith == &m && g_focus == 0;
    m.ActivateNavigationMap(7);                       // already active: no second activation
    ok = ok && g_activates == 1 && g_links == 1;
    g_NavDebugFocusOnActivate_013d277d = true; g_NavDebugFocusFlag_013d277c = true;
    m.ActivateNavigationMap(3);
    ok = ok && t1.Activated && g_activates == 2 && g_focus == 1 && g_fx == 100.5f && g_fy == -39.5f && g_fflag;
    m.ActivateNavigationMap(9);                       // unknown index: nothing
    ok = ok && g_activates == 2 && g_focus == 1;
    if (!ok) { std::printf("00a16050_TEST FAIL\n"); return 1; }
    std::printf("00a16050_TEST PASS\n");
    return 0;
}

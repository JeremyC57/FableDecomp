// Behaviour test for 00a156d0: src/00a156d0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CNavigatorManager::AddNavigationMap(long, CNavigationAreaInit&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CNavigatorManager::AddNavigationMap(long, CNavigationAreaInit&) -- add a quad-tree map for the map index unless one exists: append an entry, give it the init's world area, a new CNavQuadTree set up over that area and initialised from the init record
// Leaf: ?AddNavigationMap@CNavigatorManager@@QAEXJAAUCNavigationAreaInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A156D0, 237 bytes (+ int3 padding; `ret 8`).
// Callees: list<CNavQuadTreeMap>::insert(iterator) 0x00A17480 (STLport insert(pos, T()), landed under the propagated label
//   "CCombatSequence_WillForcePush::OnCreate"), operator new 0x00BFEA1A, CNavQuadTree::CNavQuadTree 0x00A7A760 (manifest label
//   "CPSCOrbit::CPSCOrbit" is propagated), CCountedPointer<CNavQuadTree>::Reset(T*) 0x00A16640, CNavQuadTree::SetUpMapStructure
//   (const C2DVector&, float, float, long) 0x00A762F0, CNavQuadTree::Initialise(CNavigationAreaInit&) 0x00A7A8D0.
// Layout: Ego_r.pdb CNavigatorManager with STLport containers: QuadTrees (list<CNavQuadTreeMap>) at retail +0x24;
//   CNavQuadTreeMap {PQuadTree +0, Index +8, MapArea +0xc, SectionName +0x1c}; CNavigationAreaInit WorldArea at retail +0x30
//   (PDB +0x40).
// Shape: the duplicate scan returns straight to the epilogue; the inserted iterator lands in the dead index-argument slot and
//   the entry is re-read as back(); the area is the implicit C2DBoxI copy; the origin is a named C2DVector local built before
//   the width/height arguments (each `(float)long` goes through a frame slot and fild).
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const struct C2DCoordI& c);
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
inline C2DVector::C2DVector(const C2DCoordI& c) : X((float)c.X), Y((float)c.Y) { }
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DCoordI GetTopLeft() const { return C2DCoordI(TLX, TLY); }
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
};
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CAFile;
class CASuspendableProcess;
struct CNavigationAreaInit;
class CNavQuadTree {                               // 0x88 bytes
public:
    unsigned char _body[0x88];
    CNavQuadTree();                                // 0x00A7A760
    void SetUpMapStructure(const C2DVector& origin, float width, float height, long mapIndex);   // 0x00A762F0
    void Initialise(CNavigationAreaInit& init);    // 0x00A7A8D0
    bool LoadFromFile(CAFile& file, long mapIndex, const C2DBoxI& area, CASuspendableProcess* process);   // 0x00A7AF60
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
    void Reset(T* object);                         // 0x00A16640 for CNavQuadTree
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
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) { }
    _List_iterator(const _List_iterator& it) : _M_node(it._M_node) { }
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
template <class T> class list {                    // STLport list, 4 bytes
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
    T& back() { return _M_node->_M_prev->_M_data; }
    iterator insert(iterator pos);                 // 0x00A17480: insert(pos, T())
    iterator erase(iterator pos);                  // 0x00A17110
};
struct CNavigationAreaInit {
    unsigned char _pad0[0x30];
    C2DBoxI       WorldArea;                       // +0x30
};
class CNavigatorManager {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _pad04[0x20];
    list<CNavQuadTreeMap> QuadTrees;               // +0x24
    void AddNavigationMap(long mapIndex, CNavigationAreaInit& init);
};

void CNavigatorManager::AddNavigationMap(long mapIndex, CNavigationAreaInit& init)
{
    {
        const list<CNavQuadTreeMap>& maps = QuadTrees;
        for (list<CNavQuadTreeMap>::iterator it = maps.begin(); it != maps.end(); ++it) {
            if (it->Index == mapIndex)
                return;
        }
    }
    QuadTrees.insert(QuadTrees.end());
    CNavQuadTreeMap& map = QuadTrees.back();
    map.Index = mapIndex;
    map.MapArea = init.WorldArea;
    map.PQuadTree.Reset(new CNavQuadTree);
    map.PQuadTree->SetUpMapStructure(map.MapArea.GetTopLeft(), (float)map.MapArea.GetWidth(), (float)map.MapArea.GetHeight(), mapIndex);
    map.PQuadTree->Initialise(init);
}

// ---- stubs and driver ----

#include <stdlib.h>
CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
static int g_trees = 0;
CNavQuadTree::CNavQuadTree() { ++g_trees; }
static float g_ox, g_oy, g_w, g_h; static long g_setIndex; static int g_inits = 0; static CNavigationAreaInit* g_init;
void CNavQuadTree::SetUpMapStructure(const C2DVector& origin, float width, float height, long mapIndex) { g_ox = origin.X; g_oy = origin.Y; g_w = width; g_h = height; g_setIndex = mapIndex; }
void CNavQuadTree::Initialise(CNavigationAreaInit& init) { g_init = &init; ++g_inits; }
bool CNavQuadTree::LoadFromFile(CAFile&, long, const C2DBoxI&, CASuspendableProcess*) { return true; }
template <> void CCountedPointer<CNavQuadTree>::Reset(CNavQuadTree* object) { Data = object; Info = (void*)1; }
typedef list<CNavQuadTreeMap> TList;
template <> TList::iterator TList::insert(iterator pos)
{
    _List_node<CNavQuadTreeMap>* n = (_List_node<CNavQuadTreeMap>*)malloc(sizeof(_List_node<CNavQuadTreeMap>));
    memset(&n->_M_data, 0, sizeof n->_M_data);
    _List_node<CNavQuadTreeMap>* p = pos._M_node; n->_M_next = p; n->_M_prev = p->_M_prev; p->_M_prev->_M_next = n; p->_M_prev = n;
    return iterator(n);
}
template <> TList::iterator TList::erase(iterator pos) { return pos; }
static int Count(_List_node<CNavQuadTreeMap>* h) { int c = 0; for (_List_node<CNavQuadTreeMap>* n = h->_M_next; n != h; n = n->_M_next) ++c; return c; }
int main()
{
    static unsigned char hs[sizeof(_List_node<CNavQuadTreeMap>)]; _List_node<CNavQuadTreeMap>* head = (_List_node<CNavQuadTreeMap>*)hs;
    head->_M_next = head; head->_M_prev = head;
    static CNavigatorManager mgr; mgr.QuadTrees._M_node = head;
    static CNavigationAreaInit init; init.WorldArea.TLX = 10; init.WorldArea.TLY = 20; init.WorldArea.BRX = 110; init.WorldArea.BRY = 70;
    mgr.AddNavigationMap(3, init);
    CHECK(Count(head) == 1); CNavQuadTreeMap& m = head->_M_next->_M_data;
    CHECK(m.Index == 3); CHECK(m.MapArea.TLX == 10 && m.MapArea.BRY == 70); CHECK(g_trees == 1); CHECK(m.PQuadTree.Data != 0);
    CHECK(g_ox == 10.0f && g_oy == 20.0f); CHECK(g_w == 100.0f); CHECK(g_h == 50.0f); CHECK(g_setIndex == 3); CHECK(g_inits == 1); CHECK(g_init == &init);
    mgr.AddNavigationMap(3, init);
    CHECK(Count(head) == 1); CHECK(g_trees == 1); CHECK(g_inits == 1);
    if (fails == 0) printf("00a156d0_TEST PASS\n"); else printf("00a156d0_TEST FAIL\n");
    return fails;
}
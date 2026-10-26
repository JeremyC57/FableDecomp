// Behaviour test for 00a157c0: src/00a157c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CNavigatorManager::AddNavigationMap(long, const C2DBoxI&, const CCharString&, CAFile&, CASuspendableProcess*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CNavigatorManager::AddNavigationMap(long, const C2DBoxI&, const CCharString&, CAFile&, CASuspendableProcess*) -- add a quad-tree map for the map index unless one exists: append an entry with the area and section name, load a new CNavQuadTree for it from the file, and drop the entry again when the load fails
// Leaf: ?AddNavigationMap@CNavigatorManager@@QAEXJABVC2DBoxI@@ABVCCharString@@AAVCAFile@@PAVCASuspendableProcess@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A157C0, 196 bytes (+ int3 padding; `ret 14h`).
// Callees: list<CNavQuadTreeMap>::insert(iterator) 0x00A17480 and erase(iterator) 0x00A17110, CCharString::operator= 0x0099EFB0,
//   operator new 0x00BFEA1A, CNavQuadTree::CNavQuadTree 0x00A7A760, CCountedPointer<CNavQuadTree>::Reset(T*) 0x00A16640,
//   CNavQuadTree::LoadFromFile(CAFile&, long, const C2DBoxI&, CASuspendableProcess*) 0x00A7AF60 (a bool in al).
// Layout: as the CNavigationAreaInit overload 0x00A156D0: QuadTrees at retail +0x24; CNavQuadTreeMap {PQuadTree +0, Index +8,
//   MapArea +0xc, SectionName +0x1c}.
// Shape: the inserted entry is re-read as back(); on a failed load the last entry is erased through a by-value iterator
//   (--end(), the prev link of the header) whose result lands in the dead index-argument slot.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CCharString { public:
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CAFile;
class CASuspendableProcess;
class CNavigationAreaInit;
class CNavQuadTree {                               // 0x88 bytes
public:
    unsigned char _body[0x88];
    CNavQuadTree();                                // 0x00A7A760
    void SetUpMapStructure(const C2DVector& origin, float width, float height, long mapIndex);   // 0x00A762F0
    void Initialise(CNavigationAreaInit& init);    // 0x00A7A8D0
    bool LoadFromFile(CAFile& file, long mapIndex, const C2DBoxI& area, CASuspendableProcess* process);   // 0x00A7AF60
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
    void Reset(T* object);                         // 0x00A16640 for CNavQuadTree
};
class CNavQuadTreeMap { public:                           // 32 bytes
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
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
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
class CNavigatorManager {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _pad04[0x20];
    list<CNavQuadTreeMap> QuadTrees;               // +0x24
    void AddNavigationMap(long mapIndex, const C2DBoxI& area, const CCharString& sectionName, CAFile& file, CASuspendableProcess* process);
};

void CNavigatorManager::AddNavigationMap(long mapIndex, const C2DBoxI& area, const CCharString& sectionName, CAFile& file, CASuspendableProcess* process)
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
    map.MapArea = area;
    map.SectionName = sectionName;
    map.PQuadTree.Reset(new CNavQuadTree);
    if (!map.PQuadTree->LoadFromFile(file, mapIndex, area, process))
        QuadTrees.erase(QuadTrees._M_node->_M_prev);
}

// ---- stubs and driver ----

#include <stdlib.h>
CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
static int g_trees = 0;
CNavQuadTree::CNavQuadTree() { ++g_trees; }
static bool g_loadOk = true; static long g_loadIndex; static const C2DBoxI* g_loadArea; static CAFile* g_loadFile; static CASuspendableProcess* g_loadProc;
bool CNavQuadTree::LoadFromFile(CAFile& file, long mapIndex, const C2DBoxI& area, CASuspendableProcess* process)
{ g_loadFile = &file; g_loadIndex = mapIndex; g_loadArea = &area; g_loadProc = process; return g_loadOk; }
template <> void CCountedPointer<CNavQuadTree>::Reset(CNavQuadTree* object) { Data = object; Info = (void*)1; }
typedef list<CNavQuadTreeMap> TList;
template <> TList::iterator TList::insert(iterator pos)
{
    _List_node<CNavQuadTreeMap>* n = (_List_node<CNavQuadTreeMap>*)malloc(sizeof(_List_node<CNavQuadTreeMap>));
    memset(&n->_M_data, 0, sizeof n->_M_data);
    _List_node<CNavQuadTreeMap>* p = pos._M_node; n->_M_next = p; n->_M_prev = p->_M_prev; p->_M_prev->_M_next = n; p->_M_prev = n;
    return iterator(n);
}
static int g_erased = 0;
template <> TList::iterator TList::erase(iterator pos)
{
    _List_node<CNavQuadTreeMap>* n = pos._M_node; _List_node<CNavQuadTreeMap>* next = n->_M_next;
    n->_M_prev->_M_next = next; next->_M_prev = n->_M_prev; free(n); ++g_erased; return iterator(next);
}
static int Count(_List_node<CNavQuadTreeMap>* h) { int c = 0; for (_List_node<CNavQuadTreeMap>* n = h->_M_next; n != h; n = n->_M_next) ++c; return c; }
int main()
{
    static unsigned char hs[sizeof(_List_node<CNavQuadTreeMap>)]; _List_node<CNavQuadTreeMap>* head = (_List_node<CNavQuadTreeMap>*)hs;
    head->_M_next = head; head->_M_prev = head;
    static CNavigatorManager mgr; mgr.QuadTrees._M_node = head;
    C2DBoxI area = { 1, 2, 3, 4 }; CCharString name; name.m_pData = (void*)0x55; CAFile* file = (CAFile*)0x66; CASuspendableProcess* proc = (CASuspendableProcess*)0x77;
    mgr.AddNavigationMap(5, area, name, *file, proc);
    CHECK(Count(head) == 1); CNavQuadTreeMap& m = head->_M_next->_M_data;
    CHECK(m.Index == 5); CHECK(m.MapArea.TLX == 1 && m.MapArea.BRY == 4); CHECK(m.SectionName.m_pData == (void*)0x55);
    CHECK(g_trees == 1); CHECK(m.PQuadTree.Data != 0); CHECK(g_loadIndex == 5); CHECK(g_loadArea == &area); CHECK(g_loadFile == file); CHECK(g_loadProc == proc);
    mgr.AddNavigationMap(5, area, name, *file, proc);      // already present
    CHECK(Count(head) == 1); CHECK(g_trees == 1);
    g_loadOk = false;
    mgr.AddNavigationMap(6, area, name, *file, proc);      // load fails: the new entry is removed again
    CHECK(Count(head) == 1); CHECK(g_erased == 1); CHECK(g_trees == 2); CHECK(head->_M_next->_M_data.Index == 5);
    if (fails == 0) printf("00a157c0_TEST PASS\n"); else printf("00a157c0_TEST FAIL\n");
    return fails;
}
// Behaviour test for 00a786c0: src/00a786c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CNavRegion::SetAsConnectedToRegion(CNavRegion* region)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CNavRegion::SetAsConnectedToRegion(CNavRegion* region) -- count one more connection to a region already linked to this one, or create a shared connection record (both regions, one connection) and add it to this region's and the other region's connection lists
// Leaf: SetAsConnectedToRegion
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A786C0, 265 bytes (+ int3 padding), `ret 4`.
// Callees: operator new 0x00BFEA1A (the connection and its count block), malloc 0x00BFEA0E (list nodes), operator delete
//   0x00BFE9BC (count block release), the counted pointer's static deleter 0x00A7C0A0 (stored as the block's destroy
//   function; manifest label "operator_new" is wrong).
// Layout: Ego_r.pdb CNavRegion: RegionID +0, Connections +4 (STLport list, retail 4 bytes), MostRecentSearchID +8;
//   CNavRegionConnection (0xc): FirstRegion, SecondRegion, NumberOfConnections.
// Shape: the new connection's counted pointer is a block-scoped local (released before the second list append); the
//   raw-pointer counted-pointer ctor allocates the {count, destroy, object} block only for a non-null object; both list
//   appends are the inline STLport push_back with the copy ctor's add-ref.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct SCountBlock {
    long  m_references;                                 // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
    SCountBlock(void* object, void (__fastcall* destroy)(void*)) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) { }
};
struct CNavRegion;
struct CNavRegionConnection {
    CNavRegion* FirstRegion;                            // +0
    CNavRegion* SecondRegion;                           // +4
    long        NumberOfConnections;                    // +8
    CNavRegionConnection(CNavRegion* first, CNavRegion* second) : FirstRegion(first), SecondRegion(second), NumberOfConnections(1) { }
    CNavRegion* GetOtherRegion(const CNavRegion* region) const { return region == FirstRegion ? SecondRegion : FirstRegion; }
    void AddConnection() { ++NumberOfConnections; }
};
void __fastcall DeleteNavRegionConnection(void* object);   // 0x00A7C0A0 (CCountedPointer<CNavRegionConnection> deleter)

template <class T> struct CCountedPointer {
    T*           Data;
    SCountBlock* Info;
    CCountedPointer(T* object) : Data(object)
    {
        if (Data)
            Info = new SCountBlock(Data, DeleteNavRegionConnection);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
    T* operator->() const { return Data; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
    }
};
typedef CCountedPointer<CNavRegionConnection> CConnectionPointer;

struct SConnectionNode {                                // _List_node<CConnectionPointer>
    SConnectionNode*   _M_next;
    SConnectionNode*   _M_prev;
    CConnectionPointer _M_data;
};
struct CConnectionList {                                // STLport list (retail: the sentinel pointer)
    struct iterator {
        SConnectionNode* _M_node;
        iterator(SConnectionNode* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
        CConnectionPointer& operator*() const { return _M_node->_M_data; }
        CConnectionPointer* operator->() const { return &_M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SConnectionNode* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    const CConnectionPointer& back() const { return _M_node->_M_prev->_M_data; }
    SConnectionNode* _M_create_node(const CConnectionPointer& x)
    {
        SConnectionNode* node = (SConnectionNode*)malloc(sizeof(SConnectionNode));
        new (&node->_M_data) CConnectionPointer(x);
        return node;
    }
    void insert(iterator position, const CConnectionPointer& x)
    {
        SConnectionNode* node = _M_create_node(x);
        SConnectionNode* pos = position._M_node;
        SConnectionNode* prev = pos->_M_prev;
        node->_M_next = pos;
        node->_M_prev = prev;
        prev->_M_next = node;
        pos->_M_prev = node;
    }
    void push_back(const CConnectionPointer& x) { insert(end(), x); }
};

struct CNavRegion {
    long            RegionID;                           // +0
    CConnectionList Connections;                        // +4
    long            MostRecentSearchID;                 // +8
    void SetAsConnectedToRegion(CNavRegion* region);
};

void CNavRegion::SetAsConnectedToRegion(CNavRegion* region)
{
    for (CConnectionList::iterator it = Connections.begin(); it != Connections.end(); ++it) {
        if ((*it)->GetOtherRegion(this) == region) {
            (*it)->AddConnection();
            return;
        }
    }
    {
        CConnectionPointer connection(new CNavRegionConnection(this, region));
        Connections.push_back(connection);
    }
    region->Connections.push_back(Connections.back());
}

// ---- stubs and driver ----

static int g_deleted = 0;
void __fastcall DeleteNavRegionConnection(void* object) { ++g_deleted; delete (CNavRegionConnection*)object; }
static int Count(SConnectionNode* h) { int c = 0; for (SConnectionNode* n = h->_M_next; n != h; n = n->_M_next) ++c; return c; }
int main()
{
    static unsigned char hs[3][sizeof(SConnectionNode)]; SConnectionNode& ha = *(SConnectionNode*)hs[0]; SConnectionNode& hb = *(SConnectionNode*)hs[1]; SConnectionNode& hc = *(SConnectionNode*)hs[2];
    ha._M_next = ha._M_prev = &ha; hb._M_next = hb._M_prev = &hb; hc._M_next = hc._M_prev = &hc;
    CNavRegion a, b, c; a.RegionID = 1; b.RegionID = 2; c.RegionID = 3;
    a.Connections._M_node = &ha; b.Connections._M_node = &hb; c.Connections._M_node = &hc;
    a.SetAsConnectedToRegion(&b);
    CHECK(Count(&ha) == 1); CHECK(Count(&hb) == 1);
    CNavRegionConnection* ab = ha._M_next->_M_data.Data;
    CHECK(ab->FirstRegion == &a); CHECK(ab->SecondRegion == &b); CHECK(ab->NumberOfConnections == 1);
    CHECK(hb._M_next->_M_data.Data == ab); CHECK(ha._M_next->_M_data.Info == hb._M_next->_M_data.Info);
    CHECK(ha._M_next->_M_data.Info->m_references == 2); CHECK(ha._M_next->_M_data.Info->m_pfnDestroy == DeleteNavRegionConnection);
    a.SetAsConnectedToRegion(&b);                 // existing: count bumped
    CHECK(Count(&ha) == 1); CHECK(ab->NumberOfConnections == 2);
    b.SetAsConnectedToRegion(&a);                 // from the other side: found through GetOtherRegion
    CHECK(Count(&hb) == 1); CHECK(ab->NumberOfConnections == 3);
    a.SetAsConnectedToRegion(&c);
    CHECK(Count(&ha) == 2); CHECK(Count(&hc) == 1); CHECK(ha._M_prev->_M_data.Data->SecondRegion == &c);
    CHECK(ha._M_prev->_M_data.Info->m_references == 2); CHECK(g_deleted == 0);
    if (fails == 0) printf("00a786c0_TEST PASS\n"); else printf("00a786c0_TEST FAIL\n");
    return fails;
}
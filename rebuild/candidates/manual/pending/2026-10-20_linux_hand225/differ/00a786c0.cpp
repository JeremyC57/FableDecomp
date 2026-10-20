// CNavRegion::SetAsConnectedToRegion(CNavRegion* region) -- count one more connection to a region already linked to this one, or create a shared connection record (both regions, one connection) and add it to this region's and the other region's connection lists
// Grade: DIFFER(269v265) -- callee-saved push placement: ours pushes ebp/edi after the search loop (the early return pops two), retail pushes ebp at entry and edi before the loop (the early return pops four)
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
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
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
    SConnectionNode* _M_node;
    const CConnectionPointer& back() const { return _M_node->_M_prev->_M_data; }
    __forceinline void push_back(const CConnectionPointer& x)
    {
        SConnectionNode* position = _M_node;
        SConnectionNode* node = (SConnectionNode*)malloc(sizeof(SConnectionNode));
        new (&node->_M_data) CConnectionPointer(x);
        SConnectionNode* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
    }
};

struct CNavRegion {
    long            RegionID;                           // +0
    CConnectionList Connections;                        // +4
    long            MostRecentSearchID;                 // +8
    void SetAsConnectedToRegion(CNavRegion* region);
};

void CNavRegion::SetAsConnectedToRegion(CNavRegion* region)
{
    for (SConnectionNode* it = Connections._M_node->_M_next; it != Connections._M_node; it = it->_M_next) {
        if (it->_M_data->GetOtherRegion(this) == region) {
            it->_M_data->AddConnection();
            return;
        }
    }
    {
        CConnectionPointer connection(new CNavRegionConnection(this, region));
        Connections.push_back(connection);
    }
    region->Connections.push_back(Connections.back());
}

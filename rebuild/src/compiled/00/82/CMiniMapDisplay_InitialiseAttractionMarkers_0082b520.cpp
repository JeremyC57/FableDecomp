// CMiniMapDisplay::InitialiseAttractionMarkers() -- when a region is loaded, add a minimap marker for every creature in the current region with a wife interface (0xd7): marker graphics[7].Primary when married to the player, [7].Secondary when attracted to the hero
// Leaf: InitialiseAttractionMarkers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082B520, 262 bytes (+ int3 padding).
// Callees: the game's world (0x013B86A0 +0x24) slots 4 (+0x10) and 13 (+0x34); CWorldMap::GetRegionNumberMapIsIn 0x004FC190 /
//   GetCurrentRegionNumber 0x004FB150, the const LowerBound 0x004365B0, CTCWife::IsMarriedToThePlayer 0x006CEC80 /
//   IsAttractedToHero 0x006CFE80, PeekGuiDef 0x00438570, CMiniMapDisplay::AddMarker 0x0082A500.
// Shape: the InitialiseTraderMarkers model (0x0082AE80): right-operand-first `current == region(...)`, the inline long map-index
//   getter, the if / else-if marker calls tail-merged by VC7.1 into one AddMarker call.
struct CTCBase;
struct CInterfaceMap {                             // a thing's sorted {type, component} vector
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0 (the const copy)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x8e - 0x4c];
    short         MapIndex;                        // +0x8e the world-map index the thing lives on
    long GetMapIndex() const { return MapIndex; }
    unsigned char m_flags90;                       // +0x90
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct list {                   // STLport list: the header node pointer
    _List_node<T>* _M_node;
};
struct CWorldMap {
    long GetRegionNumberMapIsIn(long mapIndex);    // 0x004FC190
    long GetCurrentRegionNumber();                 // 0x004FB150
};
struct SMarkerGraphics {                           // a marker's two graphic indices
    long Primary;
    long Secondary;
};
struct CArray_MarkerGraphics {                     // CArray<SMarkerGraphics>: begin, end, ...
    SMarkerGraphics* m_pBegin;
    SMarkerGraphics* m_pEnd;
    SMarkerGraphics& operator[](unsigned long i) { return m_pBegin[i]; }
};
struct CPlayerGuiDef {
    unsigned char _pad00[0x958];
    CArray_MarkerGraphics MiniMapMarkerGraphics;           // +0x958
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
struct CTCWife {                                   // interface 0xd7
    bool IsMarriedToThePlayer() const;             // 0x006CEC80
    bool IsAttractedToHero() const;                // 0x006CFE80
};
struct CThingLists {
    unsigned char _pad00[0x14];
    list<CThing*> Creatures;                       // +0x14
};
struct CThingManager {
    void*        __vftable;
    CThingLists* Lists;                            // +0x04
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CThingManager* GetThingManager();      // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                          // +0x24 scoped_ptr<CWorld>
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CRegion;
struct CMiniMapDisplay {
    virtual void v00();
    unsigned char _pad04[0xb4];                    // +0x04 .. +0xb8
    CRegion*      m_pRegion;                       // +0xb8 (retail)
    void AddMarker(const CThing* thing, long graphicIndex);   // 0x0082A500
    void InitialiseAttractionMarkers();
};

void CMiniMapDisplay::InitialiseAttractionMarkers()
{
    if (m_pRegion) {
        CWorld* world = g_pMainGameComponent->PWorld;
        CWorldMap* worldMap = world->GetWorldMap();
        list<CThing*>& creatures = world->GetThingManager()->Lists->Creatures;
        for (_List_node<CThing*>* node = creatures._M_node->_M_next; node != creatures._M_node; node = node->_M_next) {
            CThing* thing = node->_M_data;
            if (worldMap->GetCurrentRegionNumber() == worldMap->GetRegionNumberMapIsIn(thing->GetMapIndex()) && thing->HasTC(0xd7)) {
                const CTCWife* wife = thing->GetTC<CTCWife>(0xd7);
                if (wife->IsMarriedToThePlayer())
                    AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[7].Primary);
                else if (wife->IsAttractedToHero())
                    AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[7].Secondary);
            }
        }
    }
}
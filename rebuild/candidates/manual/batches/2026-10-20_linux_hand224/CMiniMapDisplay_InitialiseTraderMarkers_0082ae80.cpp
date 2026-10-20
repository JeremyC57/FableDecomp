// CMiniMapDisplay::InitialiseTraderMarkers() -- when a region is loaded, add a minimap marker (player GUI def marker graphics[9]) for every live creature in the current region that is a shopkeeper (interfaces 0x53 and 0x90) whose shop is open
// Leaf: InitialiseTraderMarkers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082AE80, 277 bytes (+ int3 padding).
// Callees: the game's world (0x013B86A0 +0x24) slots 4 (+0x10, thing manager) and 13 (+0x34, world map);
//   CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / GetCurrentRegionNumber 0x004FB150 (labelled CWorld::DrawGetEnvironment),
//   the const CVectorMap LowerBound 0x004365B0, CTCShopKeeper::GetShopTC 0x00798F50 (twice), CTCShop::IsShopKeeperAvailable
//   0x00773AA0, PeekGuiDef 0x00438570, CMiniMapDisplay::AddMarker 0x0082A500.
// Layout: the region word at +0xb8 (retail; see the landed UpdateMarkersInternal 0x0082A960), the thing lists' creature list
//   at +0x14, thing MapIndex (short) +0x8e, the GUI def's marker-graphics array at +0x958 (8-byte entries, [9].Primary).
// Shape: `current == GetRegionNumberMapIsIn(...)` -- VC7.1 evaluates the RIGHT operand of an int `==` first, so the region
//   lookup runs first and the compare is `cmp current, saved`; the map index goes through an inline long getter (a direct
//   short member argument gives `movsx ecx`); the def's array `this` (def + 0x958) stays in eax after the inlined operator[].
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
struct CTCShop {
    bool IsShopKeeperAvailable() const;            // 0x00773AA0
};
struct CTCShopKeeper {                             // interface 0x90
    CTCShop* GetShopTC() const;                    // 0x00798F50
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
    void InitialiseTraderMarkers();
};

void CMiniMapDisplay::InitialiseTraderMarkers()
{
    if (m_pRegion) {
        CWorld* world = g_pMainGameComponent->PWorld;
        CWorldMap* worldMap = world->GetWorldMap();
        list<CThing*>& creatures = world->GetThingManager()->Lists->Creatures;
        for (_List_node<CThing*>* node = creatures._M_node->_M_next; node != creatures._M_node; node = node->_M_next) {
            CThing* thing = node->_M_data;
            if (thing && !(thing->m_flags91 & 1)) {
                if (worldMap->GetCurrentRegionNumber() == worldMap->GetRegionNumberMapIsIn(thing->GetMapIndex())
                    && thing->HasTC(0x53) && thing->HasTC(0x90)) {
                    const CTCShopKeeper* keeper = thing->GetTC<CTCShopKeeper>(0x90);
                    if (keeper->GetShopTC() && keeper->GetShopTC()->IsShopKeeperAvailable())
                        AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[9].Primary);
                }
            }
        }
    }
}

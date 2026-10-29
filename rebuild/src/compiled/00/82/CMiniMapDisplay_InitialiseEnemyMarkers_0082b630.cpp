// CMiniMapDisplay::InitialiseEnemyMarkers() -- when a region is loaded and the hero is alive, add a minimap marker (graphics[8].Primary) for every creature in the current region that is the hero's enemy, is not a guild sparrow, is not underground and is not hidden from the minimap
// Leaf: InitialiseEnemyMarkers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082B630, 278 bytes (+ int3 padding).
// Callees: CPlayerManager::GetPlayer 0x00449970 / CPlayer::GetPControlledCreature 0x00487DC0 (the game 0x013B86A0 +0x1c), the world's
//   slots 4 (+0x10) and 13 (+0x34), CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / GetCurrentRegionNumber 0x004FB150,
//   AreThingsEnemies 0x0076D2F0 (__fastcall), CThing::GetDefName 0x004C7CC0 (CDefString by hidden pointer),
//   CDefString::operator!=(const char*) 0x005FA740 ("CREATURE_BIRD_GUILD_SPARROW"), IsUnderground 0x0085F120 (__fastcall),
//   PeekGuiDef 0x00438570, CMiniMapDisplay::AddMarker 0x0082A500.
// Shape: the InitialiseTraderMarkers model plus the hero guard; the CDefString temporary (a class with a user ctor, so it is
//   returned through a hidden pointer) is compared in place; bit 7 of the thing's +0x15d flags is a 1-bit bitfield
//   (`test al,al ; js`).
class CDefString { public:                                // a def name index
    long Index;
    CDefString();
    bool operator!=(const char* name) const;       // 0x005FA740
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // a thing's sorted {type, component} vector
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the const copy)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x8e - 0x4c];
    short         MapIndex;                        // +0x8e the world-map index the thing lives on
    long GetMapIndex() const { return MapIndex; }
    unsigned char m_flags90;                       // +0x90
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    unsigned char _pad92[0x15d - 0x92];
    unsigned char m_flags15d_0 : 7;                // +0x15d
    unsigned char HiddenFromMiniMap : 1;           // +0x15d bit 7
    CDefString GetDefName() const;                 // 0x004C7CC0
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (const T*)it->second;
    }
};
class CThingPlayerCreature : public CThing { };
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct list {                   // STLport list: the header node pointer
    _List_node<T>* _M_node;
};
class CWorldMap { public:
    long GetRegionNumberMapIsIn(long mapIndex);    // 0x004FC190
    long GetLoadedRegion() const;                 // 0x004FB150
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
class CPlayerGuiDef { public:
    unsigned char _pad00[0x958];
    CArray_MarkerGraphics MiniMapMarkerGraphics;           // +0x958
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b);   // 0x0076D2F0
bool __fastcall IsUnderground(const CThing* thing);                  // 0x0085F120
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPlayer();                          // 0x00449970
};
struct CThingLists {
    unsigned char _pad00[0x14];
    list<CThing*> Creatures;                       // +0x14
};
class CThingManager { public:
    void*        __vftable;
    CThingLists* Lists;                            // +0x04
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CThingManager* GetThingManager();      // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                // +0x1c scoped_ptr<CPlayerManager>
    unsigned char   _pad20[0x24 - 0x20];
    CWorld*         PWorld;                        // +0x24 scoped_ptr<CWorld>
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CRegion;
class CMiniMapDisplay { public:
    virtual void v00();
    unsigned char _pad04[0xb4];                    // +0x04 .. +0xb8
    CRegion*      m_pRegion;                       // +0xb8 (retail)
    void AddMarker(const CThing* thing, long graphicIndex);   // 0x0082A500
    void InitialiseEnemyMarkers();
};

void CMiniMapDisplay::InitialiseEnemyMarkers()
{
    if (m_pRegion) {
        CThing* hero = g_pMainGameComponent->PPlayerManager->GetPlayer()->GetPControlledCreature();
        if (hero && !(hero->m_flags91 & 1)) {
            CWorld* world = g_pMainGameComponent->PWorld;
            CWorldMap* worldMap = world->GetWorldMap();
            list<CThing*>& creatures = world->GetThingManager()->Lists->Creatures;
            for (_List_node<CThing*>* node = creatures._M_node->_M_next; node != creatures._M_node; node = node->_M_next) {
                CThing* thing = node->_M_data;
                if (worldMap->GetLoadedRegion() == worldMap->GetRegionNumberMapIsIn(thing->GetMapIndex())
                    && AreThingsEnemies(hero, thing)
                    && thing->GetDefName() != "CREATURE_BIRD_GUILD_SPARROW"
                    && !IsUnderground(thing)
                    && !thing->HiddenFromMiniMap)
                    AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[8].Primary);
            }
        }
    }
}
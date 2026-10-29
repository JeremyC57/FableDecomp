// CMiniMapDisplay::InitialiseInformationMarkers() -- with a region loaded and the main player's creature alive: look up the four body-guard def indices and the hero's followed component (0xbc); for every information thing in the current region that carries the information flag, add a marker: the body-guard graphic ([8].Secondary) when it is one of the hero's followers of a body-guard def, else the information graphic ([2].Secondary)
// Leaf: ?InitialiseInformationMarkers@CMiniMapDisplay
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A9B0, 529 bytes (`ret`).
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960, CPlayer::PeekPControlledCreature 0x00487DD0, CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 (static __fastcall) / GetDefGlobalIndexFromName 0x009AD410,
//   CVectorMap::LowerBound const 0x004365B0 (inline GetTC 0xbc), CWorld slots 13 (GetWorldMap) and 4 (GetThingManager),
//   CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / GetCurrentRegionNumber 0x004FB150, the thing's information-flag getter
//   0x008300D0, CTCFollowed::IsFollower 0x006D4D70, PeekGuiDef 0x00438570 (__fastcall), AddMarker 0x0082A500.
// Data: "CREATURE_BODY_GUARD" 0x012578FC, "_WEAK" 0x012578E0, "_GOOD" 0x012578C4, "_GOOD_WEAK" 0x012578A4; the main game
//   component 0x013B86A0 (+0x1c player manager, +0x24 world).
// Layout: as the landed sibling InitialiseTeleportMarkers 0x0082ABD0 (PRegion +0xb8; GUI def marker-graphics array +0x958,
//   8-byte entries); the thing's def index (unsigned short) +0x8c; the information list at thing-lists +0x14.
// Shape: the Teleport model; the followed component is HasTC + the const GetTC (0x4365B0 copy) assigned over a null default.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                    // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);            // 0x009AD410
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the const copy)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
class CTCFollowed { public:                               // interface 0xbc
    bool IsFollower(const CThing* thing) const;    // 0x006D4D70
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                       // +0x8c the thing's global def index
    short         MapIndex;                        // +0x8e the world-map index the thing lives on
    long GetMapIndex() const { return MapIndex; }
    unsigned char m_flags90;                       // +0x90
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    bool HasInformationFlag();                     // 0x008300D0
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
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
class CPlayerGuiDef { public:
    unsigned char _pad00[0x958];
    CArray_MarkerGraphics MiniMapMarkerGraphics;   // +0x958
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
struct CThingLists {
    unsigned char _pad00[0x14];
    list<CThing*> InformationThings;               // +0x14
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
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;         // 0x00487DD0
};
class CPlayerManager { public:
public:
    const CPlayer& GetMainPlayer() const;   // 0x00449960
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    const CPlayerManager* PPlayerManager;          // +0x1c
    unsigned char   _pad20[4];
    CWorld*         PWorld;                        // +0x24 scoped_ptr<CWorld>
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CRegion;
class CMiniMapDisplay { public:
    virtual void v00();
    unsigned char _pad04[0xb4];                    // +0x04 .. +0xb8
    CRegion*      m_pRegion;                       // +0xb8 (retail)
    void AddMarker(const CThing* thing, long graphicIndex);   // 0x0082A500
    void InitialiseInformationMarkers();
};

void CMiniMapDisplay::InitialiseInformationMarkers()
{
    if (m_pRegion) {
        const CThing* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer().PeekPControlledCreature();
        if (hero && !(hero->m_flags91 & 1)) {
            long bodyGuard = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD");
            long bodyGuardWeak = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_WEAK");
            long bodyGuardGood = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_GOOD");
            long bodyGuardGoodWeak = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_BODY_GUARD_GOOD_WEAK");
            const CTCFollowed* followed = 0;
            if (hero->HasTC(0xbc))
                followed = hero->GetTC<CTCFollowed>(0xbc);
            CWorld* world = g_pMainGameComponent->PWorld;
            CWorldMap* worldMap = world->GetWorldMap();
            list<CThing*>& things = world->GetThingManager()->Lists->InformationThings;
            for (_List_node<CThing*>* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
                CThing* thing = node->_M_data;
                if (worldMap->GetCurrentRegionNumber() == worldMap->GetRegionNumberMapIsIn(thing->GetMapIndex()) && thing->HasInformationFlag()) {
                    if (followed->IsFollower(thing) && (thing->DefIndex == bodyGuard || thing->DefIndex == bodyGuardWeak ||
                                                        thing->DefIndex == bodyGuardGood || thing->DefIndex == bodyGuardGoodWeak))
                        AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[8].Secondary);
                    else
                        AddMarker(thing, PeekGuiDef()->MiniMapMarkerGraphics[2].Secondary);
                }
            }
        }
    }
}
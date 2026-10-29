// CTCTeleporter::StartTeleporting(CThing* hero) -- resets the hero's map-screen flags; when no GUI block or world mode is active and the map teleport UI builds, and the hero is alive with health left, queues game event 0x1a (open the map screen, payload 8) for the main player; otherwise flags the map screen state back on
// Leaf: StartTeleporting
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DFF90, 314 bytes (`ret 4`, /GS cookie 0x0139C8A8).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline GetInterface, interface 0x1B =
//          CTCInventoryMap: bit 27 of the bitset word +0x20); CTCBase::PeekWorld() const 0x00686A80;
//          CTCInventoryMap::BuildMapTeleportUI(long, bool, bool*) 0x0060C900 (`ret 0Ch`; the third argument is an out
//          flag, the landed caller's `bool keepSelection` spelling is wrong); CMemoryBuffer::Copy 0x009F1760;
//          CTCBase::PeekPlayerManager() const 0x00686D50; CPlayerManager::GetMainPlayerNumber() 0x00449700;
//          CMainGameComponent::AddGameEvent(const CGameEvent&) 0x0041649C on g_pMainGameComponent 0x013B86A0;
//          __security_check_cookie 0x00BFE9F9. Globals: g_pPlayerGui 0x013B8790 (+0xfd, an unidentified blocking flag),
//          g_epsilon_0129ba3c (1e-4f).
// Layout (retail): CThing bitset +0x20, interface map +0x44, dead bit +0x91, CThingPhysical::Health +0xb4 (PDB +0xcc);
//          CTCInventoryMap +0x240/+0x241 (by order Ego_r.pdb InventoryScreen/OpenedViaGuildSeal +0x29c/+0x29d -- inferred);
//          CWorld mode word +0x104 (as the landed caller); CGameEvent as the landed OpenTrainingScreen 0x0077D4B0.
// Shape: the out-parameter GetInterface leaves the map pointer uninitialised when the hero has no map (retail reads the
//        frame word); the lookup key and the out flag are homed in the dead `hero` argument slot (hero stays in edi);
//        the event path returns with its own cookie check, every failure joins the trailing `= true` store.
class CThing;
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         Health;                       // +0xb4 (CThingPhysical)
    template<class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

class CTCInventoryMap { public:
    unsigned char _pad_0x00[0x240];
    bool          InventoryScreen;              // +0x240
    bool          OpenedViaGuildSeal;           // +0x241
    void BuildMapTeleportUI(long screen, bool forceRebuild, bool* built);   // 0x0060C900
};

class CPlayerGui { public:
    unsigned char _pad_0x00[0xfd];
    bool          m_blockingFD;                 // +0xfd
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
extern float g_epsilon_0129ba3c;                // 0x0129BA3C (1e-4f)

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                         // +0x00
    signed char   Player;                       // +0x04
    unsigned char Data[32];                     // +0x05
    unsigned char EndPos;                       // +0x25
    bool          Valid;                        // +0x26
    bool          Replacement;                  // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event); // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CWorld { public:
    unsigned char _pad[0x104];
    long          m_mode;                       // +0x104
};
class CPlayerManager { public:
    long GetMainPlayerNumber() const;                 // 0x00449700
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    unsigned char _pad0a[2];
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCTeleporter : public CTCBase { public:
    void StartTeleporting(CThing* hero);
};

void CTCTeleporter::StartTeleporting(CThing* hero)
{
    CTCInventoryMap* map;
    hero->GetInterface(&map, 0x1b);
    map->InventoryScreen = false;
    map->OpenedViaGuildSeal = false;
    if (!g_pPlayerGui->m_blockingFD && PeekWorld().m_mode == 0) {
        bool built = false;
        map->BuildMapTeleportUI(0, true, &built);
        if (built && !(hero->m_flags91 & 1) && hero->Health > g_epsilon_0129ba3c) {
            CGameEvent event(0x1a);
            event.AddULONG(8);
            event.Player = (signed char)PeekPlayerManager().GetMainPlayerNumber();
            g_pMainGameComponent->AddGameEvent(event);
            return;
        }
    }
    map->InventoryScreen = true;
}
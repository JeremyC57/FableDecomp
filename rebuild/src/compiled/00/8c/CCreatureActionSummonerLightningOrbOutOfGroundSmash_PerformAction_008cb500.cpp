// CCreatureAction_SummonerLightningOrbOutOfGroundSmash::PerformAction() -- remove creature mode 0x5a through the owner's mode manager when it has one, spawn THING_EXPLOSION_SUMMONER_LIGHTNING_GROUND_SMASH at the summoner and tell the explosion's driver to spare the summoner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB500, length 189 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, bit 17 of +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall),
// CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CCharString::~CCharString @ 0x0099EAE0, CThing::GetPosition
// @ 0x004C73D0, CreateThing(long, const C3DVector&) @ 0x0080C460 (`__fastcall`), the explosion driver's exclusion add
// @ 0x0080CBF0.
// Shape: the landed CCreatureAction_ScorpionTailPlunge::PerformAction (0x008C4810) without the explosion null test,
// preceded by the CHECKED out-parameter mode-manager lookup (component left in eax, `mov ecx,eax`); the lookup key,
// the def-name string and the unchecked driver out pointer share the one `push ecx` frame word.
struct C3DVector { float x, y, z; };
struct CThing;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CExplosionDriver {
    void AddExcludedThing(CThing* thing);   // 0x0080CBF0
};
struct CThing {
    unsigned char     _pad0[0x20];
    unsigned long     m_presentBits[9];     // +0x20 (byte +0x28 bit 1: the driver component is cached)
    CInterfaceMap     m_interfaces;         // +0x44
    unsigned char     _pad1[0x5c - 0x4c];
    CExplosionDriver* m_pDriver;            // +0x5c
    const C3DVector& GetPosition() const;   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    bool GetTCDriver(CExplosionDriver** out)
    {
        if (m_presentBits[2] & 2) {             // the driver interface bit (byte +0x28 bit 1)
            *out = m_pDriver;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position);   // 0x0080C460

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();                 // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);        // 0x009AD410
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};
struct CCreatureAction_SummonerLightningOrbOutOfGroundSmash : CCombatActionBase {
    void PerformAction();
};

void CCreatureAction_SummonerLightningOrbOutOfGroundSmash::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->RemoveMode(0x5a);
    long explosionIndex;
    {
        CCharString name("THING_EXPLOSION_SUMMONER_LIGHTNING_GROUND_SMASH", -1);
        explosionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* explosion = CreateThing(explosionIndex, PCreature.GetPItem()->GetPosition());
    CExplosionDriver* driver;
    explosion->GetTCDriver(&driver);
    driver->AddExcludedThing(PCreature.GetPItem());
}
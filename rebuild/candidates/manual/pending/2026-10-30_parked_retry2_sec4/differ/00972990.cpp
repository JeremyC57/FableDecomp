// CAIStateGroup_JackOfBladesStage1::OnActivate() -- register Jack as a combatant against the target, switch his hit locations to the plain one, kill the spot-light markers within 50 units, find and keep the shield object, copy the arena centre's position into the boss component, and reset the speech counter
// Grade: DIFFER(792v803) nd=89 (was 97) -- lever: the shield search block at function scope (its filter/area/out pointer no longer overlay the light-search block); tried: shield out pointer declared first (97), shield test outside the block (SRC_FAIL); remaining: frame 0x2c v 0x30 -- retail shares one word for the interface keys, the first def-index filter and the shield out-pointer (+0x20) and gives the second area its own words (+0x30)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972990, length 803 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020, GetPWorld @ 0x008FCFC0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CCombatManager::RegisterCombatant @ 0x006EDC50, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0,
// CTCHitLocations::GetHitLocation @ 0x00815820, CHitLocation::SetEnabled @ 0x00816390, CGameDefinitionManager::Get
// @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CThing::GetPosition @ 0x004C73D0, _ftol
// @ 0x00BFEA70, PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetAllThingsInRoughArea<CThingFilter_HasDefGlobalIndex>
// @ 0x0096BCB0 / GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex,CThing> @ 0x007FCEF0, CThing::Kill @ 0x004C9B80,
// CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, free @ 0x00BFEA14.
// Layout: Ego_r.pdb CAIStateGroup_JackOfBladesBase PArenaCentre +0x1c, PTargetThing +0x24, SpeechCounter +0x30 (retail
// 12-byte vectors), CAIStateGroup_JackOfBladesStage1 PShieldThing +0x5c; CTCBoss ArenaCentrePos +0x22.
#include <stdlib.h>
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool b);                          // 0x004C9B80
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing { };
struct CHitLocation {
    void SetEnabled(bool enabled);              // 0x00816390
};
struct CTCHitLocations {                        // interface 0x7f
    CHitLocation* GetHitLocation(const CCharString& name);   // 0x00815820
};
#pragma pack(push, 1)
struct CTCBoss {                                // interface 0xad
    unsigned char _pad0[0x22];
    C3DVector ArenaCentrePos;                   // +0x22
};
#pragma pack(pop)
struct CBaseIntelligentPointer {
    void* vftable;
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
extern void* const CIntelligentPointer_CThing_vftable;   // 0x01238C6C
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item) { *(void* const**)this = &CIntelligentPointer_CThing_vftable; SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& o) { SetPItem(o.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CThingList {                             // STLport vector<CThing*>, retail 12 bytes
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingList() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CThing* operator[](unsigned long i) const { return _M_start[i]; }
};
struct CRoughArea {
    long X0, Y0, X1, Y1;
    CRoughArea(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};
struct CThingFilter_HasDefGlobalIndex {
    long Index;
    CThingFilter_HasDefGlobalIndex(long index) : Index(index) { }
};
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const CRoughArea& area, const CThingFilter_HasDefGlobalIndex& filter, CThingList* out);   // 0x0096BCB0
    bool GetPFirstThingInRoughArea(const CRoughArea& area, const CThingFilter_HasDefGlobalIndex& filter, CThing** out);   // 0x007FCEF0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager {
    static CDefinitionManager* Get();           // 0x0044C6B0
};
struct CCombatManager {
    void RegisterCombatant(CThingCreatureBase* creature, CThing* enemy, bool b);   // 0x006EDC50
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_JackOfBladesStage1 : CAIStateGroupBase {
    CIntelligentPointerThing PSwordEmitter;     // +0x14
    CIntelligentPointerThing PArenaCentre;      // +0x1c
    CIntelligentPointerThing PTargetThing;      // +0x24
    long EnemyKnockDownFrame;                   // +0x2c
    long SpeechCounter;                         // +0x30
    unsigned char PillarMarkers[0x0c];          // +0x34
    unsigned char Pillars[0x0c];                // +0x40
    bool HasAttacked;                           // +0x4c
    long ZigZagCounter;                         // +0x50
    CIntelligentPointerThing PObstruction;      // +0x54
    CIntelligentPointerThing PShieldThing;      // +0x5c
    void OnActivate();
};

void CAIStateGroup_JackOfBladesStage1::OnActivate()
{
    CThingCreatureBase* creature = GetCreature();
    CTCHitLocations* hits;
    creature->GetInterface(&hits, 0x7f);
    CCombatManager* combat = GetPWorld()->CombatManager;
    combat->RegisterCombatant(creature, PTargetThing.GetPItem(), true);
    CHitLocation* location;
    {
        CCharString name("HIT_LOCATION_JACK_OF_BLADES_BIG");
        location = hits->GetHitLocation(name);
    }
    location->SetEnabled(false);
    {
        CCharString name("HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD");
        location = hits->GetHitLocation(name);
    }
    location->SetEnabled(false);
    {
        CCharString name("HIT_LOCATION_JACK_OF_BLADES");
        location = hits->GetHitLocation(name);
    }
    location->SetEnabled(true);
    CThingList lights;
    {
        long index;
        {
            CCharString name("MARKER_SPOT_LIGHT");
            index = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName(name);
        }
        CThingFilter_HasDefGlobalIndex filter(index);
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        CRoughArea area(x - 50, y - 50, x + 50, y + 50);
        PeekThingSearchTools()->GetAllThingsInRoughArea(area, filter, &lights);
    }
    unsigned long count = lights.size();
    for (unsigned long i = 0; i < count; ++i) {
        CThing* light = lights[i];
        if (light != 0 && !(light->m_flags91 & 1))
            light->Kill(true);
    }
    long index;
    {
        CCharString name("OBJECT_JACK_OF_BLADES_SHIELD");
        index = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName(name);
    }
    CThingFilter_HasDefGlobalIndex filter(index);
    const C3DVector& pos = GetCreature()->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    CRoughArea area(x - 50, y - 50, x + 50, y + 50);
    CThing* shield = 0;
    if (PeekThingSearchTools()->GetPFirstThingInRoughArea(area, filter, &shield))
        PShieldThing = CIntelligentPointerThing(shield);
    if (IS_THING_ALIVE(PArenaCentre)) {
        CTCBoss* boss;
        GetCreature()->GetInterface(&boss, 0xad);
        boss->ArenaCentrePos = PArenaCentre.GetPItem()->GetPosition();
    }
    SpeechCounter = 0;
}

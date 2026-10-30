// CTeamSpawn::CTeamSpawn(CQ_OrchardFarmRaidScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& thing) -- construct the team-spawn entity script: the parent's quest interface, its thing, parent, master data, an empty OtherSpawnPoint thing and the two empty team-member strings
// Leaf: ??0CTeamSpawn
// Retail: Fable.exe @ 0x00DCD160, 96 bytes. Vtables 0x012C3224 (the entity-script base, as in the landed
// CPermanentBarrier constructor 0x00D2D7B0) and 0x012DB2D4 (this class, no RTTI); 0x01238C8C = CScriptThing.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBaseClass::CBaseClass() @ 0x0099A2D0 (the inline
// CScriptThing default constructor's base), CCharString::CCharString() @ 0x0099E4B0 (twice).
// Layout: PDB CTeamSpawn (0x40 bytes): ParentClass +0x14, PMasterData +0x18, OtherSpawnPoint (CScriptThing) +0x1c,
// TeamID / TeamMemberLimit / TeamRespawnTime +0x28..+0x30 (left uninitialised), TeamMemberName +0x34,
// TeamMemberDefName +0x38, BanditsLeftID +0x3c. CScriptThing = CBase + the CCountedPointer PImp (+4/+8).
// Shape: the CPermanentBarrier shape (root interface pointer before the dead root vtable, the thing copy after the base
// vtable, ParentClass in the init list before the derived vtable store, PMasterData a body assignment stored last); the
// default-constructed OtherSpawnPoint is CBase() + its vtable + the PImp member's inline zeroing (after the vtable store).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

template <class T> class CCountedPointer { public:                       // CCountedPointer<CScriptThing>: object + count block
    void* m_pObject;                               // +0x00
    void* m_pCount;                                // +0x04
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};

struct CGameScriptInterface_Calls;

struct CQuestScriptBase_Retail {                   // the parent quest script: interface pointer at +0x40
    unsigned char               _pad[0x40];
    CGameScriptInterface_Calls* GSI;               // +0x40
};

struct CQ_OrchardFarmRaidScript : CQuestScriptBase_Retail { };
struct CQ_SunnyvaleMasterData;

struct CEntityScriptRoot {                         // root: its vtable store is dead
    virtual void v00();
    CGameScriptInterface_Calls* PQuest;            // +0x04
    CEntityScriptRoot(CGameScriptInterface_Calls* quest) : PQuest(quest) { }
};

class CEntityScriptBase : public CEntityScriptRoot { public:     // vtable 0x012C3224
    virtual void v00();
    CScriptThing Thing;                            // +0x08
    CEntityScriptBase(CQuestScriptBase_Retail* parent, const CScriptThing& thing)
        : CEntityScriptRoot(parent->GSI), Thing(thing) { }
};

struct CTeamSpawn : CEntityScriptBase {            // vtable 0x012DB2D4, 0x40 bytes
    virtual void v00();
    CQ_OrchardFarmRaidScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
    CScriptThing              OtherSpawnPoint;     // +0x1c
    long                      TeamID;              // +0x28
    long                      TeamMemberLimit;     // +0x2c
    long                      TeamRespawnTime;     // +0x30
    CCharString               TeamMemberName;      // +0x34
    CCharString               TeamMemberDefName;   // +0x38
    long                      BanditsLeftID;       // +0x3c
    CTeamSpawn(CQ_OrchardFarmRaidScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& thing);
};

CTeamSpawn::CTeamSpawn(CQ_OrchardFarmRaidScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& thing)
    : CEntityScriptBase(parent, thing), ParentClass(parent)
{
    PMasterData = masterData;
}
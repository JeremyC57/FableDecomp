// CCrateTeamManager::CCrateTeamManager() -- a crate-team manager: timer from the thing manager,
// two null script things, then the six state counters and the member count cleared in the body
// (MaxTeamMembers and EnemyTeam are left uninitialised)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDB360, length 94 bytes (+ int3 padding).
// Callees: the thing manager global @ 0x0143E8F8 vtable slot 87 (+0x15c, long: a new timer
// index), CBaseClass::CBaseClass @ 0x0099A2D0 (twice, the base of each CScriptThing member).
// PDB CCrateTeamManager (64 bytes): long StateCounter[6] +0x00, long MemberCount +0x18,
// CTimer TeamReinforcementsTimer +0x1c, long MaxTeamMembers +0x20, CScriptThing CrateDropPos +0x24,
// CScriptThing TeamCrateCarrier +0x30, CCrateTeamManager* EnemyTeam +0x3c.
// Shape: the timer member is constructed first (its value comes from the virtual call), then
// each CScriptThing inline: CBase(), the vtable 0x01238C8C (CScriptThing, rtti-vtables.tsv) and
// the two null counted-pointer words; the seven zero stores that follow the member constructors
// are constructor-body assignments (an init-list zero would be stored before the timer call and
// would claim the zero register first). The `mov [edi],vtable` after a `lea edi` is the member
// address kept in a register.
#include "engine/CScriptThing.h"   // PDB layout cited: CScriptThing = {vptr, PImp_Data, PImp_Info}, 12 bytes (modelled below with its constructor)

struct CThingManager_Calls {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086();
    virtual long CreateTimer();                // slot 87 (+0x15c)
};
extern CThingManager_Calls* g_pThingManager_0143e8f8;   // 0x0143E8F8

class CBaseClass {
public:
    virtual void v00();
    CBaseClass();                                   // 0x0099A2D0
};

struct CCountedPointer_Retail {               // CCountedPointer<CScriptThing>: Data / Info, both null
    void* Data;
    void* Info;
    CCountedPointer_Retail() : Data(0), Info(0) { }
};

struct CScriptThing_Retail : CBaseClass {           // vtable 0x01238C8C; layout as engine/CScriptThing.h
    virtual void v00();
    CCountedPointer_Retail PImp;               // +0x04 (Data +0x04, Info +0x08)
    CScriptThing_Retail() : CBaseClass(), PImp() { }
};

class CTimer { public:
    long TimerIndex;
    CTimer() : TimerIndex(g_pThingManager_0143e8f8->CreateTimer()) { }
};

struct CCrateTeamManager {
    long               StateCounter[6];        // +0x00
    long               MemberCount;            // +0x18
    CTimer             TeamReinforcementsTimer;    // +0x1c
    long               MaxTeamMembers;         // +0x20
    CScriptThing_Retail CrateDropPos;          // +0x24
    CScriptThing_Retail TeamCrateCarrier;      // +0x30
    CCrateTeamManager* EnemyTeam;              // +0x3c
    CCrateTeamManager();
};

CCrateTeamManager::CCrateTeamManager()
    : TeamReinforcementsTimer(), CrateDropPos(), TeamCrateCarrier()
{
    for (long i = 0; i < 6; ++i)
        StateCounter[i] = 0;
    MemberCount = 0;
}
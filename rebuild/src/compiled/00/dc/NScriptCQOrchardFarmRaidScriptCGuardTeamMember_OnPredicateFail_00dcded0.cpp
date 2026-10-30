// NScript::CQ_OrchardFarmRaidScript::CGuardTeamMember::OnPredicateFail() -- take this member off its team (member count and the counter of its state), and when it is killed record a bandit (team 1) or a guard kill in the master data
// Leaf: ?OnPredicateFail@CGuardTeamMember@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCDED0, 137 bytes, thiscall, plain `ret`.
// Identity (probable): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012DB364, which the landed entity Alloc 0x00DCDE20 (`NScript::CQ_OrchardFarmRaidScript::CGuardTeamMember::Alloc`, the "GuardTeamMember" binding) stores. Neither PDB has a CGuardTeamMember (the class name is the landed Alloc's, from the binding string); Ego_r.pdb's same-sized CCrateTeamMember (60 bytes, TeamID +0x24, MemberState +0x28, OnPredicateFail) is the debug-build twin whose member names are used here.
// Label: the manifest's "sub_00dcded0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptThing::MsgIsKilledBy (slot 18) with the pooled empty string 0x0122D70E.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent Teams (CCrateTeamManager[2], 0x40 each) +0x8c (Ego_r.pdb +0xa4: -0x14, -4 for the STLport CrateList vector); CQ_SunnyvaleMasterData OrchardFarmBanditKilled +0x118, OrchardFarmGuardKilled +0x119.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb offsets (same in retail)
public:
    unsigned char _pad000[0x118];
    bool OrchardFarmBanditKilled;                       // +0x118
    bool OrchardFarmGuardKilled;                        // +0x119
};
class CQ_OrchardFarmRaidScript {
public:
    class CCrateTeamManager {                           // Ego_r.pdb ...@CCrateTeamManager@CQ_OrchardFarmRaidScript@NScript@@, 0x40 bytes
    public:
        long StateCounter[6];                           // +0x00
        long MemberCount;                               // +0x18
        CTimer TeamReinforcementsTimer;                 // +0x1c
        long MaxTeamMembers;                            // +0x20
        CScriptThing CrateDropPos;                      // +0x24
        CScriptThing TeamCrateCarrier;                  // +0x30
        CCrateTeamManager* EnemyTeam;                   // +0x3c
    };
    unsigned char _pad000[0x8c];
    CCrateTeamManager Teams[2];                         // +0x8c (Ego_r.pdb +0xa4)
    class CGuardTeamMember;
};
class CQ_OrchardFarmRaidScript::CGuardTeamMember : public CEntityScriptBase {
public:
    CQ_OrchardFarmRaidScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      CurrentAIState;             // +0x1c (CCrateTeamMember names, descriptive)
    long                      PreviousAIState;            // +0x20
    long                      TeamID;                     // +0x24
    long                      MemberState;                // +0x28
    CQ_OrchardFarmRaidScript::CCrateTeamManager* MyTeam;  // +0x2c
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_OrchardFarmRaidScript::CGuardTeamMember::OnPredicateFail()
{
    ParentClass->Teams[TeamID].MemberCount--;
    ParentClass->Teams[TeamID].StateCounter[MemberState]--;
    if (Me.MsgIsKilledBy("")) {
        if (TeamID == 1)
            PMasterData->OrchardFarmBanditKilled = true;
        else
            PMasterData->OrchardFarmGuardKilled = true;
    }
}
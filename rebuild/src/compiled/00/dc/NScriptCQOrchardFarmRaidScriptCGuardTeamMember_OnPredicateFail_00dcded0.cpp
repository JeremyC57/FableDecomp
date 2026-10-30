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
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv / Ego_r.pdb)
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5, 7..17, 19 simplified: not called here)
    virtual const C3DVector& GetPos() const;              // slot 6 (+0x18)
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10
    virtual void GetPThing() const;                       // slot 11
    virtual void GetPThingUniqueID() const;               // slot 12
    virtual void GetCurrentStateGroupType() const;        // slot 13
    virtual void GetCurrentScriptPriority() const;        // slot 14
    virtual void IsSneaking() const;                      // slot 15
    virtual void IsAwareOfHero() const;                   // slot 16
    virtual void MsgIsTriggeredBy() const;                // slot 17
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    virtual void t019() const;                            // slot 19
    virtual bool MsgIsUsedBy(const CCharString& user) const;       // slot 20 (+0x50)
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
};
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes
public:
    virtual ~CEntityScriptBase();
private:
    virtual void OnPredicateFail();                 // slot 5 (+0x14)
public:
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
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
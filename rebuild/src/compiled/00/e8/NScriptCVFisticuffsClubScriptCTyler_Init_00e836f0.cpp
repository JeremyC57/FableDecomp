// NScript::CV_FisticuffsClubScript::CTyler::Init() -- Tyler of the fisticuffs club: conversation flags cleared, unpushable, neutral faction, no money, no reason for leaving and no outstanding crimes
// Leaf: ?Init@CTyler@CV_FisticuffsClubScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E836F0, 131 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012EB0C0, which the landed
//   NScript::CV_FisticuffsClubScript::CTyler::Alloc 0x00E83570 stores (site 0x00E835A5); Ego_r.pdb has ?Init@CTyler@CV_FisticuffsClubScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CTyler (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 844 SetIsPushableByHero (Me by value), 601 EntitySetInFaction ("FACTION_NEUTRAL" temporary);
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, operator=(const char*) 0x0099EFE0 ("NONE").
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FisticuffsClubScript {
public:
    class CTyler;
};
class CV_FisticuffsClubScript::CTyler : public CEntityScriptBase {
public:
    CV_FisticuffsClubScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    bool AnsweredYes;                                   // +0x1c
    bool ShownItem;                                     // +0x1d
    bool IntroducedPunchClub;                           // +0x1e
    long Money;                                         // +0x20
    CCharString PunchClubLeftReason;                    // +0x24
    bool OutstandingCrimes;                             // +0x28
private:
    virtual void Init();
};

void CV_FisticuffsClubScript::CTyler::Init()
{
    AnsweredYes = false;
    ShownItem = false;
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_NEUTRAL");
    IntroducedPunchClub = false;
    Money = 0;
    PunchClubLeftReason = "NONE";
    OutstandingCrimes = false;
}
}
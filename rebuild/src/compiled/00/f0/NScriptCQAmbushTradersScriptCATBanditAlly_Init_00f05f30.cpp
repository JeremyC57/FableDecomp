// NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Init() -- the ambush-traders bandit ally: make Me an ally of the hero, allowed to follow him and carrying information, apply the parent's bandit allegiances, reset the invalid-region timer and the hire/follow/teleport flags, disable the SG_MINION_SIT_OUTSIDE state group and spawn the parent's WatchForHeroAttackOnBandit thread on Me
// Leaf: ?Init@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F05F30, 409 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the entity-script vtable 0x012F7D88 that the landed CAT_BanditAlly::Alloc 0x00F037A0
//   stores [dtor 0x00F03870, Main 0x00F063B0, Init 0x00F05F30, 0x00F03840, OnPersist 0x00F0A8E0 (BanditHired /
//   BanditFollowing keys), OnPredicateFail 0x00F03850 (landed CAT_BanditAlly::OnPredicateFail)]; the row is referenced only
//   from that slot. Ego_r.pdb ?Init@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXXZ; the members written are the Ego_r.pdb
//   CAT_BanditAlly ones at their PDB offsets. Catalog label: sub_00f05f30.
// Callees: CQ_AmbushTradersScript::SetBanditAllegiences(const CScriptThing&) 0x00F060D0 (landed), CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator+ 0x0099F570, operator new 0x00BFEA1A, CSpawnedFuncBase ctor 0x00CDD450,
//   CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CScriptBase::AddSpawnedFunction 0x00CB7E50; CSpawnedFunc1<
//   CQ_AmbushTradersScript, CScriptThing> vtable 0x012F7EBC, CQ_AmbushTradersScript::WatchForHeroAttackOnBandit 0x00F061E0
//   (landed); CTimer::operator=(long) inline: g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer; Game slots 70 GetHero,
//   235 EntitySetAsAllowedToFollowHero, 360 SetThingHasInformation, 590 EntitySetStategroupEnabled, 599
//   EntitySetThingAsAllyOfThing (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "SG_MINION_SIT_OUTSIDE" 0x012891B8, "WatchForHeroAttackOnBandit" 0x012F7ED8, "ParentClass." 0x012C9934, "" 0x0122D70E.
// Layout: Ego_r.pdb CAT_BanditAlly (0x2C, the retail new size): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, BanditHired +0x1c, BanditFollowing +0x1d, BanditTeleported +0x1e, BanditSwitchedAllegience +0x1f,
//   BanditRemovedFromBowerstonePosh +0x20, ResetInvalidRegionTimer +0x21, BanditHealthBarID +0x24, InvalidRegionTimer +0x28.
//   CSpawnedFuncBase (0x34) is modelled locally as in the landed CAT_Sentry::Init 0x00F08E90 (engine/CSpawnedFuncBase.h
//   flattens the vptr into a data member and has no constructor).
// Shape: the landed CAT_Sentry::Init 0x00F08E90 spawn block (real CSpawnedFunc1 template, __forceinline ctor, inline copy of
//   Me into the by-value argument, out-of-line ~CScriptThing); the conditional-temporary flags are homed in the frame here.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
void* __cdecl operator new(unsigned int bytes);        // 0x00BFEA1A (MSVCR71)
class CCPPointerInfo { public: long References; void (__fastcall* DeleteFunc)(void* data); void* Data; };
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
// CScriptThing stays local in this row (wave 1 residue): retail expands the copy 0x004ABE90 inline at the by-value argument
// but calls it for the CSpawnedFunc1 member copy; the shared engine/CScriptThing.h model with an inline copy expands both.
class CScriptThing : public CBaseClass { public:                     // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThing>     PImp;            // +0x04
    struct InlineCopy { };                             // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other);           // 0x004ABE90 (the out-of-line copy)
    __forceinline CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing();                                   // 0x004AA840
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"               // CGameScriptInterfaceBase (917 slots) + vtable 0x01260F0C
#include "engine/CTimer.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// Retail expands CTimer::operator=(long) inline (Ego_r ??4CTimer@@QAEAAV0@J@Z): slot 89 SetTimer through 0x0143E8F8.
__forceinline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
// Layout: the retail polymorphic suspendable-process chain from the shared headers (FABLE_RETAIL_SUSPENDABLE_PROCESS
//   opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess : CBaseClassNonCopyable : CBaseClass,
//   0x34 bytes, vtable 0x012C2F9C).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class TClass, class TPar1> class CSpawnedFunc1 : public CSpawnedFuncBase { public:   // 0x48 bytes
    void (TClass::*Func)(TPar1);                       // +0x34
    TClass* PClass;                                    // +0x38
    TPar1   Par1;                                      // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, TClass* pclass, void (TClass::*func)(TPar1), TPar1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
    virtual void SuspendableProcess();
};
#include "engine/CScriptBase.h"                        // retail 0x40, vtable 0x012C1648
// CEntityScriptBase stays local with CScriptThing (engine/CEntityScriptBase.h includes engine/CScriptThing.h); retail
//   vtable 0x012C3224, 7 slots, the only one this row needs being the dtor.
class CEntityScriptBase { public:                     // retail 0x14 bytes
    virtual ~CEntityScriptBase();                      // slot 0
    CGameScriptInterfaceBase* Game;                    // +0x04
    CScriptThing          Me;                          // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CScriptBase { public:
    void WatchForHeroAttackOnBandit(CScriptThing thing);   // 0x00F061E0
    void SetBanditAllegiences(const CScriptThing& thing);  // 0x00F060D0
    class CAT_BanditAlly;
};
class CQ_AmbushTradersScript::CAT_BanditAlly : public CEntityScriptBase {
public:
    CQ_AmbushTradersScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    bool                    BanditHired;               // +0x1c
    bool                    BanditFollowing;           // +0x1d
    bool                    BanditTeleported;          // +0x1e
    bool                    BanditSwitchedAllegience;  // +0x1f
    bool                    BanditRemovedFromBowerstonePosh;   // +0x20
    bool                    ResetInvalidRegionTimer;   // +0x21
    long                    BanditHealthBarID;         // +0x24
    CTimer                  InvalidRegionTimer;        // +0x28
private:
    virtual void Main();                               // slot 1 0x00F063B0
    virtual void Init();                               // slot 2 0x00F05F30 (this row)
};
}

void NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Init()
{
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    Game->SetThingHasInformation(Me, true, false, false);
    ParentClass->SetBanditAllegiences(Me);
    InvalidRegionTimer = 0;
    ResetInvalidRegionTimer = true;
    BanditHired = false;
    BanditFollowing = false;
    BanditTeleported = false;
    BanditSwitchedAllegience = false;
    BanditRemovedFromBowerstonePosh = false;
    Game->EntitySetStategroupEnabled(Me, "SG_MINION_SIT_OUTSIDE", false);
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("WatchForHeroAttackOnBandit"), 0, ParentClass,
            &CQ_AmbushTradersScript::WatchForHeroAttackOnBandit, CScriptThing(Me, CScriptThing::InlineCopy())),
        CCharString(""));
}
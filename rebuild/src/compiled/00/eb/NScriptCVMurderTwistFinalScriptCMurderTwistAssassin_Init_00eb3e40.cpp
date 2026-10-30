// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Init() -- the assassin: reset the threat state (threat distance from the script def); removed when the sheriff's state is 0, otherwise made unpushable, unkillable and undamageable, looking at the hero, informative, friends with everything and persistent
// Leaf: ?Init@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3E40, 247 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F00BC, which the landed
//   NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Alloc 0x00EB3C60 stores (site 0x00EB3C95); Ego_r.pdb has ?Init@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMurderTwistAssassin (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 108 RemoveThing, 745 SetIsThingForcePushable, 844 SetIsPushableByHero (Me by value), 529
//   EntitySetAsUseMovementInActions, 517 EntitySetAsKillable, 516 EntitySetAsDamageable, 70 GetHero, 498
//   EntityForceToLookAtThing, 360 SetThingHasInformation, 477 SetThingPersistent; CScriptThing slot 67
//   SetFriendsWithEverythingFlag. Data: pGlobalScriptDef 0x0143E90C, AssassinThreatDistSq at retail +0x720.
class CScriptDef {                                      // Ego_r.pdb CScriptDef; retail offsets (agent501: HeroKillsSheriffRenown +0x71c)
public:
    unsigned char _pad0[0x720];
    float AssassinThreatDistSq;                         // +0x720 (Ego_r +0x77c)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
enum ESheriffState { };
enum EAssassinState { };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0, implicit copy
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
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08();
    virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16();
    virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23(); virtual void t24();
    virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30(); virtual void t31(); virtual void t32();
    virtual void t33(); virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39(); virtual void t40();
    virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46(); virtual void t47(); virtual void t48();
    virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54(); virtual void t55(); virtual void t56();
    virtual void t57(); virtual void t58(); virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62(); virtual void t63(); virtual void t64();
    virtual void t65(); virtual void t66();
    virtual void SetFriendsWithEverythingFlag(bool flag);   // slot 67 (+0x10c)
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CScriptGameResourceObjectScriptedThingBase {      // Ego_r.pdb, 16 bytes: vptr, base word, PImp
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    long _base;                                         // +0x04 (unnamed in the PDB layout)
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
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
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    ESheriffState SheriffState;                         // +0x48
    EAssassinState AssassinState;                       // +0x4c
    bool TaskDeclined;                                  // +0x50
    bool PreemptiveAttackInProgress;                    // +0x51
    CScriptThing Sheriff;                               // +0x54
    std::vector<CScriptThing> Guards;                   // +0x60
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;   // +0x6c
    std::vector<CScriptGameResourceObjectScriptedThingBase> seh_Guards;   // +0x7c
    class CMurderTwistAssassin;
};
class CV_MurderTwistFinalScript::CMurderTwistAssassin : public CEntityScriptBase {
public:
    CV_MurderTwistFinalScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    unsigned long NextLine;                             // +0x2c
    bool HasBeenThreatened;                             // +0x30
    float ThreatDistSq;                                 // +0x34
    bool HasGivenIntroToHero;                           // +0x38
    bool PreemptiveAttackInProgress;                    // +0x39
private:
    virtual void Init();
};

void CV_MurderTwistFinalScript::CMurderTwistAssassin::Init()
{
    HasBeenThreatened = false;
    ThreatDistSq = pGlobalScriptDef->AssassinThreatDistSq;
    NextLine = 1;
    HasGivenIntroToHero = false;
    PreemptiveAttackInProgress = false;
    if (ParentClass->SheriffState == 0) {
        Game->RemoveThing(Me, false, true);
        return;
    }
    Game->SetIsThingForcePushable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsUseMovementInActions(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntityForceToLookAtThing(Me, Game->GetHero());
    Game->SetThingHasInformation(Me, false, true, false);
    Me.SetFriendsWithEverythingFlag(true);
    Game->SetThingPersistent(Me, true);
}
}
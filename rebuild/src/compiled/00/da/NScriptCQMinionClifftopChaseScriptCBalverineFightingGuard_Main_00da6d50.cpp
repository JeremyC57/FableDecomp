// NScript::CQ_MinionClifftopChaseScript::CBalverineFightingGuard::Main() -- the guard fighting the balverines: register the alive filter, find the nearest MinionAttackingGuard, set the two fighting each other, then (once) wait for the quest start screen, show the guard's health bar, wait for the hero to come close, speak, wait for the attacker to die, reward morality, thank the hero and follow him; finally spawn the parent's GuardChatter on Me, wait for the mission to end and remove Me
// Leaf: ?Main@CBalverineFightingGuard@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members References->RefCount are the header CCPPointerInfo's (same offsets).
// Naming wave 29 (family sfbh): engine/CSpawnedFuncBase.h now carries retail's implicit destructor and the real CCharString /
//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: a declaration-only CCountedPointer<T> declared before it (the row keeps its own CCPPointerInfo).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA6D50, length 1168 bytes, plain `ret`.
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (out of line in the filter and in the spawned functor's member),
//   ~CScriptThing @ 0x004AA840, CScriptThing() @ 0x006E7B40, the filter registration @ 0x00F35B10,
//   IsActiveThreadTerminating @ 0x00F35B30, IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), CCharString(const
//   char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, operator+(const CCharString&, const CCharString&) @ 0x0099F570
//   (`__fastcall`, hidden result), operator new @ 0x00BFEA1A, CSpawnedFuncBase ctor @ 0x00CDD450 (CSpawnedFunc1 vtable
//   0x012D771C), CScriptBase::AddSpawnedFunction @ 0x00CB7E50, CQ_MinionClifftopChaseScript::GuardChatter @ 0x00DA71E0.
//   CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 77 GetNearestWithScriptName, 108 RemoveThing, 156
//   GiveHeroMorality, 229 EntityFollowThing, 321 DisplayQuestInfo, 325 AddQuestInfoBarHealth, 364 AddNewConversation,
//   366 AddLineToConversation, 451 GiveThingBestEnemyTarget, 480 SetEntityAsRegionFollowing; CScriptThing slot 75 IsAlive.
// Literals: "MinionAttackingGuard" 0x012D7274, "HUD_QUEST_ICON_GUARD" 0x012D2B54, "TEXT_QST_045_GUARD_SO_POWERFUL"
//   0x012D7764, "TEXT_QST_045_GUARD_THANKS" 0x012D7748, "ParentClass." 0x012C9934, "GuardChatter" 0x012D7738, ""
//   0x0122D70E. Globals: pGlobalScriptDef 0x0143E90C (+0xd44 the guard talk distance, +0xd50 the rescue morality).
// Layout: Ego_r.pdb CBalverineFightingGuard (36 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, ExplainedAboutSummoning +0x1c, HealthBarID +0x20. CQ_MinionClifftopChaseScript is retail = PDB -
//   0x14: MissionFailed +0x49, MissionSucceeded +0x4a, QuestStartScreened +0x4b.
// Shape: the landed entity-script filter idiom; the spawned functor (0x48 bytes) takes Me by value: the argument copy
//   is the inline CScriptThing copy, the member copy inside the inlined functor ctor is the out-of-line 0x004ABE90 (inline
//   depth 1 around the statement); the new-expression's name temporaries are conditional (flags 7).
// Naming wave 35: identity reg 00da368c "BalverineFightingGuard" in 00da34b0; Alloc 00da4b80; vtable 012d7490 slot 1; PDB ?Main@CBalverineFightingGuard@CQ_MinionClifftopChaseScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CBalverineFightingGuard::Main (wave3/labels_alloc.tsv); PDB ?Main@CBalverineFightingGuard@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CBalverineFightingGuard@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CQ_MinionClifftopChaseScript moved into NScript); same bytes.
#include "fable_compiler.h"
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
class CRGBColour { public:
    unsigned char B, G, R, A;
    FABLE_FORCEINLINE CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
#include "engine/CCountedPointer.h"
struct CCPPointerImp {                                 // CCountedPointer<CScriptThingImp>: Data, Info
    void*           Data;
    CCPPointerInfo* Info;
    FABLE_FORCEINLINE CCPPointerImp(const CCPPointerImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                           // slot 0, 0x004AA840
    VPAD64(ta) VPAD8(tb) VPAD2(tc)                     // slots 1..74
    virtual bool IsAlive() const;                      // slot 75 (+0x12c)
    CCPPointerImp PImp;                                // +0x04
    CScriptThing();                                    // 0x006E7B40
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    FABLE_FORCEINLINE CScriptThing(CScriptThing& other) : PImp(other.PImp) { }   // the inline copy of a non-const thing
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD2(qe)                              // slots 8..69
    virtual const CScriptThing& GetHero() const;                                     // slot 70 (+0x118)
    VPAD4(ra) VPAD2(rb)                                                              // slots 71..76
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const;   // slot 77 (+0x134)
    VPAD16(sa) VPAD8(sb) VPAD4(sc) VPAD2(sd)                                         // slots 78..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;       // slot 108 (+0x1b0)
    VPAD32(ta) VPAD8(tb) VPAD4(tc) VPAD2(td) VPAD1(te)                               // slots 109..155
    virtual void GiveHeroMorality(long amount) const;                                // slot 156 (+0x270)
    VPAD64(ua) VPAD8(ub)                                                             // slots 157..228
    virtual void EntityFollowThing(const CScriptThing& thing, const CScriptThing& target, float distance, bool a) const;   // slot 229 (+0x394)
    VPAD64(va) VPAD16(vb) VPAD8(vc) VPAD2(vd) VPAD1(ve)                              // slots 230..320
    virtual void DisplayQuestInfo(bool show) const;                                  // slot 321 (+0x504)
    VPAD2(wa) VPAD1(wb)                                                              // slots 322..324
    virtual long AddQuestInfoBarHealth(const CScriptThing& thing, const CRGBColour& colour, const CCharString& icon, float a) const;   // slot 325 (+0x514)
    VPAD32(xa) VPAD4(xb) VPAD2(xc)                                                   // slots 326..363
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b) const;   // slot 364 (+0x5b0)
    VPAD1(ya)                                                                        // slot 365
    virtual void AddLineToConversation(long conversation, const CCharString& line, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
    VPAD64(za) VPAD16(zb) VPAD4(zc)                                                  // slots 367..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
    VPAD16(wa2) VPAD8(wb2) VPAD4(wc2)                                                // slots 452..479
    virtual void SetEntityAsRegionFollowing(const CScriptThing& thing, const CScriptThing& leader, bool follow) const;   // slot 480 (+0x780)
};
class CScriptDef { public:
    unsigned char _pad[0xd44];
    long          GuardTalkDistance;                   // +0xd44
    unsigned char _pad2[0xd50 - 0xd48];
    long          GuardRescueMorality;                 // +0xd50
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T, class P1> struct CSpawnedFunc1 : CSpawnedFuncBase {   // 0x48 bytes
    void (T::*Func)(P1);                               // +0x34
    T* PClass;                                         // +0x38
    P1 Par1;                                           // +0x3c
    FABLE_FORCEINLINE CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), const P1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) {}
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_MinionClifftopChaseScript : public CScriptBase {
public:
    unsigned char _pad40[0x49 - 0x40];
    bool MissionFailed;                                // +0x49
    bool MissionSucceeded;                             // +0x4a
    bool QuestStartScreened;                           // +0x4b
    void GuardChatter(CScriptThing guard);             // 0x00DA71E0
public:
    class CBalverineFightingGuard;   // the entity class below (naming wave 35)
};
}
using NScript::CQ_MinionClifftopChaseScript;
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_MinionClifftopChaseScript::CBalverineFightingGuard : public CEntityScriptBase {
public:
        CGameScriptInterface*         Game;                // +0x04
    CScriptThing                  Me;                  // +0x08
    CQ_MinionClifftopChaseScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;         // +0x18
    bool                          ExplainedAboutSummoning;   // +0x1c
    long                          HealthBarID;         // +0x20
private:
    virtual void Main();                          // PDB ?Main@CBalverineFightingGuard@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ
};
}

void NScript::CQ_MinionClifftopChaseScript::CBalverineFightingGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing attacker = Game->GetNearestWithScriptName(Game->GetHero(), "MinionAttackingGuard");
    if (!ExplainedAboutSummoning) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, attacker);
        Game->GiveThingBestEnemyTarget(attacker, Me);
        while (!ParentClass->QuestStartScreened) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->DisplayQuestInfo(true);
        HealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_GUARD", 1.0f);
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->GuardTalkDistance)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_045_GUARD_SO_POWERFUL", false, Me, CScriptThing());
        ExplainedAboutSummoning = true;
        while (reinterpret_cast<const CScriptThingView&>(attacker).Thing().IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroMorality(pGlobalScriptDef->GuardRescueMorality);
        conversation = Game->AddNewConversation(Me, false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_045_GUARD_THANKS", false, Me, CScriptThing());
        Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
        Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    }
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQ_MinionClifftopChaseScript, CScriptThing>(CCharString("ParentClass.") + CCharString("GuardChatter"), 0, ParentClass, &CQ_MinionClifftopChaseScript::GuardChatter, Me), CCharString(""));
    while (!ParentClass->MissionFailed && !ParentClass->MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}
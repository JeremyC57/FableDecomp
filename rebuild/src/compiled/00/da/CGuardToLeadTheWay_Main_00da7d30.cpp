// CGuardToLeadTheWay::Main() -- the minion clifftop-chase guard who leads the hero: register a thing-alive filter, attach Me to the active quest, wait for the nearest minion attacker to appear and fight it (healing 100 each time Me is hit) until it dies, take scripted control, say the "lead the way" hint once, release control, follow the hero, show Me's health bar, spawn the quest's GuardChatter thread on Me, wait for the mission to end and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA7D30, 1472 bytes, plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / default ctor 0x006E7B40 / dtor 0x004AA840 (inline copies in the first loop) /
//   operator= 0x008AB980 (inline in the first loop), ~CBaseClass 0x0099A2E0, operator new 0x00BFEA1A / delete 0x00BFE9BC, RegisterFilter
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsValid
//   0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0, CCharString 0x0099EBF0 / 0x0099EAE0, operator+ 0x0099F690 (const char*,
//   CCharString) / 0x0099F600 (CCharString, const char*) / 0x0099F570 (CCharString, CCharString), CSpawnedFuncBase ctor
//   0x00CDD450, CScriptBase::Add 0x00CB7E50; GameCalls slots 7, 8, 70, 77, 108, 229, 265, 321, 325, 364-366, 376, 451, 465,
//   480, 655; CScriptThing slots 3 GetDataString, 21 MsgIsHitBy, 75 IsAlive, 76 IsDead.
// Layout: Ego_r.pdb CGuardToLeadTheWay (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_MinionClifftopChaseScript, retail = PDB - 0x14: MissionFailed +0x49, MissionSucceeded +0x4a, PointedToPrisonPath +0x4e),
//   PMasterData +0x18, HealthBarID +0x1c. Spawned function as the landed CUndeadRisingZombieAttackerBoss::Main 0x00E0E2E0.
// Shape: ~CScriptThing and operator= are plain inline bodies (expanded in the attacker-search loop); inline_depth(0) on the hint
//   line sends the later temporaries and every exit out of line; the __forceinline CSpawnedFunc1 constructor is defined under
//   inline_depth(0) so it still expands while its by-value CScriptThing is destroyed out of line.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl {                              // the game-side thing behind CScriptThing::PImp
    virtual void i000() const; virtual void i001() const; virtual void i002() const; virtual void i003() const; virtual void i004() const; virtual void i005() const; virtual void i006() const; virtual void i007() const;
    virtual void i008() const; virtual void i009() const; virtual void i010() const; virtual void i011() const; virtual void i012() const; virtual void i013() const; virtual void i014() const; virtual void i015() const;
    virtual void i016() const; virtual void i017() const; virtual void i018() const; virtual void i019() const; virtual void i020() const; virtual void i021() const; virtual void i022() const; virtual void i023() const;
    virtual void i024() const; virtual void i025() const; virtual void i026() const; virtual void i027() const; virtual void i028() const; virtual void i029() const; virtual void i030() const; virtual void i031() const;
    virtual void i032() const; virtual void i033() const; virtual void i034() const; virtual void i035() const; virtual void i036() const; virtual void i037() const; virtual void i038() const; virtual void i039() const;
    virtual void i040() const; virtual void i041() const; virtual void i042() const; virtual void i043() const; virtual void i044() const; virtual void i045() const; virtual void i046() const; virtual void i047() const;
    virtual void i048() const; virtual void i049() const; virtual void i050() const; virtual void i051() const; virtual void i052() const; virtual void i053() const; virtual void i054() const; virtual void i055() const;
    virtual void i056() const; virtual void i057() const; virtual void i058() const; virtual void i059() const; virtual void i060() const; virtual void i061() const; virtual void i062() const; virtual void i063() const;
    virtual void i064() const; virtual void i065() const; virtual void i066() const; virtual void i067() const; virtual void i068() const; virtual void i069() const; virtual void i070() const; virtual void i071() const;
    virtual void i072() const; virtual void i073() const; virtual void i074() const;
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
namespace N_006e7b40 { struct CScriptThing : CBaseClass {                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const;
    virtual CCharString GetDataString() const;   // slot 3 (+0xc)
    virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const;
    virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const;
    virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const;
    virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const;
    virtual void s038() const; virtual void s039() const; virtual void s040() const; virtual void s041() const; virtual void s042() const; virtual void s043() const; virtual void s044() const; virtual void s045() const;
    virtual void s046() const; virtual void s047() const; virtual void s048() const; virtual void s049() const; virtual void s050() const; virtual void s051() const; virtual void s052() const; virtual void s053() const;
    virtual void s054() const; virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const; virtual void s061() const;
    virtual void s062() const; virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const; virtual void s067() const; virtual void s068() const; virtual void s069() const;
    virtual void s070() const; virtual void s071() const; virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
    virtual bool IsDead() const { return !IsAlive(); }   // slot 76 (+0x130)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing() { }                                 // out-of-line copy 0x004AA840
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }   // out-of-line copy 0x008AB980
    CScriptThing();                                     // 0x006E7B40
}; } using N_006e7b40::CScriptThing;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04 (copied and destroyed out of line)
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();
};
}
using NScript::CIsThingAlive;
// The filter's destructor is inlined into Main, but the CScriptThing member it destroys stays an out-of-line call (0x004AA840).
#pragma inline_depth(0)
inline CIsThingAlive::~CIsThingAlive() { }
#pragma inline_depth()
class CRGBColour { public:                                     // B,G,R,A bytes
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
struct CQ_MinionClifftopChaseScript {
    unsigned char _pad00[0x49];
    bool MissionFailed;                                 // +0x49 (PDB +0x5d)
    bool MissionSucceeded;                              // +0x4a (PDB +0x5e)
    unsigned char _pad4b[0x4e - 0x4b];
    bool PointedToPrisonPath;                           // +0x4e (PDB +0x62)
    void GuardChatter(CScriptThing guard);              // 0x00DA71E0
    void Add(CSpawnedFuncBase* function, const CCharString& name);   // 0x00CB7E50
};
typedef void (CQ_MinionClifftopChaseScript::*SpawnedThingCallback)(CScriptThing);
struct CSpawnedFunc1 : CSpawnedFuncBase {               // CSpawnedFunc1<CQ_MinionClifftopChaseScript, CScriptThing>, vtable 0x012D771C
    SpawnedThingCallback          Func;                 // +0x34
    CQ_MinionClifftopChaseScript* PClass;               // +0x38
    CScriptThing                  Par1;                 // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, SpawnedThingCallback func, CQ_MinionClifftopChaseScript* owner, CScriptThing par1);
};
// The constructor expands in Main, but the by-value CScriptThing it destroys stays an out-of-line call (0x004AA840).
#pragma inline_depth(0)
__forceinline CSpawnedFunc1::CSpawnedFunc1(const CCharString& name, unsigned long id, SpawnedThingCallback func, CQ_MinionClifftopChaseScript* owner, CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(owner), Par1(par1) { }
#pragma inline_depth()
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CGuardToLeadTheWay : CEntityScriptBase {
    CQ_MinionClifftopChaseScript* ParentClass;          // +0x14
    void* PMasterData;                                  // +0x18
    long HealthBarID;                                   // +0x1c
    void Main();
};

void CGuardToLeadTheWay::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guard;
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
    CScriptThing attacker = Game->GetNearestWithScriptName(Game->GetHero(), "MinionAttackingGuard");
    while (attacker.IsDead()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        attacker = Game->GetNearestWithScriptName(Game->GetHero(), "MinionAttackingGuard");
        Game->Pause(0.5f);
    }
    if (IsActiveThreadTerminating())
        return;
    if (attacker.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, attacker);
        Game->GiveThingBestEnemyTarget(attacker, Me);
        while (attacker.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating())
                    return;
                Game->ModifyThingHealth(Me, 100.0f, false);
            }
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->PointedToPrisonPath) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
#pragma inline_depth(0)
        Game->AddLineToConversation(conversation, "TEXT_QST_045_" + Me.GetDataString() + "_HINT_10", false, Me, CScriptThing());
#pragma inline_depth()
        ParentClass->PointedToPrisonPath = true;
    }
    guard.Release();
    Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->DisplayQuestInfo(true);
    HealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_GUARD", 1.0f);
    ParentClass->Add(new CSpawnedFunc1(CCharString("ParentClass.") + CCharString("GuardChatter"), 0,
                                       &CQ_MinionClifftopChaseScript::GuardChatter, ParentClass, Me), CCharString(""));
    while (!ParentClass->MissionFailed && !ParentClass->MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}
// CHTG_Prisoner::Main() -- the bound prisoner on the Hanging Tree (good) quest: take scripted control, bind his hands and make him unpushable, wait for the party to be set up, then until the quest state reaches 7: in state 2 follow the bandit leader; in state 5 find the nearest REGION_EXIT_POINT and, when it is a real route, try to escape towards it (the leader shouts for the guards when he gets 12 units away, he dies and fails the mission on reaching the exit, and goes back to following the leader when the hero catches him); in any other state stand still; finally release him
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D71B30, 1453 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / operator= 0x008AB980, CBaseClass ctor 0x0099A2D0, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / FollowThing 0x007E7320 / ClearCommands 0x007E7360 / IsPerformingScriptTask
//   0x007E7450 / dtor 0x007E74D0, IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA / FadeOutAndKillEntity 0x00CBE716
//   (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0 / operator!= 0x0099E960 / operator= 0x0099EFE0; GameCalls slots 7, 8, 70,
//   81 GetNearestWithDefName, 364-366 conversations, 376 Pause, 494, 745. Globals: empty string 0x0143E8EC, zero vector 0x0143E8E0.
// Layout: Ego_r.pdb CHTG_Prisoner: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_HangingTreeGoodScript, the usual
//   0x14 quest-script shift: PartySuccessfullySetUp +0x49, MissionFailed +0x4b, BanditsFlee +0x57, PrisonerEscaping +0x58,
//   HTG_State +0x5c, Leader +0x68, ReasonForFailing +0x9c), PMasterData +0x18, ChopBlockMarker +0x1c, ReturningToParty +0x28,
//   GuardsCalledAfterMe +0x29.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
    const CCharString& operator=(const char* text);          // 0x0099EFE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
class C3DVector { public: float X, Y, Z; };
extern const CCharString g_EmptyString;                // 0x0143E8EC
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
struct CScriptThingImpl {                              // the game-side thing behind CScriptThing::PImp
    virtual void i000() const;
    virtual const CCharString& GetDataString() const;   // slot 1 (+0x4)
    virtual void i002() const; virtual void i003() const; virtual void i004() const; virtual void i005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void i007() const; virtual void i008() const; virtual void i009() const; virtual void i010() const; virtual void i011() const; virtual void i012() const; virtual void i013() const; virtual void i014() const;
    virtual void i015() const; virtual void i016() const; virtual void i017() const; virtual void i018() const; virtual void i019() const; virtual void i020() const; virtual void i021() const; virtual void i022() const;
    virtual void i023() const; virtual void i024() const; virtual void i025() const; virtual void i026() const; virtual void i027() const; virtual void i028() const; virtual void i029() const; virtual void i030() const;
    virtual void i031() const; virtual void i032() const; virtual void i033() const; virtual void i034() const; virtual void i035() const; virtual void i036() const; virtual void i037() const; virtual void i038() const;
    virtual void i039() const; virtual void i040() const; virtual void i041() const; virtual void i042() const; virtual void i043() const; virtual void i044() const; virtual void i045() const; virtual void i046() const;
    virtual void i047() const; virtual void i048() const; virtual void i049() const; virtual void i050() const; virtual void i051() const; virtual void i052() const; virtual void i053() const; virtual void i054() const;
    virtual void i055() const; virtual void i056() const; virtual void i057() const; virtual void i058() const; virtual void i059() const; virtual void i060() const; virtual void i061() const; virtual void i062() const;
    virtual void i063() const; virtual void i064() const; virtual void i065() const; virtual void i066() const; virtual void i067() const; virtual void i068() const; virtual void i069() const; virtual void i070() const;
    virtual void i071() const; virtual void i072() const; virtual void i073() const; virtual void i074() const;
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const;
    virtual const CCharString& GetDataString() const { return !PImp.Data ? g_EmptyString : PImp.Data->GetDataString(); }   // slot 3 (+0xc)
    virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPos(); }   // slot 6 (+0x18)
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const;
    virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const; virtual void s021() const; virtual void s022() const;
    virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const;
    virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const;
    virtual void s039() const; virtual void s040() const; virtual void s041() const; virtual void s042() const; virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const;
    virtual void s047() const; virtual void s048() const; virtual void s049() const; virtual void s050() const; virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const;
    virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const;
    virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const; virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const;
    virtual void s071() const; virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    CScriptThing() { }
    virtual ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
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
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool fade, float time, bool kill);           // 0x00CBE716
struct CQ_HangingTreeGoodScript {
    unsigned char _pad00[0x49];
    bool PartySuccessfullySetUp;                        // +0x49 (PDB +0x5d)
    unsigned char _pad4a;
    bool MissionFailed;                                 // +0x4b (PDB +0x5f)
    unsigned char _pad4c[0x57 - 0x4c];
    bool BanditsFlee;                                   // +0x57 (PDB +0x6b)
    bool PrisonerEscaping;                              // +0x58 (PDB +0x6c)
    unsigned char _pad59[3];
    long HTG_State;                                     // +0x5c (PDB +0x70)
    unsigned char _pad60[0x68 - 0x60];
    CScriptThing Leader;                                // +0x68 (PDB +0x7c)
    unsigned char _pad74[0x9c - 0x74];
    CCharString ReasonForFailing;                       // +0x9c (PDB +0xb0)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CHTG_Prisoner : CEntityScriptBase {
    CQ_HangingTreeGoodScript* ParentClass;              // +0x14
    void*                     PMasterData;              // +0x18
    CScriptThing              ChopBlockMarker;          // +0x1c
    bool                      ReturningToParty;         // +0x28
    bool                      GuardsCalledAfterMe;      // +0x29
    void Main();
};

void CHTG_Prisoner::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase prisoner;
    if (prisoner.IsValid())
        prisoner.Release();
    while (!Game->StartScriptingEntity(Me, prisoner, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing exitPoint;
    Game->EntitySetAsHavingBoundHands(Me, true);
    Game->SetIsThingForcePushable(Me, false);
    while (!ParentClass->PartySuccessfullySetUp) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (ParentClass->HTG_State != 7) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->HTG_State == 2) {
            if (IsActiveThreadTerminating())
                return;
            prisoner.FollowThing(ParentClass->Leader, 1.0f, true);
            while (ParentClass->HTG_State == 2) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        } else if (ParentClass->HTG_State == 5) {
            if (IsActiveThreadTerminating())
                return;
            exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            GuardsCalledAfterMe = false;
            prisoner.ClearCommands();
            if (exitPoint.IsAlive() && exitPoint.GetDataString() != "NoQuestRoute") {
                if (IsActiveThreadTerminating())
                    return;
                while (ParentClass->HTG_State == 5) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                    if (!GuardsCalledAfterMe && !ReturningToParty
                        && IsDistanceBetweenThingsOver(Me, ParentClass->Leader, 12.0f)) {
                        long conversation = Game->AddNewConversation(ParentClass->Leader, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_030_GUARD_PRISONER_ESCAPING", false, ParentClass->Leader, Game->GetHero());
                        GuardsCalledAfterMe = true;
                    } else if (!prisoner.IsPerformingScriptTask()) {
                        prisoner.MoveToPosition(exitPoint.GetPos(), 0.0f, (EScriptEntityMoveType)0, false, true);
                        ParentClass->PrisonerEscaping = true;
                        ReturningToParty = false;
                    } else if (IsDistanceBetweenThingsUnder(Me, exitPoint, 1.5f)) {
                        FadeOutAndKillEntity(Me, true, 0.5f, true);
                        ParentClass->BanditsFlee = true;
                        Game->Pause(10.0f);
                        ParentClass->ReasonForFailing = "TEXT_QST_030_FAILURE_PRISONER_ESCAPED";
                        ParentClass->MissionFailed = true;
                    } else {
                        bool closeToHero = IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 3.0f);
                        if (closeToHero && !ReturningToParty) {
                            prisoner.ClearCommands();
                            prisoner.FollowThing(ParentClass->Leader, 1.0f, true);
                            if (GuardsCalledAfterMe) {
                                long conversation = Game->AddNewConversation(Me, false, false);
                                Game->AddLineToConversation(conversation, "TEXT_QST_030_PRISONER_RETURNING", false, Me, Game->GetHero());
                                GuardsCalledAfterMe = false;
                            }
                            ReturningToParty = true;
                            ParentClass->PrisonerEscaping = false;
                        } else if (ReturningToParty && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 4.0f)) {
                            prisoner.ClearCommands();
                            ReturningToParty = false;
                        }
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
                prisoner.ClearCommands();
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            prisoner.ClearCommands();
            while (ParentClass->HTG_State != 2 && ParentClass->HTG_State != 7) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    prisoner.ClearCommands();
    if (prisoner.IsValid())
        prisoner.Release();
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
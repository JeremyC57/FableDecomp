// CBH_GangGrunt::Main() -- bounty-hunt bandit grunt: removed at once unless the bounty-hunt intro is complete; else register the alive filter, wait for the bandit cutscene, walk to the thing named by Me's data string, wander around it facing the hero until the hero comes within 20 units or hits Me, then face the hero, say the next TEXT_QST_017_BANDIT<n> line, attack the hero and idle until terminated
// Leaf: ?Main@CBH_GangGrunt@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D15A40, length 1408 bytes, plain `ret`.
// Callees: as CBH_GangGruntSpawn::Main 0x00D15510 plus GFIntToCharString 0x0099F830 and operator+(const char*, const
//   CCharString&) 0x0099F690 (both `__fastcall` with a hidden result), ClearCommands 0x007E7360 (devirtualised on the
//   local handle). CGameScriptInterface slots 7, 8, 70, 72 GetThingWithScriptName, 108 RemoveThing, 364/365/366
//   conversation, 451 GiveThingBestEnemyTarget, 475 EntitySetFacingAngleTowardsThing, 763/766/769 SetWander*, 784
//   SetScriptingStateGroup.
// Literals: "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_017_BANDIT" 0x012CA540. Global: pGlobalScriptDef 0x0143E90C.
// Layout: Ego_r.pdb CBH_GangGrunt (28 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
//   +0x18. CQ_BountyHuntScript retail = PDB - 0x14: BanditCutsceneDone +0x6f, BountyHuntIntroComplete +0x70,
//   BanditGruntTextCounter +0x74. CScriptDef BountyHuntBanditGruntWanderDistance is PDB +0xbc8, retail +0xb20 (the
//   retail def is smaller ahead of it; read straight off the bytes).
namespace NScript { class CPredicateBase; }
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
extern C3DVector g_ZeroVector;                         // 0x0143E8E0
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);            // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
void operator delete(void* p);                         // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};
class CScriptThing;
template <class T> class CCountedPointer { public:                               // {Data, Info}
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE };
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    VPAD2(p0x0) VPAD1(p0x1)   // slots 0..2
    virtual CCharString GetDataString() const;   // slot 3 (+0xc)
    VPAD2(p1x0)   // slots 4..5
    __forceinline virtual const C3DVector& GetPos() const { if (!PImp.Data) return g_ZeroVector; return PImp.Data->GetPos(); }   // slot 6 (+0x18)
    VPAD8(p2x0) VPAD4(p2x1) VPAD2(p2x2)   // slots 7..20
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    VPAD16(p3x0) VPAD2(p3x1) VPAD1(p3x2)   // slots 22..40
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    VPAD32(p4x0)   // slots 43..74
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    CCountedPointer<CScriptThing> PImp;                              // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};

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
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 16 bytes
    CCountedPointer<CScriptThing> PImp;                              // +0x08
    VPAD2(s0) VPAD1(s1)                                // slots 1..3
    virtual void MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool);   // slot 4 (+0x10), 0x007E72F0
    bool IsValid() const;                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    virtual void ClearCommands();                              // 0x007E7360
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CQ_BountyHuntScript {
    unsigned char _pad[0x6f];
    bool BanditCutsceneDone;                           // +0x6f
    bool BountyHuntIntroComplete;                      // +0x70
    long BanditGruntTextCounter;                       // +0x74
};
class CScriptDef { public:
    unsigned char _pad[0xb20];
    float BountyHuntBanditGruntWanderDistance;         // +0xb20
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CBH_GangGrunt : public CEntityScriptBase {
public:
    void*                   __vftable;                 // +0x00
    CGameScriptInterface*   Game;                      // +0x04
    CScriptThing            Me;                        // +0x08
    CQ_BountyHuntScript*    ParentClass;               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    void Main();
};

void CBH_GangGrunt::Main()
{
    if (!ParentClass->BountyHuntIntroComplete) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, false);
        return;
    }
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->BanditCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    CCharString data = Me.GetDataString();
    CScriptThing target = Game->GetThingWithScriptName(data);
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    scripted.MoveToPosition(target.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, false);
    while (!IsDistanceBetweenThingsUnder(Me, target, 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (scripted.IsValid())
        scripted.Release();
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    Game->SetWanderCentrePoint(Me, target.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->BountyHuntBanditGruntWanderDistance);
    Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)
           && !Me.MsgIsHitBy("SCRIPT_NAME_HERO")
           && !(Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)0);
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    scripted.ClearCommands();
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    CCharString line = "TEXT_QST_017_BANDIT" + GFIntToCharString(ParentClass->BanditGruntTextCounter);
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
    if (ParentClass->BanditGruntTextCounter + 1 < 4) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->BanditGruntTextCounter++;
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (scripted.IsValid())
        scripted.Release();
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
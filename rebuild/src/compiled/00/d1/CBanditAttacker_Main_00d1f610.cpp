// CBanditAttacker::Main() -- Break-Siege bandit attacker: look up "MB_HeroTalksToChief", hold an alive filter over Me, wait for HeroLetInTown and StartSecondWave, take Me under scripted control (priority 3), set Me's combat break-off range from the script def, then (until BanditLeaderFree) keep running Me to "ImprisonedBanditLeader" while it is more than 5 units away; finally flag BanditLeaderFree, release scripting, make Me follow the leader and idle
// Leaf: ?Main@CBanditAttacker@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1F610, 757 bytes (768 with padding); entity-script vtable slot 1 (Main).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing copy ctor 0x004ABE90 /
//   ~CScriptThing 0x004AA840 (out-of-line copy) / ~CBaseClass 0x0099A2E0, CEntityScriptBase::SetPredicate 0x00F35B10 /
//   IsActiveThreadTerminating 0x00F35B30, handle (vtable 0x0127094C) ctor 0x007E72A0 (out of line in this module) /
//   IsScripting 0x00CD23B9 / Release 0x00CD2770 / MoveToThing 0x007E7300 / IsPerformingScriptTask 0x007E7450 / dtor
//   0x007E74D0, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall), operator delete 0x00BFE9BC;
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 72 GetThingWithScriptName,
//   229 EntityFollowThing, 817 SetCombatNearbyBreakOffRange (CScriptThing by value). Script def global 0x0143E90C.
//   Literals "MB_HeroTalksToChief" 0x012CB040, "ImprisonedBanditLeader" 0x012CAD60.
// Layout: Ego_r.pdb CBanditAttacker (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_BreakSiegeScript: retail = PDB - 0x14, so BanditLeaderFree +0x4c, StartSecondWave +0x4f, HeroLetInTown +0x51),
//   PMasterData +0x18, HeroHitMe +0x1c.
// Shape: CScriptThing has an INLINE destructor (VC7.1 expands it at the first return only, as in the landed
//   CTrophiesBriarTrophy); the filter member is a plain CScriptThing, and `#pragma inline_depth(0)` around the filter
//   class keeps its member destruction an out-of-line call to ~CScriptThing (retail calls 0x004AA840 there).
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes

    void*           PImp_Data;                          // +0x04
    CCPPointerInfo* PImp_Info;                          // +0x08
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing()                                     // inline here; out-of-line copy 0x004AA840
    {
        if (PImp_Info) {
            --PImp_Info->RefCount;
            if (PImp_Info->RefCount == 0) {
                PImp_Info->DeleteFunc(PImp_Info->Data);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                         // +0x08
    CScriptGameResourceObjectScriptedThingBase();      // 0x007E72A0 (out of line in this module)
    virtual ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004();
    virtual void MoveToThing(const CScriptThing& thing, float distance, EScriptEntityMoveType moveType, bool a, bool b, bool c, bool d);   // slot 5 (+0x14)
    virtual void s006(); virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013();
    virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020(); virtual void s021();
    virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025();
    virtual bool IsPerformingScriptTask() const;   // slot 26 (+0x68)
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class __declspec(novtable) CPredicateBase
{
public:
    virtual void f00();
};
}
#pragma inline_depth(0)
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing    m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#pragma inline_depth()
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
class CScriptDef { public:
    unsigned char _pad[0xb8c];
    float         BanditAttackerBreakOffRange;         // +0xb8c (retail offset)
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_BreakSiegeScript {
    unsigned char _pad[0x4c];
    bool BanditLeaderFree;                             // +0x4c
    bool BanditSiegeStart;                             // +0x4d
    bool HasFirstWaveAttacked;                         // +0x4e
    bool StartSecondWave;                              // +0x4f
    bool BanditsAttackHero;                            // +0x50
    bool HeroLetInTown;                                // +0x51
};
struct CBanditAttacker : CEntityScriptBase {
    CQ_BreakSiegeScript* ParentClass;                  // +0x14
    void*                PMasterData;                  // +0x18 CQ_SunnyvaleMasterData*
    bool                 HeroHitMe;                    // +0x1c
    void Main();
};

void CBanditAttacker::Main()
{
    CScriptThing chief = Game->GetThingWithScriptName("MB_HeroTalksToChief");
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->HeroLetInTown) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->StartSecondWave) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCombatNearbyBreakOffRange(Me, pGlobalScriptDef->BanditAttackerBreakOffRange);
    CScriptThing leader = Game->GetThingWithScriptName("ImprisonedBanditLeader");
    while (IsDistanceBetweenThingsOver(Me, leader, 5.0f) && !ParentClass->BanditLeaderFree) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!scripted.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            scripted.MoveToThing(leader, 0.0f, (EScriptEntityMoveType)1, false, true, false, true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->BanditLeaderFree = true;
    if (scripted.IsScripting())
        scripted.Release();
    Game->EntityFollowThing(Me, leader, 1.0f, true);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
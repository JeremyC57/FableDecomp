// CBanditFirstWave::Main() -- Break-Siege first-wave bandit: hold an alive filter over Me, take Me under scripted control (priority 3), set Me's combat break-off range from the script def, then (until BanditLeaderFree) keep running Me to "ImprisonedBanditLeader" while it is more than 5 units away; finally flag BanditLeaderFree, release scripting, make Me follow the leader and idle
// Leaf: ?Main@CBanditFirstWave@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1FA00, 557 bytes (560 with padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 (filter member) / ~CScriptThing 0x004AA840, CEntityScriptBase::SetPredicate
//   0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380, handle (vtable 0x0127094C) IsScripting
//   0x00CD23B9 / Release 0x00CD2770 / MoveToThing 0x007E7300 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall);
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 72 GetThingWithScriptName, 229 EntityFollowThing,
//   817 SetCombatNearbyBreakOffRange (CScriptThing by value: the copy is expanded inline, CBase vtable
//   0x01231710 then CScriptThing 0x01238C8C, AddRef; written as a tagged inline constructor whose temporary is built
//   straight into the argument slot, so 0x004ABE90 binds only to CScriptThing's own copy ctor). Script def global 0x0143E90C. Literal "ImprisonedBanditLeader" 0x012CAD60.
// Layout: Ego_r.pdb CBanditFirstWave: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_BreakSiegeScript,
//   retail = PDB - 0x14: BanditLeaderFree +0x4c), PMasterData +0x18.
// Shape: the tail of the sibling CBanditAttacker::Main (0x00D1F610); the filter member is a plain CScriptThing (out-of-line
//   copy ctor 0x004ABE90 / dtor 0x004AA840); the by-value argument copy is the inline tagged constructor.
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
struct CBase {                                          // vtable 0x01231710
    CBase() { }
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes

    struct PImpPointer {                                // CCountedPointer<CScriptThing>
        void*           Data;
        CCPPointerInfo* Info;
        PImpPointer(const PImpPointer& other) : Data(other.Data), Info(other.Info)
        {
            if (Info)
                ++Info->RefCount;
        }
    } PImp;                                             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the by-value argument copy VC7.1 expands inline
    CScriptThing(const CScriptThing& other, InlineCopy) : PImp(other.PImp) { }
    ~CScriptThing();                                    // 0x004AA840
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
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
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
class CPredicateBase
{
public:
    virtual void f00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing    m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
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
};
struct CBanditFirstWave : CEntityScriptBase {
    CQ_BreakSiegeScript* ParentClass;                  // +0x14
    void*                PMasterData;                  // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CBanditFirstWave::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
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
    Game->SetCombatNearbyBreakOffRange(CScriptThing(Me, CScriptThing::InlineCopy()), pGlobalScriptDef->BanditAttackerBreakOffRange);
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
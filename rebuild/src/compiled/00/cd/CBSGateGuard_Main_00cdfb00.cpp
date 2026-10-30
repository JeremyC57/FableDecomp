// CBSGateGuard::Main() -- Bowerstone gate guard: hold an alive filter over Me, take Me under scripted control (priority 1); if the guard has wandered more than 1 unit from his home position, walk him back there (re-issuing MoveToPosition until within 2 units), then face the hero; afterwards idle one script frame at a time until the thread terminates
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDFB00, 563 bytes (576 with padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (CIsThingAlive filter, vtable 0x012C32F0, inline dtor here),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380,
//   scripted-thing handle (vtable 0x0127094C) IsValid 0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0 (slot 4) /
//   GetScriptThing 0x007E7490 (slot 12) / IsPerformingScriptTask 0x007E7450 (slot 26) / dtor 0x007E74D0,
//   IsDistanceFromThingToPositionOver 0x00CBE45C (__fastcall), CScriptThing slot 7 GetHomePos (by value);
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 475 EntitySetFacingAngleTowardsThing
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Layout: Ego_r.pdb CBSGateGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the landed CCham::Main / CTollBandit::Should_I_Flee models; the home position is a named by-value local.
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* PStringData; };
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual C3DVector GetHomePos() const;   // slot 7 (+0x1c)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
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
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
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
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CBSGateGuard : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CPersonalScript_GlobalThingsScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CBSGateGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)1)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsDistanceFromThingToPositionOver(Me, Me.GetHomePos(), 1.0f)) {
        if (IsActiveThreadTerminating())
            return;
        C3DVector home = Me.GetHomePos();
        while (IsDistanceFromThingToPositionOver(scripted.GetScriptThing(), home, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            scripted.MoveToPosition(home, 0.0f, (EScriptEntityMoveType)0, false, true);
            while (scripted.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
// CMeleeThunder::Main() (0x00E386E0) -- conscious filter on Me; the first time, take scripted control (priority 4) and play
//   the jump-from-chest combat animation to its end; then take scripted control (priority 3), make Me the hero's enemy,
//   follow the hero and idle until the script is done
// Leaf: ?Main@CMeleeThunder@
// Retail: Fable.exe @ 0x00E386E0, 523 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingConscious filter (vtable 0x012C2FE8, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing
//   0x004AA840), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380, the
//   scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayCombatAnimation 0x007E73E0 /
//   IsPerformingScriptTask 0x007E7450 / FollowThing 0x007E7320 / destructor 0x007E74D0; GameCalls slots 7
//   NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 597 EntitySetThingAsEnemyOfThing. String "SCRIPT_JUMP_FROM_CHEST"
//   0x012E3720.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} + ParentClass +0x14, PMasterData +0x18, a "jumped" flag +0x1c; the
//   scripted-thing handle is the landed CAttackingAssassin::Main 0x00E383A0 model.
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
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
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (the row's local CBase)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
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
struct CIsThingConscious : public NScript::CPredicateBase {                              // vtable 0x012C2FE8: a filter holding a script thing
    CScriptThing m_thing;                               // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
#pragma inline_depth()
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CMeleeThunder : CEntityScriptBase {
    void* ParentClass;                                  // +0x14
    void* PMasterData;                                  // +0x18
    bool  HasJumped;                                    // +0x1c
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CMeleeThunder::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (!HasJumped) {
        if (IsActiveThreadTerminating())
            return;
        HasJumped = true;
        if (scripted.IsValid())
            scripted.Release();
        while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        scripted.PlayCombatAnimation("SCRIPT_JUMP_FROM_CHEST", true, false, false, true, false, false);
        while (scripted.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
    scripted.FollowThing(Game->GetHero(), 1.0f, true);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

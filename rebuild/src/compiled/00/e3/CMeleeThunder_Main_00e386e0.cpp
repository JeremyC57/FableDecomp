// CMeleeThunder::Main() (0x00E386E0) -- conscious filter on Me; the first time, take scripted control (priority 4) and play
//   the jump-from-chest combat animation to its end; then take scripted control (priority 3), make Me the hero's enemy,
//   follow the hero and idle until the script is done
// Leaf: ?Main@CMeleeThunder@
// Retail: Fable.exe @ 0x00E386E0, 523 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingConscious filter (vtable 0x012C2FE8, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing
//   0x004AA840), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380, the
//   scripted-thing handle's IsScripting 0x00CD23B9 / Release 0x00CD2770 / PlayCombatAnimation 0x007E73E0 /
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
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline ~CScriptThing() {}                    // inline here; out-of-line copy 0x004AA840
};
class CBaseObject { public:                                    // CBaseIntelligentPointer root
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                          // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
public:
    virtual void FollowThing(const CScriptThing&, float, bool);   // 0x007E7320
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
    virtual void PlayCombatAnimation(const CCharString&, bool, bool, bool, bool, bool, bool);   // 0x007E73E0
};
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
class CEntityScriptBase { public:                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CMeleeThunder : CEntityScriptBase {
    void* ParentClass;                                  // +0x14
    void* PMasterData;                                  // +0x18
    bool  HasJumped;                                    // +0x1c
    void Main();
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
        if (scripted.IsScripting())
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
    if (scripted.IsScripting())
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

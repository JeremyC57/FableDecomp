// NScript::CQ_HobbeToothContestScript::CHobbeBarrel::Main() -- watch barrel hits while the drinking-hobbes flag remains set.
// Retail00D9C4A0,187 bytes. Actual CScriptThing/CGameScriptInterface headers retained; no CHobbeBarrel/quest headers exist.
// Native quest DrinkingHobbesNotHit+5A versus donor+6E; PDB CHobbeBarrel ParentClass+14 agrees native.
// F35B10=SetPredicate (CB7920 forwards active thread); F35B30=IsActiveThreadTerminating (CB7940 reads active-thread flag).
// Temporary predicate is CIsThingConscious: VT12C2FE8 operatorCDDAB0 tests Thing.IsAlive && !Thing.IsUnconscious.
// Thing copy ctor4ABE90,dtor4AA840; string ctor99EBF0,dtor99EAE0. Native empty literal122D70E (NUL).
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
namespace NScript {
struct ScriptThing : ::CScriptThing {
    bool MsgIsHitBy(const CCharString&) const;
    ScriptThing(const ScriptThing&); //004ABE90
    ~ScriptThing(); //004AA840
};
struct __declspec(novtable) CPredicateBase {
    virtual bool operator()() const=0;
    virtual CPredicateBase* Clone() const=0;
    virtual ~CPredicateBase() {}
};
class CIsThingConscious : public CPredicateBase { public:
    ScriptThing Thing;
    CIsThingConscious(const ScriptThing& t):Thing(t) {}
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CIsThingConscious() {}
};
struct ScriptGame;
struct CHobbeBarrel;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot { void* Address; void (ScriptGame::*Wait)() const; };
union ThingSlot { void* Address; bool (ScriptThing::*MsgIsHitBy)(const CCharString&) const; };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
FABLE_STATIC_ASSERT(sizeof(ThingSlot)==4);
#endif
__forceinline bool ScriptThing::MsgIsHitBy(const CCharString& name) const { return (this->*static_cast<const ThingSlot*>(__vftable)[21].MsgIsHitBy)(name); }
struct ScriptGame : ::CGameScriptInterface {
    __forceinline void WaitForNextFrame() const { (this->*static_cast<GameSlot*>(__vftable)[7].Wait)(); }
};
class CQ_HobbeToothContestScript { public:
    unsigned char BaseAndQuestFields[0x5a];
    bool DrinkingHobbesNotHit;
};
struct CHobbeBarrel {
    void* VTable;
    ScriptGame* Game;
    ScriptThing Me;
    CQ_HobbeToothContestScript* ParentClass;
    void* PMasterData;
    void SetPredicate(const CPredicateBase&); //00F35B10
    bool IsActiveThreadTerminating() const; //00F35B30
    void Main();
};
void CHobbeBarrel::Main() {
    SetPredicate(CIsThingConscious(Me));
    Game->WaitForNextFrame();
    if(IsActiveThreadTerminating()) return;
    while(ParentClass->DrinkingHobbesNotHit) {
        Game->WaitForNextFrame();
        if(IsActiveThreadTerminating()) return;
        bool hit;
        { CCharString name("",-1); hit=Me.MsgIsHitBy(name); }
        if(hit) {
            bool terminating=IsActiveThreadTerminating();
            if(terminating) return;
            ParentClass->DrinkingHobbesNotHit=terminating;
        }
    }
    IsActiveThreadTerminating();
}
}
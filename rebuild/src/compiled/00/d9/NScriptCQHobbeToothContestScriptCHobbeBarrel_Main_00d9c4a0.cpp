// NScript::CQ_HobbeToothContestScript::CHobbeBarrel::Main() -- watch barrel hits while the drinking-hobbes flag remains set.
// Leaf: ?Main@CHobbeBarrel@CQ_HobbeToothContestScript@NScript@@EAEXXZ
// Naming wave 30 (family cst): the invented second view NScript::ScriptThing (??1ScriptThing@NScript@@QAE@XZ) over the flattened engine/CScriptThing.h is dropped: engine/CScriptThing.h's retail
//   class declares the out-of-line copy 0x004ABE90 and the virtual ~CScriptThing 0x004AA840 itself (Ego_r.pdb / FableWin.pdb
//   ??0CScriptThing@@QAE@ABV0@@Z / ??1CScriptThing@@UAE@XZ);
//   its MsgIsHitBy is the retail class's virtual slot 21 (was a member-pointer call through the flattened vfptr)
// Retail00D9C4A0,187 bytes. Actual CScriptThing/CGameScriptInterface headers retained; no CHobbeBarrel/quest headers exist.
// Native quest DrinkingHobbesNotHit+5A versus donor+6E; PDB CHobbeBarrel ParentClass+14 agrees native.
// F35B10=SetPredicate (CB7920 forwards active thread); F35B30=IsActiveThreadTerminating (CB7940 reads active-thread flag).
// Temporary predicate is CIsThingConscious: VT12C2FE8 operatorCDDAB0 tests Thing.IsAlive && !Thing.IsUnconscious.
// Thing copy ctor4ABE90,dtor4AA840; string ctor99EBF0,dtor99EAE0. Native empty literal122D70E (NUL).
// Naming wave 35: identity reg 00d9a55c "HobbeBarrel" in 00d9a190; Alloc 00d9c3c0; vtable 012d6b48 slot 1; PDB ?Main@CHobbeBarrel@CQ_HobbeToothContestScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CHobbeBarrel::Main (wave3/labels_alloc.tsv); PDB ?Main@CHobbeBarrel@CQ_HobbeToothContestScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CHobbeBarrel@NScript@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); NScript-level CHobbeBarrel nested into CQ_HobbeToothContestScript); same bytes.
#include "fable_compiler.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840
#include "fable_string.h"
namespace NScript { class CPredicateBase; }
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class __declspec(novtable) CPredicateBase { public:
    virtual bool operator()() const=0;
    virtual CPredicateBase* Clone() const=0;
    virtual ~CPredicateBase() {}
};
class CIsThingConscious : public CPredicateBase { public:
    CScriptThing Thing;
    CIsThingConscious(const CScriptThing& t):Thing(t) {}
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CIsThingConscious() {}
};
struct ScriptGame;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot { void* Address; void (ScriptGame::*Wait)() const; };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
#endif
struct ScriptGame : ::CGameScriptInterface {
    FABLE_FORCEINLINE void WaitForNextFrame() const { (this->*static_cast<GameSlot*>(__vftable)[7].Wait)(); }
};
class CQ_HobbeToothContestScript { public:
    unsigned char BaseAndQuestFields[0x5a];
    bool DrinkingHobbesNotHit;
public:
    class CHobbeBarrel;              // the entity class below (naming wave 35)
};
class CQ_HobbeToothContestScript::CHobbeBarrel : public CEntityScriptBase {
public:
        ScriptGame* Game;
    CScriptThing Me;
    CQ_HobbeToothContestScript* ParentClass;
    void* PMasterData;
private:
    virtual void Main();                          // PDB ?Main@CHobbeBarrel@CQ_HobbeToothContestScript@NScript@@EAEXXZ
};
void CQ_HobbeToothContestScript::CHobbeBarrel::Main() {
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
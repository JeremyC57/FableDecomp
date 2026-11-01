// CGoalChest::Main -- watch the chest until hero use ends or script termination, then flag the parent
// Leaf: ?Main@CGoalChest@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Naming wave 27 (family main, DECISIONS_W26 D1): Ego_r.pdb / FableWin.pdb nest the entity in its script class and spell
//   Main a private virtual (?Main@CGoalChest@CPersonalScript_EmptyScript@NScript@@EAEXXZ); the local CEntityScriptBase carries the retail slot list of vtable 0x012C3224
//   (engine/CEntityScriptBase.h). Was: ?Main@CGoalChest@@QAEXXZ over a global CPersonalScript_EmptyScript, the vptr a `void* VTable` member.
// Naming wave 26 (family pred): CEntityScriptBase is the global PDB class (Ego_r.pdb / FableWin.pdb) with
//   ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z 0x00F35B10 and the const
//   ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30; the predicate parameter is an NScript::CPredicateBase.
//   Was: SetPredicate declared on CGoalChest (?SetPredicate@CGoalChest@@...), which cast `this` to CEntityScriptBase for the accessor.
// Retail 00EF29E0 length221. Predicate VT012C32F0; copy4ABE90, SetPredicateF35B10, predicate dtorE241B0, terminatingF35B30.
// Actual CScriptThing and CGameScriptInterface headers retained; native field-free method projections.
// String125D1C8 is SCRIPT_NAME_HERO. Slot20 CScriptThing::MsgIsUsedBy=004AAB10; game slot7 NewScriptFrame=006E7410.
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
struct ScriptThingCopy:CScriptThing { ScriptThingCopy(const CScriptThing&); ~ScriptThingCopy(); };
namespace NScript {
class CPredicateBase { public: virtual bool operator()() const=0; virtual CPredicateBase* Clone() const=0; virtual ~CPredicateBase(){} };
class CIsThingAlive:public CPredicateBase { public:
 ScriptThingCopy Thing;
 __forceinline CIsThingAlive(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const; virtual CPredicateBase* Clone()const;
 ~CIsThingAlive(); //00E241B0 out of line
};
}
struct GameCalls; struct ThingCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot { void* Address; void(GameCalls::*NewScriptFrame)(); };
union ThingSlot { void* Address; unsigned char(ThingCalls::*MsgIsUsedBy)(const CCharString&)const; };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4); FABLE_STATIC_ASSERT(sizeof(ThingSlot)==4);
#endif
struct GameCalls:CGameScriptInterface {
 __forceinline void NewScriptFrame(){(this->*reinterpret_cast<GameSlot*>(__vftable)[7].NewScriptFrame)();}
};
struct ThingCalls:CScriptThing {
 __forceinline bool NotUsedByHero() const { CCharString name("SCRIPT_NAME_HERO",-1); return MsgIsUsedBy(name) ? false : true; }
 __forceinline unsigned char MsgIsUsedBy(const CCharString& name)const {return(this->*reinterpret_cast<const ThingSlot*>(__vftable)[20].MsgIsUsedBy)(name);}
};
namespace NScript {
class CPersonalScript_EmptyScript { public: unsigned char Unknown[0x48]; bool GoalChestFinished; struct CGoalChest; };   // Ego_r.pdb NScript::
}
using NScript::CPersonalScript_EmptyScript;
class CScriptBase; class CPersistContext;
class CEntityScriptBase { public:   // global PDB class (Game / Me are declared on the entity in this row's model)
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
    virtual void Main() = 0;                            // slot 1 (vtable 0x012C3224, engine/CEntityScriptBase.h)
    virtual void Init();                                // slot 2  0x00CDEBB0
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext&);           // slot 4  0x00CDEBC0
    virtual void OnPredicateFail();                     // slot 5  0x00CDEBD0
    virtual void OnInterrupted();                       // slot 6  0x00CDEBE0
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10 (Ego_r.pdb CEntityScriptBase)
};
struct CPersonalScript_EmptyScript::CGoalChest : CEntityScriptBase {   // the PDB base (its vptr at +0x00)
 GameCalls* Game; ThingCalls Me; CPersonalScript_EmptyScript* ParentClass; void* PMasterData;
private:
 virtual void Main();   // private virtual (Ego_r / FableWin EAEXXZ)
};
void CPersonalScript_EmptyScript::CGoalChest::Main() {
 { NScript::CIsThingAlive filter(Me); SetPredicate(filter); }
 Game->NewScriptFrame();
 if(!IsActiveThreadTerminating()) {
  while(Me.NotUsedByHero()) {
   Game->NewScriptFrame();
   if(IsActiveThreadTerminating()) return;
  }
  if(!IsActiveThreadTerminating()) ParentClass->GoalChestFinished=true;
 }
}
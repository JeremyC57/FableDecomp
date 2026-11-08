// CDoor::Main -- open while the hero's hand lamp is lit; close when extinguished, until termination
// Actual CScriptThing/CGameScriptInterface headers. Predicate CIsThingAlive nativeVT012C32F0, copy4ABE90,dtorE241B0.
// Game native slots confirmed in cgsi_slots_aligned.tsv and retail dispatch:7 NewScriptFrame,185 IsHeroHandLampLit,425 OpenDoor,426 CloseDoor.
// SetPredicateF35B10; IsActiveThreadTerminatingF35B30. Native278 bytes.
// Leaf: ?Main@CDoor@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Naming wave 27 (family main, DECISIONS_W26 D1): Ego_r.pdb / FableWin.pdb nest the entity in its script class and spell
//   Main a private virtual (?Main@CDoor@CPersonalScript_EmptyScript@NScript@@EAEXXZ); the local CEntityScriptBase carries the retail slot list of vtable 0x012C3224
//   (engine/CEntityScriptBase.h). Was: ?Main@CDoor@@QAEXXZ (not nested), the vptr a `void* VTable` member.
// Naming wave 26 (family pred): CEntityScriptBase is the global PDB class (Ego_r.pdb / FableWin.pdb) with
//   ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z 0x00F35B10 and the const
//   ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30; the predicate parameter is an NScript::CPredicateBase.
//   Was: SetPredicate declared on CDoor (?SetPredicate@CDoor@@...), which cast `this` to CEntityScriptBase for the accessor.
#include "fable_compiler.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
struct ScriptThingCopy:CScriptThing {ScriptThingCopy(const CScriptThing&);~ScriptThingCopy();};
namespace NScript {
class CPredicateBase { public:virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
class CIsThingAlive:public CPredicateBase { public:
 ScriptThingCopy Thing;
 FABLE_FORCEINLINE CIsThingAlive(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const;virtual CPredicateBase* Clone()const;
 ~CIsThingAlive(); //00E241B0
};
}
struct GameCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union GameSlot {void* Address;void(GameCalls::*NewScriptFrame)()const;bool(GameCalls::*IsHeroHandLampLit)()const;void(GameCalls::*Door)(const CScriptThing&)const;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
#endif
struct GameCalls:CGameScriptInterface {
 void NewScriptFrame()const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[7].NewScriptFrame)();}
 bool IsHeroHandLampLit()const{return(this->*reinterpret_cast<const GameSlot*>(__vftable)[185].IsHeroHandLampLit)();}
 void OpenDoor(const CScriptThing& t)const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[425].Door)(t);}
 void CloseDoor(const CScriptThing& t)const{(this->*reinterpret_cast<const GameSlot*>(__vftable)[426].Door)(t);}
};
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
namespace NScript {
class CPersonalScript_EmptyScript { public: struct CDoor; };   // Ego_r.pdb / FableWin.pdb NScript::CPersonalScript_EmptyScript
}
struct NScript::CPersonalScript_EmptyScript::CDoor : CEntityScriptBase {GameCalls* Game;CScriptThing Me;   // the PDB base (its vptr at +0x00)
private:
 virtual void Main();   // private virtual (Ego_r / FableWin EAEXXZ)
};
void NScript::CPersonalScript_EmptyScript::CDoor::Main(){
 {NScript::CIsThingAlive p(Me);SetPredicate(p);}
 Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 while(!IsActiveThreadTerminating()){
  while(!Game->IsHeroHandLampLit()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  Game->OpenDoor(Me);
  while(Game->IsHeroHandLampLit()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  Game->CloseDoor(Me);Game->NewScriptFrame();
 }
}
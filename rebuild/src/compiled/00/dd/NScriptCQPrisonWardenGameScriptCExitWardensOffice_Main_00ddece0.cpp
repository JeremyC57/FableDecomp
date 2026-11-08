// NScript::CQ_PrisonWardenGameScript::CExitWardensOffice::Main() -- keep door locked; show locked notice when hero uses door and await dismissal.
// Leaf: ?Main@CExitWardensOffice@CQ_PrisonWardenGameScript@NScript@@EAEXXZ
// Native00DDECE0,235 bytes;99EBF0 CCharString ctor,99EAE0 dtor,F35B30 termination wrapper.
// Interface vtable1260F0C slots7 NewScriptFrame6E7410,40 MsgIsGameInfoClickedPast894370,115 DisplayGameInfo8929D0,518 EntitySetAsLocked89DC10.
// ScriptThing vtable1238C8C slot20 MsgIsUsedBy4AAB10. Signatures from native manifest and PDB.
// Naming wave 35: identity reg 00dddf4b "ExitWardensOffice" in 00ddde20; Alloc 00ddec30; vtable 012dccf4 slot 1; PDB ?Main@CExitWardensOffice@CQ_PrisonWardenGameScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CExitWardensOffice::Main (wave3/labels_alloc.tsv); PDB ?Main@CExitWardensOffice@CQ_PrisonWardenGameScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CExitWardensOffice@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes.
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
// VC7.1 single-inheritance member pointers contain one code address. View the vtable entry
// by reference to retain virtual dispatch shape; fixture builds genuine member entries for each host ABI.
namespace NScript { class CPredicateBase; }
template<class M> FABLE_FORCEINLINE const M& VirtualSlot(void* table,long n) {
#ifdef _MSC_VER
 typedef char NativeMemberPointerMustBeOneWord[sizeof(M)==sizeof(void*)?1:-1];
#endif
 return reinterpret_cast<const M*>(table)[n];
}
struct InterfaceMethods : CGameScriptInterface {
 void NewScriptFrame(); bool MsgIsGameInfoClickedPast(); void DisplayGameInfo(const CCharString&); void EntitySetAsLocked(const CScriptThing&,bool);
 void Frame(){(this->*VirtualSlot<void(InterfaceMethods::*)()>(__vftable,7))();}
 bool Clicked(){return (this->*VirtualSlot<bool(InterfaceMethods::*)()>(__vftable,40))();}
 void Info(const CCharString&s){(this->*VirtualSlot<void(InterfaceMethods::*)(const CCharString&)>(__vftable,115))(s);}
 void Lock(const CScriptThing&t,bool b){(this->*VirtualSlot<void(InterfaceMethods::*)(const CScriptThing&,bool)>(__vftable,518))(t,b);}
};
struct ThingMethods : CScriptThing {
 bool MsgIsUsedBy(const CCharString&);
 bool Used(const CCharString&s){return (this->*VirtualSlot<bool(ThingMethods::*)(const CCharString&)>(__vftable,20))(s);}
};
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_PrisonWardenGameScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CExitWardensOffice;
};
}
using NScript::CQ_PrisonWardenGameScript;
namespace NScript {
class CQ_PrisonWardenGameScript::CExitWardensOffice : public CEntityScriptBase {
public:
 InterfaceMethods* Interface; ThingMethods Door;
 void* ParentClass;void* PMasterData;
private:
    virtual void Main();                          // PDB ?Main@CExitWardensOffice@CQ_PrisonWardenGameScript@NScript@@EAEXXZ
};
}
void NScript::CQ_PrisonWardenGameScript::CExitWardensOffice::Main() {
 Interface->Lock(Door,true);
 while(!IsActiveThreadTerminating()) {
  bool used; { CCharString hero("SCRIPT_NAME_HERO",-1); used=Door.Used(hero); }
  if(used) {
   if(IsActiveThreadTerminating())return;
   { CCharString notice("TEXT_QST_055_WARDENS_DOOR_LOCKED",-1); Interface->Info(notice); }
   while(!Interface->Clicked()) {
    Interface->Frame();if(IsActiveThreadTerminating())return;
   }
   if(IsActiveThreadTerminating())return;
  }
  Interface->Frame();
 }
}
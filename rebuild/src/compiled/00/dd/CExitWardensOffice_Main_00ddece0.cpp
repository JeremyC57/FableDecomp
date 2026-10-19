// CExitWardensOffice::Main() -- keep door locked; show locked notice when hero uses door and await dismissal.
// Native00DDECE0,235 bytes;99EBF0 CCharString ctor,99EAE0 dtor,F35B30 termination wrapper.
// Interface vtable1260F0C slots7 NewScriptFrame6E7410,40 MsgIsGameInfoClickedPast894370,115 DisplayGameInfo8929D0,518 EntitySetAsLocked89DC10.
// ScriptThing vtable1238C8C slot20 MsgIsUsedBy4AAB10. Signatures from native manifest and PDB.
#include "fable_string.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
// VC7.1 single-inheritance member pointers contain one code address. View the vtable entry
// by reference to retain virtual dispatch shape; fixture builds genuine member entries for each host ABI.
template<class M> __forceinline const M& VirtualSlot(void* table,long n) {
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
struct CExitWardensOffice {
 void* ScriptVptr;InterfaceMethods* Interface; ThingMethods Door;
 void* ParentClass;void* PMasterData;
 bool IsDone(); //F35B30
 void Main();
};
void CExitWardensOffice::Main() {
 Interface->Lock(Door,true);
 while(!IsDone()) {
  bool used; { CCharString hero("SCRIPT_NAME_HERO",-1); used=Door.Used(hero); }
  if(used) {
   if(IsDone())return;
   { CCharString notice("TEXT_QST_055_WARDENS_DOOR_LOCKED",-1); Interface->Info(notice); }
   while(!Interface->Clicked()) {
    Interface->Frame();if(IsDone())return;
   }
   if(IsDone())return;
  }
  Interface->Frame();
 }
}
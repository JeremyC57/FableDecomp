// CFadeinSummoner::PhaseMe() -- oscillate summoner alpha about0..0.85, respecting script termination.
// Native00D9FD80 180 bytes. F35B30 active-thread termination wrapper; D9FE40 GetAlphaChange.
// CGameScriptInterface vtable1260F0C slot510=89DA70 EntitySetAlpha(const CScriptThing&,float,bool), PDB confirmed.
// Header's data model retains the real vptr; typed sparse ABI table names slot510 without inventing virtual methods.
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
struct ScriptInterfaceMethods : CGameScriptInterface { void EntitySetAlpha(const CScriptThing&,float,bool); };
typedef void (ScriptInterfaceMethods::*AlphaMethod)(const CScriptThing&,float,bool);
// VC7.1 single-inheritance member pointers are one code address; host fixture supplies its own ABI bridge.
union AlphaEntry { void* Address; AlphaMethod Member; };
FABLE_STATIC_ASSERT(sizeof(AlphaMethod)==sizeof(void*));
struct CFadeinSummoner {
 void* ScriptVptr; CGameScriptInterface* Interface; CScriptThing Summoner;
 void* ParentClass; void* PMasterData; float MyAlpha,AlphaGradient; bool AlphaMax;
 bool IsDone(); //00F35B30
 float GetAlphaChange(); //00D9FE40
 void SetAlpha(float alpha) { AlphaEntry slot; slot.Address=((void**)Interface->__vftable)[510]; (static_cast<ScriptInterfaceMethods*>(Interface)->*slot.Member)(Summoner,alpha,true); }
 void PhaseMe();
};
void CFadeinSummoner::PhaseMe() {
 if(!AlphaMax) {
  if(!IsDone()) {
   MyAlpha+=GetAlphaChange(); SetAlpha(MyAlpha);
   if(MyAlpha>=0.85f) { AlphaMax=true; MyAlpha-=MyAlpha-0.85f; }
  }
 } else {
  if(!IsDone()) {
   MyAlpha-=GetAlphaChange();SetAlpha(MyAlpha);
   if(MyAlpha<=0.0f) { AlphaMax=false;MyAlpha=-MyAlpha; }
  }
 }
}
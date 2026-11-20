// 00ddc7d0 CEntityScriptBinding<NScript::CQ_PrisonRaceScript>::CreateActiveEntityScript
// Flags: none
// Leaf: ?CreateActiveEntityScript@CEntityScriptBinding@EntityScriptBinding_00ddc7d0@@QAE?AV?$CCountedPointer@UCActiveEntityScriptBase@EntityScriptBinding_00ddc7d0@@@@PAVCScriptThing@@II@Z
#include "fable_compiler.h"
#include <new>
// CCountedPointer<T> / CCPPointerInfo: the retail model from engine/CCountedPointer.h (Naming NF194, 2026-11-20, q5992: replaces the
//   row's global and namespace-local CCountedPointer / CCPPointerInfo models, so ShareData 0x00CE1040 binds the PDB spelling).
//   Per-TU opt-ins: (T*) ctor 0x00CE10B0 and ShareData 0x00CE1040 called out of line (declaration only); dtor 0x00CE1000
//   called but visible (noinline), as the row's models had them.
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace EntityScriptBinding_00ddc7d0 {
// Layout: donor CActiveEntityScriptBase size 0x58; PEntityScript +0x34, Flags +0x3c, UID +0x40, Entity +0x48.
struct CActiveEntityScriptBase {
 virtual ~CActiveEntityScriptBase();
 unsigned char State[0x30];
 CCountedPointer<CEntityScriptBase> PEntityScript;
 unsigned Flags;
 unsigned UIDLow,UIDHigh;
 CScriptThing Entity;
 unsigned Last;
 CActiveEntityScriptBase(CScriptThing*,unsigned,unsigned,unsigned);
};
// Layout: partial NScript::CQ_PrisonRaceScript view; only observed script-data pointer +0x44 is used. ScriptData is a descriptive member label.
namespace NScript { struct CQ_PrisonRaceScript { unsigned char State[0x44]; void* ScriptData; }; }
typedef NScript::CQ_PrisonRaceScript ParentScript;
// Layout: factory retail reads PParentScript +8, Func +0xc, ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18.
struct CActiveEntityScript: CActiveEntityScriptBase {
 void* Func; ParentScript* PParentScript;
 virtual ~CActiveEntityScript();
 FABLE_FORCEINLINE CActiveEntityScript(CScriptThing* entity,unsigned lo,unsigned hi,unsigned flags,ParentScript* script,void* fn):CActiveEntityScriptBase(entity,lo,hi,flags),Func(fn),PParentScript(script) {}
 FABLE_FORCEINLINE CActiveEntityScript(CScriptThing* entity,unsigned lo,unsigned hi,unsigned flags,ParentScript* script,const CCountedPointer<CEntityScriptBase>& object):CActiveEntityScriptBase(entity,lo,hi,flags),PParentScript(script) {PEntityScript.ShareData(object.Data,object.Info);}
};
struct CEntityScriptBinding {
 void* VTable; void* EntityScriptName; ParentScript* PParentScript; void* Func;
 typedef CCountedPointer<CEntityScriptBase> (__fastcall *Allocator)(ParentScript*,void*,CScriptThing*);
 Allocator ClassAllocFunc; bool ClassAllocValid; unsigned char Pad[3]; unsigned Flags;
 CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(CScriptThing*,unsigned,unsigned);
};
CCountedPointer<CActiveEntityScriptBase> CEntityScriptBinding::CreateActiveEntityScript(CScriptThing* entity,unsigned lo,unsigned hi) {
 if(ClassAllocValid) {
  CCountedPointer<CEntityScriptBase> object(ClassAllocFunc(PParentScript,PParentScript->ScriptData,entity));
  object.Data->Init();
  return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript(entity,lo,hi,Flags,PParentScript,object));
 }
 return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript(entity,lo,hi,Flags,PParentScript,Func));
}
}
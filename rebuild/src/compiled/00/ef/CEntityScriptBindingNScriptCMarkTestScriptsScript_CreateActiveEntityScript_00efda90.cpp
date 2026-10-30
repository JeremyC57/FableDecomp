// 00efda90 CEntityScriptBinding<NScript::CMarkTestScriptsScript>::CreateActiveEntityScript
// Flags: none
// Leaf: ?CreateActiveEntityScript@CEntityScriptBinding
#include <new>
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace EntityScriptBinding_00efda90 {
struct CCPPointerInfo { unsigned RefCount; void (__fastcall *DeleteFunc)(void*); void* Data; };
template<class T> struct CCountedPointer {
 T* Data; CCPPointerInfo* Info;
 CCountedPointer(T* p);
 __forceinline CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info) {if(Info) ++Info->RefCount;}
 __declspec(noinline) ~CCountedPointer() {if(Info){if(--Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}} Data=0;Info=0;}
 void ShareData(T*,CCPPointerInfo*);
};
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
// Layout: partial NScript::CMarkTestScriptsScript view; only observed script-data pointer +0x44 is used. ScriptData is a descriptive member label.
namespace NScript { struct CMarkTestScriptsScript { unsigned char State[0x44]; void* ScriptData; }; }
typedef NScript::CMarkTestScriptsScript ParentScript;
// Layout: factory retail reads PParentScript +8, Func +0xc, ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18.
struct CActiveEntityScript: CActiveEntityScriptBase {
 void* Func; ParentScript* PParentScript;
 virtual ~CActiveEntityScript();
 __forceinline CActiveEntityScript(CScriptThing* entity,unsigned lo,unsigned hi,unsigned flags,ParentScript* script,void* fn):CActiveEntityScriptBase(entity,lo,hi,flags),Func(fn),PParentScript(script) {}
 __forceinline CActiveEntityScript(CScriptThing* entity,unsigned lo,unsigned hi,unsigned flags,ParentScript* script,const CCountedPointer<CEntityScriptBase>& object):CActiveEntityScriptBase(entity,lo,hi,flags),PParentScript(script) {PEntityScript.ShareData(object.Data,object.Info);}
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
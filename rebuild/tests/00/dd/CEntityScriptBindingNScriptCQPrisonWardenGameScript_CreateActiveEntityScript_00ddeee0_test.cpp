// 00ddeee0 CEntityScriptBinding<NScript::CQ_PrisonWardenGameScript>::CreateActiveEntityScript
// Flags: none
// Leaf: ?CreateActiveEntityScript@CEntityScriptBinding
#include <new>
#include "engine/CScriptThing.h"
namespace EntityScriptBinding_00ddeee0 {
struct CCPPointerInfo { unsigned RefCount; void (__fastcall *DeleteFunc)(void*); void* Data; };
struct CEntityScriptBase { virtual ~CEntityScriptBase(); virtual void Stop(); virtual void Start(); };
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
// Layout: partial NScript::CQ_PrisonWardenGameScript view; only observed script-data pointer +0x44 is used. ScriptData is a descriptive member label.
namespace NScript { struct CQ_PrisonWardenGameScript { unsigned char State[0x44]; void* ScriptData; }; }
typedef NScript::CQ_PrisonWardenGameScript ParentScript;
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
  object.Data->Start();
  return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript(entity,lo,hi,Flags,PParentScript,object));
 }
 return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript(entity,lo,hi,Flags,PParentScript,Func));
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace EntityScriptBinding_00ddeee0 {
static int Started,EntityDestroyed,ActiveDestroyed;
static ParentScript* ExpectedParent;
static CScriptThing* ExpectedEntity;
CEntityScriptBase::~CEntityScriptBase(){++EntityDestroyed;} void CEntityScriptBase::Stop(){} void CEntityScriptBase::Start(){++Started;}
CActiveEntityScriptBase::~CActiveEntityScriptBase(){} CActiveEntityScript::~CActiveEntityScript(){++ActiveDestroyed;}
template<class T> void __fastcall DestroyObject(void* p){delete (T*)p;}
template<class T> CCountedPointer<T>::CCountedPointer(T* p):Data(p),Info(p?new CCPPointerInfo:0){if(Info){Info->RefCount=1;Info->DeleteFunc=&DestroyObject<T>;Info->Data=p;}}
template<class T> void CCountedPointer<T>::ShareData(T* data,CCPPointerInfo* info){if(info!=Info){if(Info&&--Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}Data=data;Info=info;if(Info)++Info->RefCount;}}
CActiveEntityScriptBase::CActiveEntityScriptBase(CScriptThing* entity,unsigned lo,unsigned hi,unsigned flags):PEntityScript((CEntityScriptBase*)0),Flags(flags),UIDLow(lo),UIDHigh(hi),Entity(*entity),Last(0){}
static CCountedPointer<CEntityScriptBase> __fastcall Spawn(ParentScript* parent,void* data,CScriptThing* entity){assert(parent==ExpectedParent);assert(data==(void*)16);assert(entity==ExpectedEntity);return CCountedPointer<CEntityScriptBase>(new CEntityScriptBase);}
}
int main(){using namespace EntityScriptBinding_00ddeee0;ParentScript script;memset(&script,0,sizeof(script));script.ScriptData=(void*)16;CScriptThing entity;memset(&entity,0,sizeof(entity));ExpectedParent=&script;ExpectedEntity=&entity;CEntityScriptBinding binding;memset(&binding,0,sizeof(binding));binding.PParentScript=&script;binding.Flags=7;binding.Func=(void*)32;binding.ClassAllocFunc=&Spawn;{CCountedPointer<CActiveEntityScriptBase> result=binding.CreateActiveEntityScript(&entity,4,8);CActiveEntityScript* active=(CActiveEntityScript*)result.Data;assert(result.Info->RefCount==1);assert(active->Flags==7&&active->UIDLow==4&&active->UIDHigh==8&&active->Func==(void*)32&&active->PParentScript==&script);assert(!active->PEntityScript.Data);assert(ActiveDestroyed==0&&EntityDestroyed==0);}assert(ActiveDestroyed==1&&EntityDestroyed==0);binding.ClassAllocValid=true;{CCountedPointer<CActiveEntityScriptBase> result=binding.CreateActiveEntityScript(&entity,16,32);CActiveEntityScript* active=(CActiveEntityScript*)result.Data;assert(result.Info->RefCount==1);assert(Started==1&&active->PEntityScript.Data&&active->PEntityScript.Info->RefCount==1&&active->PParentScript==&script&&active->Flags==7&&active->UIDLow==16&&active->UIDHigh==32);assert(ActiveDestroyed==1&&EntityDestroyed==0);}assert(ActiveDestroyed==2&&EntityDestroyed==1);puts("00ddeee0_TEST PASS");return 0;}
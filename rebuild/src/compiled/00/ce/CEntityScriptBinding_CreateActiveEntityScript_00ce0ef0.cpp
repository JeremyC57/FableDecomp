// 00ce0ef0 CEntityScriptBinding<NScript::CPersonalScript_GlobalThingsScript>::CreateActiveEntityScript
// Flags: /O2 /Oy /W3 
// Leaf: ?CreateActiveEntityScript@?$CEntityScriptBinding@UCPersonalScript_GlobalThingsScript@NScript@ExternalRetry_00ce0ef0@@@ExternalRetry_00ce0ef0@@QAE?AV?$CCountedPointer@UCActiveEntityScriptBase@ExternalRetry_00ce0ef0@@@@PAXII@Z
#include "fable_compiler.h"
// CCountedPointer<T> / CCPPointerInfo / CScriptThing / CEntityScriptBase: the retail models from engine/CCountedPointer.h,
//   engine/CScriptThing.h and engine/CEntityScriptBase.h (Naming NF194, 2026-11-20, q5992: replace the row's namespace-local
//   CEntityScriptBase / CCPPointerInfo / CCountedPointer specializations, so ShareData 0x00CE1040 binds the PDB spelling).
//   Per-TU opt-ins: (T*) ctor 0x00CE10B0 and ShareData 0x00CE1040 called out of line (declaration only); dtor 0x00CE1000 called.
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
namespace ExternalRetry_00ce0ef0 {
typedef ::CCountedPointer< ::CEntityScriptBase> Ptr;
typedef Ptr CCountedPointerShare;
// Layout: ce1110 constructs CActiveEntityScriptBase: Share34, Flags3c, UID40, Entity48.
// Layout: pack4 preserves first member4; Entity interval48..58 includes unknown tail54..58.
// Layout: donor base88 and retail derived Func58/PParentScript5c establish the88-byte base extent.
#pragma pack(push,4)
struct CActiveEntityScriptBase{virtual ~CActiveEntityScriptBase();virtual void v1();virtual void v2();virtual void v3();virtual void Step();virtual void v5();virtual void v6();char pad[0x30];CCountedPointerShare share;unsigned flags;unsigned __int64 uid;char entity[16];CActiveEntityScriptBase(void*,unsigned,unsigned,unsigned);};

#pragma pack(pop)
struct CActiveEntityScript:CActiveEntityScriptBase{void*Func;void*PParentScript;virtual void Step();FABLE_FORCEINLINE CActiveEntityScript(void*e,unsigned l,unsigned h,unsigned k,void*o,const Ptr&s):CActiveEntityScriptBase(e,l,h,k),PParentScript(o){share.ShareData(s.Data,s.Info);}FABLE_FORCEINLINE CActiveEntityScript(void*e,unsigned l,unsigned h,unsigned k,void*o,void*c):CActiveEntityScriptBase(e,l,h,k),Func(c),PParentScript(o){}};
typedef ::CCountedPointer<CActiveEntityScriptBase> Result;namespace NScript {struct CPersonalScript_GlobalThingsScript {char state[0x44];void* ScriptData;};}
typedef NScript::CPersonalScript_GlobalThingsScript ParentScript;
typedef Ptr(__fastcall*FactoryFn)(ParentScript*,void*,void*);
// Layout: factory header base+0..c, Func+10,flag+14,kind+18. CActiveEntityScript allocation60 with share+34 Func+58 PParentScript+5c.
// Layout: CEntityScriptBinding<NScript::CPersonalScript_GlobalThingsScript>, donor struct_layouts_egor.tsv: Name4, PParentScript8, Func+c, ClassAllocFunc10, ClassAllocValid14, Flags18.
// Layout: cde4a4 registers TeleportPoint through table12c31f0 containing this method. cde457 registers another GlobalThings entity using the same table.
// Callback returns a counted object through hidden ECX; ParentScript is the second fastcall argument in EDX. ScriptData and Entity follow on the stack.
template<class T> struct CEntityScriptBinding {void* VTable;void* EntityScriptName;T* PParentScript;void* Func;FactoryFn ClassAllocFunc;bool ClassAllocValid;char gap[3];unsigned Flags;Result CreateActiveEntityScript(void*,unsigned,unsigned);};
template<class T> Result CEntityScriptBinding<T>::CreateActiveEntityScript(void*entity,unsigned low,unsigned high){if(ClassAllocValid){Ptr source=ClassAllocFunc(PParentScript,PParentScript->ScriptData,entity);source.Data->Init();CActiveEntityScript*p=new CActiveEntityScript(entity,low,high,Flags,PParentScript,source);return Result(p);}CActiveEntityScript*p=new CActiveEntityScript(entity,low,high,Flags,PParentScript,Func);return Result(p);}
template struct CEntityScriptBinding<ParentScript>;
}
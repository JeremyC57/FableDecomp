// 00ce0ef0 CEntityScriptBinding<NScript::CPersonalScript_GlobalThingsScript>::CreateActiveEntityScript
// Flags: /O2 /Oy /W3 
// Leaf: ?CreateActiveEntityScript@?$CEntityScriptBinding@UCPersonalScript_GlobalThingsScript@NScript@ExternalRetry_00ce0ef0@@@ExternalRetry_00ce0ef0@@QAE?AU?$CCountedPointer@UCActiveEntityScriptBase@ExternalRetry_00ce0ef0@@@2@PAXII@Z
namespace ExternalRetry_00ce0ef0 {
struct CEntityScriptBase{virtual ~CEntityScriptBase();virtual void B()=0;virtual void Init()=0;};struct CCPPointerInfo{int RefCount;void(__fastcall* DeleteFunc)(void*);void* Data;};template<class T>struct CCountedPointer;
template<>struct CCountedPointer<CEntityScriptBase>{CEntityScriptBase*object;CCPPointerInfo*control;__forceinline CCountedPointer():object(0),control(0){}__forceinline CCountedPointer(const CCountedPointer&p):object(p.object),control(p.control){if(control)++control->RefCount;}CCountedPointer(CEntityScriptBase*,CCPPointerInfo*);~CCountedPointer();void ShareData(CEntityScriptBase*,CCPPointerInfo*);};typedef CCountedPointer<CEntityScriptBase> Ptr;
typedef Ptr CCountedPointerShare;
// Layout: ce1110 constructs CActiveEntityScriptBase: Share34, Flags3c, UID40, Entity48.
// Layout: pack4 preserves first member4; Entity interval48..58 includes unknown tail54..58.
// Layout: donor base88 and retail derived Func58/PParentScript5c establish the88-byte base extent.
#pragma pack(push,4)
struct CActiveEntityScriptBase{virtual ~CActiveEntityScriptBase();virtual void v1();virtual void v2();virtual void v3();virtual void Step();virtual void v5();virtual void v6();char pad[0x30];CCountedPointerShare share;unsigned flags;unsigned __int64 uid;char entity[16];CActiveEntityScriptBase(void*,unsigned,unsigned,unsigned);};

#pragma pack(pop)
struct CActiveEntityScript:CActiveEntityScriptBase{void*Func;void*PParentScript;virtual void Step();__forceinline CActiveEntityScript(void*e,unsigned l,unsigned h,unsigned k,void*o,const Ptr&s):CActiveEntityScriptBase(e,l,h,k),PParentScript(o){share.ShareData(s.object,s.control);}__forceinline CActiveEntityScript(void*e,unsigned l,unsigned h,unsigned k,void*o,void*c):CActiveEntityScriptBase(e,l,h,k),Func(c),PParentScript(o){}};
template<>struct CCountedPointer<CActiveEntityScriptBase>{CActiveEntityScriptBase*object;CCPPointerInfo*control;CCountedPointer(CActiveEntityScriptBase*);CCountedPointer(const CCountedPointer&);~CCountedPointer();};typedef CCountedPointer<CActiveEntityScriptBase> Result;namespace NScript {struct CPersonalScript_GlobalThingsScript {char state[0x44];void* ScriptData;};}
typedef NScript::CPersonalScript_GlobalThingsScript ParentScript;
typedef Ptr(__fastcall*FactoryFn)(ParentScript*,void*,void*);
// Layout: factory header base+0..c, Func+10,flag+14,kind+18. CActiveEntityScript allocation60 with share+34 Func+58 PParentScript+5c.
// Layout: CEntityScriptBinding<NScript::CPersonalScript_GlobalThingsScript>, donor struct_layouts_egor.tsv: Name4, PParentScript8, Func+c, ClassAllocFunc10, ClassAllocValid14, Flags18.
// Layout: cde4a4 registers TeleportPoint through table12c31f0 containing this method. cde457 registers another GlobalThings entity using the same table.
// Callback returns a counted object through hidden ECX; ParentScript is the second fastcall argument in EDX. ScriptData and Entity follow on the stack.
template<class T> struct CEntityScriptBinding {void* VTable;void* EntityScriptName;T* PParentScript;void* Func;FactoryFn ClassAllocFunc;bool ClassAllocValid;char gap[3];unsigned Flags;Result CreateActiveEntityScript(void*,unsigned,unsigned);};
template<class T> Result CEntityScriptBinding<T>::CreateActiveEntityScript(void*entity,unsigned low,unsigned high){if(ClassAllocValid){Ptr source=ClassAllocFunc(PParentScript,PParentScript->ScriptData,entity);source.object->Init();CActiveEntityScript*p=new CActiveEntityScript(entity,low,high,Flags,PParentScript,source);return Result(p);}CActiveEntityScript*p=new CActiveEntityScript(entity,low,high,Flags,PParentScript,Func);return Result(p);}
template struct CEntityScriptBinding<ParentScript>;
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace ExternalRetry_00ce0ef0 {
static int initialized,shared,liveSources,liveControls,liveActives,baseCalls;
static void* expectedEntity;
static void Release(CCPPointerInfo* c){if(c&&!--c->RefCount){c->DeleteFunc(c->Data);delete c;--liveControls;}}
CCountedPointer<CEntityScriptBase>::CCountedPointer(CEntityScriptBase*s,CCPPointerInfo*c):object(s),control(c){if(c)++c->RefCount;}
CCountedPointer<CEntityScriptBase>::~CCountedPointer(){Release(control);object=0;control=0;}
void CCountedPointer<CEntityScriptBase>::ShareData(CEntityScriptBase*s,CCPPointerInfo*c){++shared;if(control!=c){Release(control);object=s;control=c;if(c)++c->RefCount;}}
// The external base constructor is a fixture boundary: validate its arguments and model owned CCountedPointerShare state.
CActiveEntityScriptBase::CActiveEntityScriptBase(void*e,unsigned l,unsigned h,unsigned k):flags(k),uid(((unsigned __int64)h<<32)|l){assert(e==expectedEntity);share.object=0;share.control=0;memset(pad,0,sizeof pad);memset(entity,0,sizeof entity);++baseCalls;++liveActives;}
CActiveEntityScriptBase::~CActiveEntityScriptBase(){Release(share.control);share.object=0;share.control=0;--liveActives;}
void CActiveEntityScriptBase::v1(){}void CActiveEntityScriptBase::v2(){}void CActiveEntityScriptBase::v3(){}void CActiveEntityScriptBase::Step(){}void CActiveEntityScriptBase::v5(){}void CActiveEntityScriptBase::v6(){}void CActiveEntityScript::Step(){assert(share.object||Func);}
static void __fastcall DeleteActive(void*p){delete (CActiveEntityScript*)p;}
CCountedPointer<CActiveEntityScriptBase>::CCountedPointer(CActiveEntityScriptBase*p):object(p),control(0){if(p){control=new CCPPointerInfo;control->RefCount=1;control->DeleteFunc=DeleteActive;control->Data=p;++liveControls;}}
CCountedPointer<CActiveEntityScriptBase>::CCountedPointer(const CCountedPointer&r):object(r.object),control(r.control){if(control)++control->RefCount;}
CCountedPointer<CActiveEntityScriptBase>::~CCountedPointer(){Release(control);object=0;control=0;}
CEntityScriptBase::~CEntityScriptBase(){}
struct S:CEntityScriptBase{S(){++liveSources;}~S(){--liveSources;}void A(){}void B(){}void Init(){++initialized;}};
static void __fastcall DeleteSource(void*p){delete (S*)p;}
static ParentScript* expectedParent;
Ptr __fastcall Make(ParentScript* parent,void* scriptData,void* entity){assert(parent==expectedParent&&scriptData==(void*)73&&entity==expectedEntity);S*p=new S;CCPPointerInfo*c=new CCPPointerInfo;c->RefCount=0;c->DeleteFunc=DeleteSource;c->Data=p;++liveControls;return Ptr(p,c);}
}
int main(){using namespace ExternalRetry_00ce0ef0;
 assert(sizeof(CActiveEntityScriptBase)==88&&sizeof(CActiveEntityScript)==96&&sizeof(CCPPointerInfo)==12&&sizeof(Ptr)==8);
 CEntityScriptBinding<ParentScript> f;memset(&f,0,sizeof f);ParentScript PParentScript;memset(&PParentScript,0,sizeof PParentScript);PParentScript.ScriptData=(void*)73;expectedParent=&PParentScript;f.PParentScript=&PParentScript;f.Func=&PParentScript;f.Flags=9;int entityTag=41;expectedEntity=&entityTag;
 {Result r=f.CreateActiveEntityScript(expectedEntity,0x11223344,0x55667788);assert(((CActiveEntityScript*)r.object)->PParentScript==&PParentScript&&((CActiveEntityScript*)r.object)->Func==&PParentScript&&!shared);assert(r.object->flags==9&&r.object->uid==(((unsigned __int64)0x55667788<<32)|0x11223344));assert((char*)&r.object->share-(char*)r.object==0x34&&(char*)&((CActiveEntityScript*)r.object)->Func-(char*)r.object==0x58&&(char*)&((CActiveEntityScript*)r.object)->PParentScript-(char*)r.object==0x5c);CActiveEntityScriptBase*b=r.object;b->Step();{Result copy(r);assert(r.control->RefCount==2);}assert(r.control->RefCount==1);}
 assert(liveActives==0&&liveControls==0&&liveSources==0);
 f.ClassAllocValid=true;f.ClassAllocFunc=Make;{Result r=f.CreateActiveEntityScript(expectedEntity,11,22);assert(initialized==1&&shared==1&&((CActiveEntityScript*)r.object)->PParentScript==&PParentScript&&r.object->share.control->RefCount==1);CActiveEntityScriptBase*b=r.object;b->Step();assert(liveSources==1&&liveControls==2);}
 assert(liveActives==0&&liveControls==0&&liveSources==0&&baseCalls==2);{Result empty(0);Result copy(empty);assert(!copy.object&&!copy.control);}puts("00ce0ef0_TEST PASS");return 0;}
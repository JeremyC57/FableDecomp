// CExitWardensOffice::Main. Native alive predicate VT12C32F0, operators0/4/dtor8.
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "fable_string.h"
#include <string.h>
struct CAliveScriptThing:CScriptThing {CAliveScriptThing(const CScriptThing&);}; //4ABE90 genuine copy constructor, increments counted Info refs0
struct CIsThingAlive {
 virtual bool operator()()const;virtual CIsThingAlive* Clone()const;virtual ~CIsThingAlive(); //CDF120/CDF130/E241B0
 CAliveScriptThing Thing;CIsThingAlive(const CScriptThing& thing):Thing(thing){}
};
struct CExitWardensOffice {void* Vtable;CGameScriptInterface* Script;CScriptThing Thing;void SetPredicate(const CIsThingAlive&);bool IsActiveThreadTerminating()const;void Main();}; //F35B10/F35B30
__forceinline bool WardenUsed(const CScriptThing& thing,const CCharString& name){typedef bool(CScriptThing::*F)(const CCharString&)const;return (thing.*static_cast<const F*>(thing.__vftable)[20])(name);}
__forceinline void WardenFrame(CGameScriptInterface* s){typedef void(CGameScriptInterface::*F)()const;F f=0;memcpy(&f,static_cast<const F*>(s->__vftable)+7,sizeof(f));(s->*f)();}
__forceinline void WardenInfo(CGameScriptInterface* s,const CCharString& name){typedef void(CGameScriptInterface::*F)(const CCharString&)const;F f=0;memcpy(&f,static_cast<const F*>(s->__vftable)+115,sizeof(f));(s->*f)(name);}
__forceinline bool WardenClicked(CGameScriptInterface* s){typedef bool(CGameScriptInterface::*F)()const;return (s->*static_cast<const F*>(s->__vftable)[40])();}
#ifdef _MSC_VER
typedef char Alive16[sizeof(CIsThingAlive)==16?1:-1];
typedef char Thing8[offsetof(CExitWardensOffice,Thing)==8?1:-1];
#endif
void CExitWardensOffice::Main(){
 {CIsThingAlive alive(Thing);SetPredicate(alive);}
 WardenFrame(Script);if(IsActiveThreadTerminating())return;
 while(!IsActiveThreadTerminating()){
 bool used;{CCharString name("SCRIPT_NAME_HERO",-1);used=WardenUsed(Thing,name);}
 if(used){
 if(IsActiveThreadTerminating())return;
 {CCharString name("TEXT_QST_055_DIARY_LOCKED",-1);WardenInfo(Script,name);}
 while(!WardenClicked(Script)){WardenFrame(Script);if(IsActiveThreadTerminating())return;}
 if(IsActiveThreadTerminating())return;
 }
 WardenFrame(Script);
 }
}
// NScript::CPersonalScript_alextestScript::CSignpost::Main() -- CSignpost::Main. Native alive predicate VT12C32F0, operators0/4/dtor8.
// Leaf: ?Main@CSignpost@CPersonalScript_alextestScript@NScript@@EAEXXZ
// Naming wave 35: identity reg 00eed40e "Signpost" in 00eed360; Alloc 00eed5d0; vtable 012f5890 slot 1; PDB ?Main@CSignpost@CPersonalScript_alextestScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was CVtable_012f5890::vfunc_1 (wave3/labels_alloc.tsv); PDB ?Main@CSignpost@CPersonalScript_alextestScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CExitWardensOffice@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CExitWardensOffice renamed CSignpost); same bytes.
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "fable_string.h"
#include <string.h>
struct CAliveScriptThing:CScriptThing {CAliveScriptThing(const CScriptThing&);}; //4ABE90 genuine copy constructor, increments counted Info refs0
namespace NScript {
class CIsThingAlive { public:
 virtual bool operator()()const;virtual CIsThingAlive* Clone()const;virtual ~CIsThingAlive(); //CDF120/CDF130/E241B0
 CAliveScriptThing Thing;CIsThingAlive(const CScriptThing& thing):Thing(thing){}
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
};
namespace NScript {
class CPersonalScript_alextestScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CSignpost;
};
}
using NScript::CPersonalScript_alextestScript;
namespace NScript {
class CPersonalScript_alextestScript::CSignpost {
public:CGameScriptInterface* Script;CScriptThing Thing;void SetPredicate(const CIsThingAlive&);
private:
    virtual void Main();                          // PDB ?Main@CSignpost@CPersonalScript_alextestScript@NScript@@EAEXXZ
};
}
typedef NScript::CPersonalScript_alextestScript::CSignpost CSignpost;    // global spelling used below //F35B10/F35B30
__forceinline bool WardenUsed(const CScriptThing& thing,const CCharString& name){typedef bool(CScriptThing::*F)(const CCharString&)const;return (thing.*static_cast<const F*>(thing.__vftable)[20])(name);}
__forceinline void WardenFrame(CGameScriptInterface* s){typedef void(CGameScriptInterface::*F)()const;F f=0;memcpy(&f,static_cast<const F*>(s->__vftable)+7,sizeof(f));(s->*f)();}
__forceinline void WardenInfo(CGameScriptInterface* s,const CCharString& name){typedef void(CGameScriptInterface::*F)(const CCharString&)const;F f=0;memcpy(&f,static_cast<const F*>(s->__vftable)+115,sizeof(f));(s->*f)(name);}
__forceinline bool WardenClicked(CGameScriptInterface* s){typedef bool(CGameScriptInterface::*F)()const;return (s->*static_cast<const F*>(s->__vftable)[40])();}
#ifdef _MSC_VER
typedef char Alive16[sizeof(CIsThingAlive)==16?1:-1];
typedef char Thing8[offsetof(CSignpost,Thing)==8?1:-1];
#endif
void NScript::CPersonalScript_alextestScript::CSignpost::Main(){
 {CIsThingAlive alive(Thing);SetPredicate(alive);}
 WardenFrame(Script);if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;
 while(!((CEntityScriptBase*)this)->IsActiveThreadTerminating()){
 bool used;{CCharString name("SCRIPT_NAME_HERO",-1);used=WardenUsed(Thing,name);}
 if(used){
 if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;
 {CCharString name("TEXT_QST_055_DIARY_LOCKED",-1);WardenInfo(Script,name);}
 while(!WardenClicked(Script)){WardenFrame(Script);if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;}
 if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;
 }
 WardenFrame(Script);
 }
}
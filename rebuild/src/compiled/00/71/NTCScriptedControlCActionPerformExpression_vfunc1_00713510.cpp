// 00713510 NTCScriptedControl::CActionPerformExpression::Update, PDB00727660.
// Layout: shared script header flattens virtual methods; restore its12byte polymorphic view.
// PerformExpression PDBd4 size / def+b4 is retailc8 / def+a8, base shrinks0xc.
#include "engine/CScriptThing.h"
#include "fable_string.h"
#include "engine/CCreatureAction_PerformExpression.h"
struct Thing;struct ActionBase {unsigned char Data[0xa8];~ActionBase();}; //00693ef0 CCreatureActionBase
struct Creature {bool SetCurrentAction(const ActionBase&);bool IsPerformingAnyAction()const;}; //006644f0,006622a0
struct RefCount {long References;void DecRef(){--References;}long GetRefCount()const{return References;}};
struct DefVirtual {virtual void Slot0()=0;virtual void Delete()=0;};
struct DefObject:DefVirtual,RefCount {void Release(){DecRef();if(GetRefCount()==0)Delete();}};
struct DefPointer {DefObject* Object;DefPointer():Object(0){}__forceinline ~DefPointer(){if(Object){Object->Release();Object=0;}}};
struct IntelligentPointer {unsigned char Data[8];~IntelligentPointer();}; //00a01c10
struct DefinitionManager {bool GetDef(const CCharString&,DefPointer*);}; //00593666 typedtemplate GetDef
DefinitionManager* __fastcall GetDefinitionManager(); //0044c6b0 globalsingleton013b879c
struct ScriptThing {
virtual void Slot000();
virtual void Slot001();
virtual void Slot002();
virtual void Slot003();
virtual void Slot004();
virtual void Slot005();
virtual void Slot006();
virtual void Slot007();
virtual void Slot008();
virtual void Slot009();
virtual void Slot010();
virtual Thing* GetThing();
virtual void Slot012();
virtual void Slot013();
virtual void Slot014();
virtual void Slot015();
virtual void Slot016();
virtual void Slot017();
virtual void Slot018();
virtual void Slot019();
virtual void Slot020();
virtual void Slot021();
virtual void Slot022();
virtual void Slot023();
virtual void Slot024();
virtual void Slot025();
virtual void Slot026();
virtual void Slot027();
virtual void Slot028();
virtual void Slot029();
virtual void Slot030();
virtual void Slot031();
virtual void Slot032();
virtual void Slot033();
virtual void Slot034();
virtual void Slot035();
virtual void Slot036();
virtual void Slot037();
virtual void Slot038();
virtual void Slot039();
virtual void Slot040();
virtual void Slot041();
virtual void Slot042();
virtual void Slot043();
virtual void Slot044();
virtual void Slot045();
virtual void Slot046();
virtual void Slot047();
virtual void Slot048();
virtual void Slot049();
virtual void Slot050();
virtual void Slot051();
virtual void Slot052();
virtual void Slot053();
virtual void Slot054();
virtual void Slot055();
virtual void Slot056();
virtual void Slot057();
virtual void Slot058();
virtual void Slot059();
virtual void Slot060();
virtual void Slot061();
virtual void Slot062();
virtual void Slot063();
virtual void Slot064();
virtual void Slot065();
virtual void Slot066();
virtual void Slot067();
virtual void Slot068();
virtual void Slot069();
virtual void Slot070();
virtual void Resolve();
virtual void Slot072();
virtual void Slot073();
virtual void Slot074();
virtual void Slot075();
virtual void Slot076();
virtual bool IsResolved()const;
void* Data;void* Info;};
namespace Retail {
struct PerformExpression:ActionBase {DefPointer Expression;IntelligentPointer Target;unsigned char Tail[0xc8-0xb4];
 __forceinline ~PerformExpression(){}
 PerformExpression(Creature&,Thing*,const DefPointer&,long,long,long,long); //008430b0
};
}
struct Controller {void* Vtable;Creature* Owner;};
namespace NTCScriptedControl {
struct CActionPerformExpression {void* Vtable;Controller* Control;bool Active,Complete;unsigned char Pad[2];ScriptThing Target;bool Started;unsigned char Pad2[3];CCharString Expression;long ExtraData1,ExtraData2;void Update();};
void CActionPerformExpression::Update(){
 if(!Target.IsResolved())Target.Resolve();
 if(!Started){DefPointer definition;GetDefinitionManager()->GetDef(Expression,&definition);
 if(definition.Object){Creature& owner=*Control->Owner;owner.SetCurrentAction(Retail::PerformExpression(owner,Target.GetThing(),definition,0,0,ExtraData1,ExtraData2));Started=true;}
 else Complete=true;
 }else if(!Control->Owner->IsPerformingAnyAction())Complete=true;
}
}
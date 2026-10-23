// BEGIN exact source snapshot SHA256 8de24ed9a8499ce4808f586a5bfc3a65d39a275e3dadeaf1a9146800fa49ed0b
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

// END exact source snapshot
#include <stdio.h>
CCharString::CCharString():storage_(0){}CCharString::~CCharString(){}
static bool found=true,performing,resolved;static int resolves,sets,baseDtors,ipDtors,deleted;
struct TestDef:DefObject {void Slot0(){}void Delete(){++deleted;}}obj;
DefinitionManager* __fastcall GetDefinitionManager(){static DefinitionManager m;return &m;}
bool DefinitionManager::GetDef(const CCharString&,DefPointer* p){if(found){p->Object=&obj;++obj.References;}return found;}
bool Creature::SetCurrentAction(const ActionBase&){++sets;return true;}bool Creature::IsPerformingAnyAction()const{return performing;}
ActionBase::~ActionBase(){++baseDtors;}IntelligentPointer::~IntelligentPointer(){++ipDtors;}
Retail::PerformExpression::PerformExpression(Creature&,Thing*,const DefPointer& d,long,long,long,long){Expression.Object=d.Object;++d.Object->References;}
void ScriptThing::Slot000(){}
void ScriptThing::Slot001(){}
void ScriptThing::Slot002(){}
void ScriptThing::Slot003(){}
void ScriptThing::Slot004(){}
void ScriptThing::Slot005(){}
void ScriptThing::Slot006(){}
void ScriptThing::Slot007(){}
void ScriptThing::Slot008(){}
void ScriptThing::Slot009(){}
void ScriptThing::Slot010(){}
void ScriptThing::Slot012(){}
void ScriptThing::Slot013(){}
void ScriptThing::Slot014(){}
void ScriptThing::Slot015(){}
void ScriptThing::Slot016(){}
void ScriptThing::Slot017(){}
void ScriptThing::Slot018(){}
void ScriptThing::Slot019(){}
void ScriptThing::Slot020(){}
void ScriptThing::Slot021(){}
void ScriptThing::Slot022(){}
void ScriptThing::Slot023(){}
void ScriptThing::Slot024(){}
void ScriptThing::Slot025(){}
void ScriptThing::Slot026(){}
void ScriptThing::Slot027(){}
void ScriptThing::Slot028(){}
void ScriptThing::Slot029(){}
void ScriptThing::Slot030(){}
void ScriptThing::Slot031(){}
void ScriptThing::Slot032(){}
void ScriptThing::Slot033(){}
void ScriptThing::Slot034(){}
void ScriptThing::Slot035(){}
void ScriptThing::Slot036(){}
void ScriptThing::Slot037(){}
void ScriptThing::Slot038(){}
void ScriptThing::Slot039(){}
void ScriptThing::Slot040(){}
void ScriptThing::Slot041(){}
void ScriptThing::Slot042(){}
void ScriptThing::Slot043(){}
void ScriptThing::Slot044(){}
void ScriptThing::Slot045(){}
void ScriptThing::Slot046(){}
void ScriptThing::Slot047(){}
void ScriptThing::Slot048(){}
void ScriptThing::Slot049(){}
void ScriptThing::Slot050(){}
void ScriptThing::Slot051(){}
void ScriptThing::Slot052(){}
void ScriptThing::Slot053(){}
void ScriptThing::Slot054(){}
void ScriptThing::Slot055(){}
void ScriptThing::Slot056(){}
void ScriptThing::Slot057(){}
void ScriptThing::Slot058(){}
void ScriptThing::Slot059(){}
void ScriptThing::Slot060(){}
void ScriptThing::Slot061(){}
void ScriptThing::Slot062(){}
void ScriptThing::Slot063(){}
void ScriptThing::Slot064(){}
void ScriptThing::Slot065(){}
void ScriptThing::Slot066(){}
void ScriptThing::Slot067(){}
void ScriptThing::Slot068(){}
void ScriptThing::Slot069(){}
void ScriptThing::Slot070(){}
void ScriptThing::Slot072(){}
void ScriptThing::Slot073(){}
void ScriptThing::Slot074(){}
void ScriptThing::Slot075(){}
void ScriptThing::Slot076(){}
Thing* ScriptThing::GetThing(){return 0;}void ScriptThing::Resolve(){resolved=true;++resolves;}bool ScriptThing::IsResolved()const{return resolved;}
int main(){NTCScriptedControl::CActionPerformExpression a;Controller c;Creature owner;c.Owner=&owner;a.Control=&c;a.Started=a.Complete=false;a.ExtraData1=2;a.ExtraData2=3;obj.References=0;
 a.Update();if(!a.Started||a.Complete||sets!=1||baseDtors!=1||ipDtors!=1||deleted!=1||resolves!=1)return 1;
 performing=true;a.Update();if(a.Complete)return 2;performing=false;a.Update();if(!a.Complete)return 3;
 a.Started=a.Complete=false;found=false;a.Update();if(a.Started||!a.Complete||sets!=1)return 4;
 puts("00713510_TEST PASS");return 0;}
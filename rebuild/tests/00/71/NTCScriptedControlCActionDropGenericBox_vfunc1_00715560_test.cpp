// BEGIN exact source snapshot SHA256 65f98b423ded7fe1fe8abbb65a346a17a0dc79fa7aeb6bc44661b5d0e89e069a
// 00715560 NTCScriptedControl::CActionDropGenericBox::Update.
// Layout: donor action size b4 contracts to retail a8 (temporary spans esp+10..b7).
// Component presence is a retail precondition on the failure path: absent interfaces leave the out-local unset.
#include "engine/CCreatureAction_DropGenericBox.h"
struct Creature;
namespace Retail {struct ActionBase {unsigned char Data[0xa8];~ActionBase();long GetActionID()const;};struct DropGenericBox:ActionBase {DropGenericBox(Creature&);};}
struct Thing;struct Component {};class CTCCarrying:public Component { public:Thing* RemoveThingFromPrimarySlot(bool);};class CTCCreatureModeManager:public Component { public:void RemoveMode(unsigned long);};
struct Pair {int Key;Component* Value;};
struct ComponentMap {Pair* First;Pair* Last;Pair* Limit;Pair* LowerBound(const int&);__forceinline void Get(int key,Component** result){int query=key;Pair* i=LowerBound(query);*result=(i==Last||key<i->Key?Last:i)->Value;}};
struct Creature {unsigned char ToFlags[0x24];unsigned int Flags24,Flags28;unsigned char ToMap[0x44-0x2c];ComponentMap Components;unsigned char ToAction[0x100-0x50];Retail::ActionBase* CurrentAction;bool IsPerformingAnyAction()const;bool SetCurrentAction(const Retail::ActionBase&);
 template<class T> __forceinline bool GetInterface(T** result,long key){if((key==0x46?(Flags28&0x40):(Flags24&0x20000))){Component* component;Components.Get(key,&component);*result=(T*)component;return true;}return false;}
};
struct Controller {void* Vtable;Creature* Owner;};
namespace NTCScriptedControl {
struct CActionDropGenericBox {void* Vtable;Controller* Control;bool Active,Complete;unsigned char Pad[2];long ActionID;bool Started;void Update();};
void CActionDropGenericBox::Update(){
 if(Started){if(!Control->Owner->IsPerformingAnyAction()||Control->Owner->CurrentAction->GetActionID()!=ActionID)Complete=true;}
 else {Creature& owner=*Control->Owner;if(!owner.SetCurrentAction(Retail::DropGenericBox(owner))){CTCCarrying* carrying;Control->Owner->GetInterface(&carrying,0x46);carrying->RemoveThingFromPrimarySlot(false);CTCCreatureModeManager* mode;Control->Owner->GetInterface(&mode,0x31);mode->RemoveMode(0x22);Complete=true;}
 else {Started=true;ActionID=Control->Owner->CurrentAction->GetActionID();}}
}
}

// END exact source snapshot
#include <stdio.h>
static bool accepted=true,performing=true;static long id=7;static int made,destroyed,cleared,removed;static Creature* expected;
Retail::ActionBase::~ActionBase(){++destroyed;}long Retail::ActionBase::GetActionID()const{return id;}
Retail::DropGenericBox::DropGenericBox(Creature& c){if(&c==expected)++made;}
bool Creature::SetCurrentAction(const Retail::ActionBase&){return accepted;}bool Creature::IsPerformingAnyAction()const{return performing;}
Pair* ComponentMap::LowerBound(const int& key){Pair* p=First;while(p!=Last&&p->Key<key)++p;return p;}
Thing* CTCCarrying::RemoveThingFromPrimarySlot(bool b){if(!b)++cleared;return 0;}void CTCCreatureModeManager::RemoveMode(unsigned long n){if(n==0x22)++removed;}
int main(){Creature c={0};expected=&c;Controller ctl={0,&c};Retail::ActionBase a;c.CurrentAction=&a;CTCCreatureModeManager mode;CTCCarrying box;Pair entries[2]={{0x31,&mode},{0x46,&box}};c.Components.First=entries;c.Components.Last=entries+2;c.Flags24=0x20000;c.Flags28=0x40;
NTCScriptedControl::CActionDropGenericBox s={0};s.Control=&ctl;s.Update();if(!s.Started||s.ActionID!=7||s.Complete||made!=1||destroyed!=1)return 1;s.Update();if(s.Complete)return 2;++id;s.Update();if(!s.Complete)return 3;s.Started=false;s.Complete=false;accepted=false;s.Update();if(!s.Complete||cleared!=1||removed!=1||destroyed!=2)return 4;puts("00715560_TEST PASS");return 0;}
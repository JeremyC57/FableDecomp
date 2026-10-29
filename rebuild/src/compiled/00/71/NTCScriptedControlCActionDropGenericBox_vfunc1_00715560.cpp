// 00715560 NTCScriptedControl::CActionDropGenericBox::Update.
// Layout: donor action size b4 contracts to retail a8 (temporary spans esp+10..b7).
// Component presence is a retail precondition on the failure path: absent interfaces leave the out-local unset.
#include "engine/CCreatureAction_DropGenericBox.h"
struct Creature;
namespace Retail {struct ActionBase {unsigned char Data[0xa8];~ActionBase();long GetActionID()const;};struct DropGenericBox:ActionBase {DropGenericBox(Creature&);};}
struct Thing;struct Component {};class CTCCarrying:public Component { public:Thing* RemoveThingFromPrimarySlot(bool);};class CTCCreatureModeManager:public Component { public:void RemoveMode(unsigned long);};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> Pair;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:Pair* First;Pair* Last;Pair* Limit;Pair* LowerBound(const ETCInterfaceType& key);__forceinline void Get(int key,Component** result){ETCInterfaceType query = (ETCInterfaceType)(key);Pair* i=LowerBound(query);*result=((Component*)(i==Last||key<i->first?Last:i)->second);}};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
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
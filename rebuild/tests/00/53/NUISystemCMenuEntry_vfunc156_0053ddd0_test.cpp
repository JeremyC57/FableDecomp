// BEGIN exact source snapshot SHA256 86f43f48d6cf2bce1a28a17cf91f7f07c3f90072b03b361e6f58c74de3d9d148
//0053ddd0 NUISystem::CMenuEntry::GoBackwards (inferred method label).
// Layout: secondary CObserver at+4; retailobserver20 versus donor24 (STLset shrink).
#include "engine/CObserver.h"
#include "fable_string.h"
struct RefCount {long Count;void DecRef(){--Count;}long GetRefCount()const{return Count;}};struct RefVirtual {virtual void Slot0()=0;virtual void Delete()=0;};struct Def:RefVirtual,RefCount {unsigned char BeforeFlag[0x20a-8];bool PartOfListTree;unsigned char BeforeClose[0x221-0x20b];bool PCStyle;void Release(){DecRef();if(GetRefCount()==0)Delete();}};
struct DefPointer {Def* Object;__forceinline ~DefPointer(){if(Object){Object->Release();Object=0;}}Def* operator->()const{return Object;}};

// Layout: CUIMiscThingsDef SoundBack debug1a4 ->retail19c; CUIDef flags have independent container drift.
struct MiscDef:RefVirtual,RefCount {unsigned char ToSound[0x19c-8];CCharString SoundBack;void Release(){DecRef();if(GetRefCount()==0)Delete();}};
struct MiscPointer {MiscDef* Object;__forceinline ~MiscPointer(){if(Object){Object->Release();Object=0;}}MiscDef* operator->()const{return Object;}};

#include <stdlib.h>
namespace NUISystem {
struct Component {virtual void Slot000()=0;
virtual void Slot001()=0;
virtual void Slot002()=0;
virtual void Slot003()=0;
virtual void Slot004()=0;
virtual void Slot005()=0;
virtual void Slot006()=0;
virtual void Slot007()=0;
virtual void Slot008()=0;
virtual void Slot009()=0;
virtual void Slot010()=0;
virtual void Slot011()=0;
virtual void Slot012()=0;
virtual void Slot013()=0;
virtual void Slot014()=0;
virtual void Slot015()=0;
virtual void Slot016()=0;
virtual void Slot017()=0;
virtual void Slot018()=0;
virtual void Slot019()=0;
virtual void Slot020()=0;
virtual void Slot021()=0;
virtual void Slot022()=0;
virtual void Slot023()=0;
virtual void Slot024()=0;
virtual void Slot025()=0;
virtual void Slot026()=0;
virtual void Slot027()=0;
virtual void Slot028()=0;
virtual void Slot029()=0;
virtual void Slot030()=0;
virtual void Slot031()=0;
virtual void Slot032()=0;
virtual void Slot033()=0;
virtual void Slot034()=0;
virtual void Slot035()=0;
virtual void Slot036()=0;
virtual void Slot037()=0;
virtual void Slot038()=0;
virtual void Slot039()=0;
virtual void Slot040()=0;
virtual void Slot041()=0;
virtual void Slot042()=0;
virtual void Slot043()=0;
virtual void Slot044()=0;
virtual void Slot045()=0;
virtual void Slot046()=0;
virtual void Slot047()=0;
virtual void ChangeState(unsigned long)=0;
virtual void Slot049()=0;
virtual void Slot050()=0;
virtual void Slot051()=0;
virtual void Slot052()=0;
virtual void Slot053()=0;
virtual void Slot054()=0;
virtual void Slot055()=0;
virtual void Slot056()=0;
virtual void Slot057()=0;
virtual void Slot058()=0;
virtual void Slot059()=0;
virtual void Slot060()=0;
virtual void Slot061()=0;
virtual void Slot062()=0;
virtual void Slot063()=0;
virtual void Slot064()=0;
virtual unsigned long GetType()const=0;
virtual void Slot066()=0;
virtual void Slot067()=0;
virtual void Slot068()=0;
virtual void Slot069()=0;
virtual void Slot070()=0;
virtual void Slot071()=0;
virtual void Slot072()=0;
virtual void Slot073()=0;
virtual void Slot074()=0;
virtual void Slot075()=0;
virtual void Slot076()=0;
virtual void Slot077()=0;
virtual void Slot078()=0;
virtual void Slot079()=0;
virtual void Slot080()=0;
virtual void Slot081()=0;
virtual void Slot082()=0;
virtual void Slot083()=0;
virtual void Slot084()=0;
virtual void Slot085()=0;
virtual void Slot086()=0;
virtual void Slot087()=0;
virtual void Slot088()=0;
virtual void Slot089()=0;
virtual void Slot090()=0;
virtual void Slot091()=0;
virtual void Slot092()=0;
virtual void Slot093()=0;
virtual void Slot094()=0;
virtual void Slot095()=0;
virtual void Slot096()=0;
virtual void Slot097()=0;
virtual void Slot098()=0;
virtual void Slot099()=0;
virtual void Slot100()=0;
virtual void Slot101()=0;
virtual void Slot102()=0;
virtual void Slot103()=0;
virtual void Slot104()=0;
virtual void Slot105()=0;
virtual void Slot106()=0;
virtual void Slot107()=0;
virtual DefPointer GetDef()const=0;
virtual void Slot109()=0;
virtual void Slot110()=0;
virtual void Slot111()=0;
virtual void Slot112()=0;
virtual void Slot113()=0;
virtual void Slot114()=0;
virtual void Slot115()=0;
virtual void Slot116()=0;
virtual void Slot117()=0;
virtual void Slot118()=0;
virtual void Slot119()=0;
virtual void Slot120()=0;
virtual void Slot121()=0;
virtual void Slot122()=0;
virtual void Slot123()=0;
virtual void Slot124()=0;
virtual void Slot125()=0;
virtual void Slot126()=0;
virtual void Slot127()=0;
virtual void Slot128()=0;
virtual void Slot129()=0;
virtual void Slot130()=0;
virtual void SetTargetState(unsigned long)=0;
};
struct Observer {virtual void Observer0()=0;unsigned char Data[0x10];};
struct Stack {struct Node {Node* Next;Node* Prev;Component* Object;};Node* Head;__forceinline unsigned long size()const{unsigned long n=0;for(Node*p=Head->Next;p!=Head;p=p->Next)++n;return n;}__forceinline void pop_front(){Node*p=Head->Next;Node*n=p->Next;Node*previous=p->Prev;previous->Next=n;n->Prev=previous;free(p);}Component* front()const{return Head->Next->Object;}};extern Stack MenuStack_013b8a98;
class CManager { public:
virtual void ManagerSlot00()=0;
virtual void ManagerSlot01()=0;
virtual void ManagerSlot02()=0;
virtual void ManagerSlot03()=0;
virtual void ManagerSlot04()=0;
virtual void RemoveObserver(Observer*)=0;
virtual void ManagerSlot06()=0;
virtual void ManagerSlot07()=0;
virtual void ManagerSlot08()=0;
virtual void ManagerSlot09()=0;
virtual void ManagerSlot10()=0;
virtual void ManagerSlot11()=0;
virtual void ManagerSlot12()=0;
virtual void ManagerSlot13()=0;
virtual void ManagerSlot14()=0;
virtual void ManagerSlot15()=0;
virtual void ManagerSlot16()=0;
virtual void ManagerSlot17()=0;
virtual void ManagerSlot18()=0;
virtual void ManagerSlot19()=0;
virtual void ManagerSlot20()=0;
virtual void ManagerSlot21()=0;
virtual void ManagerSlot22()=0;
virtual void ManagerSlot23()=0;
virtual void ManagerSlot24()=0;
virtual void Action25(unsigned long)=0;
virtual void ManagerSlot26()=0;
virtual void ManagerSlot27()=0;
virtual void Action28(unsigned long)=0;
virtual void ManagerSlot29()=0;
virtual void ManagerSlot30()=0;
virtual void ManagerSlot31()=0;
virtual void ManagerSlot32()=0;
virtual void ManagerSlot33()=0;
virtual void ManagerSlot34()=0;
virtual void ManagerSlot35()=0;
virtual void ManagerSlot36()=0;
virtual void ManagerSlot37()=0;
virtual void ManagerSlot38()=0;
virtual void ManagerSlot39()=0;
virtual void PlaySoundW(CCharString&,long)=0;
virtual void ManagerSlot41()=0;
virtual MiscPointer GetMiscThingsDef()=0;
static CManager* __fastcall GetInstance();};
struct ActionList {struct Node {Node* Next;Node* Prev;unsigned long Value;};Node* Head;};
class CMenuEntry:public Component,public Observer { public:unsigned char BeforeTime[0x30-0x18];unsigned long CurrentTime;unsigned char BeforeParent[0xc8-0x34];Component* Parent;unsigned char BeforeLast[0x194-0xcc];unsigned long LastTime;unsigned long Unknown198;ActionList* Actions28;ActionList* Actions25;unsigned char BeforeState[8];unsigned long TargetState;void GoBackwards();};
class CRedefinerList { public:static void __fastcall Cancel();};double __fastcall GFGetTime();extern double LastMenuTime_013b8aa0;
void CMenuEntry::GoBackwards(){if(Parent&&Parent->GetType()==8){CRedefinerList::Cancel();if(!Parent->GetDef()->PartOfListTree){SetTargetState(TargetState);LastTime=CurrentTime;return;}}
if(MenuStack_013b8a98.size()>1){CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack,1);bool immediate=GetDef()->PCStyle;if(immediate)MenuStack_013b8a98.front()->ChangeState(0);else MenuStack_013b8a98.front()->ChangeState(6);MenuStack_013b8a98.pop_front();MenuStack_013b8a98.front()->ChangeState(3);if(Actions28)for(ActionList::Node*p=Actions28->Head->Next;p!=Actions28->Head;p=p->Next)CManager::GetInstance()->Action28(p->Value);if(Actions25)for(ActionList::Node*p=Actions25->Head->Next;p!=Actions25->Head;p=p->Next)CManager::GetInstance()->Action25(p->Value);CManager::GetInstance()->RemoveObserver(this);}
SetTargetState(TargetState);LastTime=CurrentTime;LastMenuTime_013b8aa0=GFGetTime();}
}

// END exact source snapshot
#include <stdio.h>
using namespace NUISystem;
Stack NUISystem::MenuStack_013b8a98;double NUISystem::LastMenuTime_013b8aa0;static int changed,actions,removed,prepared;static unsigned long target;
struct TestMisc:MiscDef{void Slot0(){}void Delete(){}}misc;
struct TestDef:Def{void Slot0(){}void Delete(){}}def;
void __fastcall NUISystem::CRedefinerList::Cancel(){++prepared;}double __fastcall NUISystem::GFGetTime(){return 123.5;}
struct TestManager:NUISystem::CManager {
void ManagerSlot00(){}
void ManagerSlot01(){}
void ManagerSlot02(){}
void ManagerSlot03(){}
void ManagerSlot04(){}
void ManagerSlot06(){}
void ManagerSlot07(){}
void ManagerSlot08(){}
void ManagerSlot09(){}
void ManagerSlot10(){}
void ManagerSlot11(){}
void ManagerSlot12(){}
void ManagerSlot13(){}
void ManagerSlot14(){}
void ManagerSlot15(){}
void ManagerSlot16(){}
void ManagerSlot17(){}
void ManagerSlot18(){}
void ManagerSlot19(){}
void ManagerSlot20(){}
void ManagerSlot21(){}
void ManagerSlot22(){}
void ManagerSlot23(){}
void ManagerSlot24(){}
void ManagerSlot26(){}
void ManagerSlot27(){}
void ManagerSlot29(){}
void ManagerSlot30(){}
void ManagerSlot31(){}
void ManagerSlot32(){}
void ManagerSlot33(){}
void ManagerSlot34(){}
void ManagerSlot35(){}
void ManagerSlot36(){}
void ManagerSlot37(){}
void ManagerSlot38(){}
void ManagerSlot39(){}
void ManagerSlot41(){}
void RemoveObserver(Observer*){++removed;}void Action25(unsigned long){++actions;}void Action28(unsigned long){++actions;}void PlaySoundW(CCharString&,long){++actions;}MiscPointer GetMiscThingsDef(){MiscPointer p;p.Object=&misc;++misc.Count;return p;}}manager;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance(){return &manager;}
struct Entry:CMenuEntry {
void Slot000(){}
void Slot001(){}
void Slot002(){}
void Slot003(){}
void Slot004(){}
void Slot005(){}
void Slot006(){}
void Slot007(){}
void Slot008(){}
void Slot009(){}
void Slot010(){}
void Slot011(){}
void Slot012(){}
void Slot013(){}
void Slot014(){}
void Slot015(){}
void Slot016(){}
void Slot017(){}
void Slot018(){}
void Slot019(){}
void Slot020(){}
void Slot021(){}
void Slot022(){}
void Slot023(){}
void Slot024(){}
void Slot025(){}
void Slot026(){}
void Slot027(){}
void Slot028(){}
void Slot029(){}
void Slot030(){}
void Slot031(){}
void Slot032(){}
void Slot033(){}
void Slot034(){}
void Slot035(){}
void Slot036(){}
void Slot037(){}
void Slot038(){}
void Slot039(){}
void Slot040(){}
void Slot041(){}
void Slot042(){}
void Slot043(){}
void Slot044(){}
void Slot045(){}
void Slot046(){}
void Slot047(){}
void Slot049(){}
void Slot050(){}
void Slot051(){}
void Slot052(){}
void Slot053(){}
void Slot054(){}
void Slot055(){}
void Slot056(){}
void Slot057(){}
void Slot058(){}
void Slot059(){}
void Slot060(){}
void Slot061(){}
void Slot062(){}
void Slot063(){}
void Slot064(){}
void Slot066(){}
void Slot067(){}
void Slot068(){}
void Slot069(){}
void Slot070(){}
void Slot071(){}
void Slot072(){}
void Slot073(){}
void Slot074(){}
void Slot075(){}
void Slot076(){}
void Slot077(){}
void Slot078(){}
void Slot079(){}
void Slot080(){}
void Slot081(){}
void Slot082(){}
void Slot083(){}
void Slot084(){}
void Slot085(){}
void Slot086(){}
void Slot087(){}
void Slot088(){}
void Slot089(){}
void Slot090(){}
void Slot091(){}
void Slot092(){}
void Slot093(){}
void Slot094(){}
void Slot095(){}
void Slot096(){}
void Slot097(){}
void Slot098(){}
void Slot099(){}
void Slot100(){}
void Slot101(){}
void Slot102(){}
void Slot103(){}
void Slot104(){}
void Slot105(){}
void Slot106(){}
void Slot107(){}
void Slot109(){}
void Slot110(){}
void Slot111(){}
void Slot112(){}
void Slot113(){}
void Slot114(){}
void Slot115(){}
void Slot116(){}
void Slot117(){}
void Slot118(){}
void Slot119(){}
void Slot120(){}
void Slot121(){}
void Slot122(){}
void Slot123(){}
void Slot124(){}
void Slot125(){}
void Slot126(){}
void Slot127(){}
void Slot128(){}
void Slot129(){}
void Slot130(){}
void ChangeState(unsigned long){++changed;}unsigned long GetType()const{return 8;}DefPointer GetDef()const{DefPointer p;p.Object=&def;++def.Count;return p;}void SetTargetState(unsigned long n){target=n;}void Observer0(){}};
int main(){Entry e,parent;e.Parent=&parent;e.TargetState=9;e.CurrentTime=77;e.Actions25=e.Actions28=0;def.Count=10;def.PartOfListTree=false;e.GoBackwards();if(e.LastTime!=77||target!=9||prepared!=1||LastMenuTime_013b8aa0)return 1;Stack::Node head,b;Stack::Node*a=(Stack::Node*)malloc(sizeof(Stack::Node));head.Next=a;head.Prev=&b;a->Next=&b;a->Prev=&head;a->Object=&e;b.Next=&head;b.Prev=a;b.Object=&parent;MenuStack_013b8a98.Head=&head;def.PartOfListTree=true;def.PCStyle=true;e.GoBackwards();if(changed!=2||removed!=1||actions!=1||LastMenuTime_013b8aa0!=123.5)return 2;puts("0053ddd0_TEST PASS");return 0;}

CCharString::CCharString():storage_(0){}CCharString::~CCharString(){}
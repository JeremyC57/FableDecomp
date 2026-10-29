// BEGIN exact source snapshot SHA256 8bf1466d123fdaf5f5d687e2f46645176b9d08cd67fd3b8f3837300644f4f779
//00555cf0 NUISystem::CScrollableList::ProcessEvent.
// Layout: secondary CObserver at+4; retailobserver20 versus donor24 (STLset shrink).
#include "engine/CObserver.h"
#include "fable_string.h"
struct RefCount {long Count;void DecRef(){--Count;}long GetRefCount()const{return Count;}};struct RefVirtual {virtual void Slot0()=0;virtual void Delete()=0;};struct Def:RefVirtual,RefCount {unsigned char ToActions[0x1a0-8];CCharString SoundForward;void Release(){DecRef();if(GetRefCount()==0)Delete();}};
struct DefPointer {Def* Object;__forceinline ~DefPointer(){if(Object){Object->Release();Object=0;}}Def* operator->()const{return Object;}};
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
};
struct Observer {virtual void Observer0()=0;virtual void ProcessEvent(int);unsigned char Events[0xc];bool Prevent;unsigned char Pad[3];};
struct Pointer {Component* Object;void* Info;};struct Vector {Pointer* First;Pointer* Last;Pointer* Limit;unsigned int size()const{return Last-First;}};
struct Stack {struct Node {Node* Next;Node* Prev;Component* Object;};Node* Head;unsigned long size()const;void pop_front();Component* front()const{return Head->Next->Object;}};extern Stack MenuStack_013b8a98;
class CManager { public:virtual void ManagerSlot00()=0;
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
virtual void ManagerSlot25()=0;
virtual void ManagerSlot26()=0;
virtual void ManagerSlot27()=0;
virtual void ManagerSlot28()=0;
virtual void ManagerSlot29()=0;
virtual void ManagerSlot30()=0;
virtual void ManagerSlot31()=0;
virtual void SetChange(long)=0;
virtual void ManagerSlot33()=0;
virtual void ManagerSlot34()=0;
virtual void ManagerSlot35()=0;
virtual void ManagerSlot36()=0;
virtual void ManagerSlot37()=0;
virtual void ManagerSlot38()=0;
virtual void ManagerSlot39()=0;
virtual void PlaySoundW(CCharString&,long)=0;
virtual void ManagerSlot41()=0;
virtual DefPointer GetMiscThingsDef()=0;
static CManager* __fastcall GetInstance();};
class CScrollableList:public Component,public Observer { public:unsigned char BeforeChildren[0xb0-0x18];Vector Children;unsigned char BeforeTop[0x15c-0xbc];unsigned long Top,Visible;void SetScroll(float,float);void ProcessEvent(int);};
extern void* Game_013b86a0;class CRedefinerList { public:static bool __fastcall CanAcceptChanges();};struct CUserProfileManager {void Save();};CUserProfileManager* __fastcall GetUserProfileManager();
void CScrollableList::ProcessEvent(int event){Observer::ProcessEvent(event);switch(event){case 16:case 36:if(Top>0){--Top;SetScroll(.2f,.2f);}break;case 17:case 37:if(Children.size()>Visible&&Top<Children.size()-Visible){++Top;SetScroll(.2f,.2f);}break;case 4:if(Game_013b86a0&&CRedefinerList::CanAcceptChanges()){if(MenuStack_013b8a98.size()>1){MenuStack_013b8a98.front()->ChangeState(6);MenuStack_013b8a98.pop_front();MenuStack_013b8a98.front()->ChangeState(3);CManager::GetInstance()->RemoveObserver(this);}CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward,1);CManager::GetInstance()->SetChange(0);GetUserProfileManager()->Save();}break;}}
}

// END exact source snapshot
#include <stdio.h>
using namespace NUISystem;
Stack NUISystem::MenuStack_013b8a98;void* NUISystem::Game_013b86a0;static int baseCalls,scrollCalls,actions,removed,resumed;static bool closeMenu=true;
void Observer::ProcessEvent(int){++baseCalls;}void CScrollableList::SetScroll(float x,float y){if(x==.2f&&y==.2f)++scrollCalls;}
unsigned long Stack::size()const{unsigned n=0;for(Node*p=Head->Next;p!=Head;p=p->Next)++n;return n;}void Stack::pop_front(){Head->Next=Head->Next->Next;}
bool __fastcall NUISystem::CRedefinerList::CanAcceptChanges(){return closeMenu;}void CUserProfileManager::Save(){++resumed;}CUserProfileManager* __fastcall NUISystem::GetUserProfileManager(){static CUserProfileManager p;return &p;}
struct TestDef:Def {void Slot0(){}void Delete(){}}def;
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
void ManagerSlot25(){}
void ManagerSlot26(){}
void ManagerSlot27(){}
void ManagerSlot28(){}
void ManagerSlot29(){}
void ManagerSlot30(){}
void ManagerSlot31(){}
void ManagerSlot33(){}
void ManagerSlot34(){}
void ManagerSlot35(){}
void ManagerSlot36(){}
void ManagerSlot37(){}
void ManagerSlot38(){}
void ManagerSlot39(){}
void ManagerSlot41(){}
void RemoveObserver(Observer*){++removed;}void SetChange(long){}void PlaySoundW(CCharString&,long){++actions;}DefPointer GetMiscThingsDef(){DefPointer p;p.Object=&def;++def.Count;return p;}}manager;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance(){return &manager;}
struct List:CScrollableList {
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
void ChangeState(unsigned long){}void Observer0(){}};
int main(){List l;Pointer children[5];l.Children.First=children;l.Children.Last=children+5;l.Children.Limit=children+5;l.Visible=3;l.Top=0;l.ProcessEvent(16);if(scrollCalls)return 1;l.ProcessEvent(17);l.ProcessEvent(37);l.ProcessEvent(17);if(l.Top!=2||scrollCalls!=2)return 2;l.ProcessEvent(36);if(l.Top!=1)return 3;l.ProcessEvent(25);if(scrollCalls!=3)return 4;Stack::Node head,a,b;head.Next=&a;a.Next=&b;b.Next=&head;a.Object=b.Object=&l;MenuStack_013b8a98.Head=&head;Game_013b86a0=&l;def.Count=10;l.ProcessEvent(4);if(removed!=1||actions!=1||resumed!=1)return 5;puts("00555cf0_TEST PASS");return 0;}

CCharString::CCharString(){ storage_=0; }

CCharString::~CCharString(){}
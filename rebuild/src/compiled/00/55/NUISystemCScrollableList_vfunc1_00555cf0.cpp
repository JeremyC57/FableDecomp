//00555cf0 NUISystem::CScrollableList::ProcessEvent.
// Leaf: ?ProcessEvent@CScrollableList@NUISystem@@UAEXH@Z
// Names (agentNF25b): the 0x00555A90 callee is Ego_r/FableWin protected ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z (was a local public SetScroll).
// Layout: secondary CObserver at+4; retailobserver20 versus donor24 (STLset shrink).
#include "fable_compiler.h"
#include "engine/CObserver.h"
#include "fable_string.h"
struct RefCount {long Count;void DecRef(){--Count;}long GetRefCount()const{return Count;}};struct RefVirtual {virtual void Slot0()=0;virtual void Delete()=0;};struct Def:RefVirtual,RefCount {unsigned char ToActions[0x1a0-8];CCharString SoundForward;void Release(){DecRef();if(GetRefCount()==0)Delete();}};
struct DefPointer {Def* Object;FABLE_FORCEINLINE ~DefPointer(){if(Object){Object->Release();Object=0;}}Def* operator->()const{return Object;}};
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h) (the null test only); was the stand-in NUISystem::Game_013b86a0 (void*).
#include "engine/CMainGameComponent.h"
// NF104 (q4909, 2026-11-19): 0x013B8A98 is ?GPreviousSelected@CList@NUISystem@@... (std::list<NUISystem::CList*>, Ego_r.pdb; engine/NUISystem_CList.h),
//   its size() the out-of-line STLport 0x0042B707 and pop_front() 0x0053C7D9; was the local Stack stand-in CList::GPreviousSelected.
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST
#include "engine/NUISystem_CList.h"
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
class CScrollableList:public Component,public Observer { public:unsigned char BeforeChildren[0xb0-0x18];Vector Children;unsigned char BeforeTop[0x15c-0xbc];unsigned long Top,Visible;void ProcessEvent(int);protected:void UpdateVisibleList(float,float);};
class CRedefinerList { public:static bool __fastcall CanAcceptChanges();};struct CUserProfileManager {void Save();};CUserProfileManager* __fastcall GetUserProfileManager();
void CScrollableList::ProcessEvent(int event){Observer::ProcessEvent(event);switch(event){case 16:case 36:if(Top>0){--Top;UpdateVisibleList(.2f,.2f);}break;case 17:case 37:if(Children.size()>Visible&&Top<Children.size()-Visible){++Top;UpdateVisibleList(.2f,.2f);}break;case 4:if(CMainGameComponent::Get()&&CRedefinerList::CanAcceptChanges()){if(CList::GPreviousSelected.size()>1){CList::GPreviousSelected.front()->ChangeState(6);CList::GPreviousSelected.pop_front();CList::GPreviousSelected.front()->ChangeState(3);CManager::GetInstance()->RemoveObserver(this);}CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward,1);CManager::GetInstance()->SetChange(0);GetUserProfileManager()->Save();}break;}}
}
// BEGIN exact source snapshot SHA256 b840a62d8ac3d3ca61117b24b20df2b6b4e40321ba780d9415dcd42e0f32f6f0
// 00544350 NUISystem::CSpellContainerList::ChangeState, 301 retail bytes.
// Recovered owner and member from RTTI slot48 and Ego_r PDB signature.
// Callees: CChangingStateComponent::ChangeState 0x0052cf40;
// CManager::GetInstance 0x0041e5f2. Observer slots2/5 add/remove.
// Layout: namespace header absent. PDB SelectedChild+0x194 and SpellContainers+0x1a0
// are retail+0x164/+0x170. Multiple bases: root+0, observer+4, third+0x18.
// CFrontEndManager generated header is a flattened layout with no virtual declarations;
// the local Calls view only describes its inherited observer dispatch slots.
#include "engine/CObserver.h"
// Layout: donor CObserver has EventsToObserve16 at+4 and flag+0x14.
// Retail ctor0052d9e0 (landed source) uses set12 and PreventObservation+0x10,
// so the observer is20bytes; ObserveEvent0052da20 confirms vtable01246020 slot3.
// Raw Spell secondary table012475a0 shares Observer slots2..7 with01246020;
// slot7=0041c580 writes PreventObservation at+0x10, confirming the4byte drift.
namespace NUISystem {
class CObserver { public: virtual void Obs0()=0; virtual void Obs1()=0; virtual void Obs2()=0; virtual void ObserveEvent(int event)=0; unsigned char EventsToObserve[0xc]; bool PreventObservation; unsigned char Alignment[3]; };
struct Primary {
 virtual void Slot000() = 0;
 virtual void Slot001() = 0;
 virtual void Slot002() = 0;
 virtual void Slot003() = 0;
 virtual void Slot004() = 0;
 virtual void Slot005() = 0;
 virtual void Slot006() = 0;
 virtual void Slot007() = 0;
 virtual void Slot008() = 0;
 virtual void Slot009() = 0;
 virtual void Slot010() = 0;
 virtual void Slot011() = 0;
 virtual void Slot012() = 0;
 virtual void Slot013() = 0;
 virtual void Slot014() = 0;
 virtual void Slot015() = 0;
 virtual void Slot016() = 0;
 virtual void Slot017() = 0;
 virtual void Slot018() = 0;
 virtual void Slot019() = 0;
 virtual void Slot020() = 0;
 virtual void Slot021() = 0;
 virtual void Slot022() = 0;
 virtual void Slot023() = 0;
 virtual void Slot024() = 0;
 virtual void Slot025() = 0;
 virtual void Slot026() = 0;
 virtual void Slot027() = 0;
 virtual void Slot028() = 0;
 virtual void Slot029() = 0;
 virtual void Slot030() = 0;
 virtual void Slot031() = 0;
 virtual void Slot032() = 0;
 virtual void Slot033() = 0;
 virtual void Slot034() = 0;
 virtual void Slot035() = 0;
 virtual void Slot036() = 0;
 virtual void Slot037() = 0;
 virtual void Slot038() = 0;
 virtual void Slot039() = 0;
 virtual void Slot040() = 0;
 virtual void Slot041() = 0;
 virtual void Slot042() = 0;
 virtual void Slot043() = 0;
 virtual void Slot044() = 0;
 virtual void Slot045() = 0;
 virtual void Slot046() = 0;
 virtual void Slot047() = 0;
 virtual void ChangeState(unsigned long state) = 0;
 virtual void Slot049() = 0;
 virtual void Slot050() = 0;
 virtual void Slot051() = 0;
 virtual void Slot052() = 0;
 virtual void Slot053() = 0;
 virtual void Slot054() = 0;
 virtual void Slot055() = 0;
 virtual void Slot056() = 0;
 virtual void Slot057() = 0;
 virtual void Slot058() = 0;
 virtual void Slot059() = 0;
 virtual void Slot060() = 0;
 virtual void Slot061() = 0;
 virtual void Slot062() = 0;
 virtual void Slot063() = 0;
 virtual void Slot064() = 0;
 virtual void Slot065() = 0;
 virtual void Slot066() = 0;
 virtual void Slot067() = 0;
 virtual void Slot068() = 0;
 virtual void Slot069() = 0;
 virtual void Slot070() = 0;
 virtual void Slot071() = 0;
 virtual void Slot072() = 0;
 virtual void Slot073() = 0;
 virtual void Slot074() = 0;
 virtual void Slot075() = 0;
 virtual void Slot076() = 0;
 virtual void Slot077() = 0;
 virtual void Slot078() = 0;
 virtual void Slot079() = 0;
 virtual void Slot080() = 0;
 virtual void Slot081() = 0;
 virtual void Slot082() = 0;
 virtual void Slot083() = 0;
 virtual void Slot084() = 0;
 virtual void Slot085() = 0;
 virtual void Slot086() = 0;
 virtual void Slot087() = 0;
 virtual void Slot088() = 0;
 virtual void Slot089() = 0;
 virtual void Slot090() = 0;
 virtual void Slot091() = 0;
 virtual void Slot092() = 0;
 virtual void Slot093() = 0;
 virtual void Slot094() = 0;
 virtual void Slot095() = 0;
 virtual void Slot096() = 0;
 virtual void Slot097() = 0;
 virtual void Slot098() = 0;
 virtual void Slot099() = 0;
 virtual void Slot100() = 0;
 virtual void Slot101() = 0;
 virtual void Slot102() = 0;
 virtual void Slot103() = 0;
 virtual void Slot104() = 0;
 virtual void Slot105() = 0;
 virtual void Slot106() = 0;
 virtual void Slot107() = 0;
 virtual void Slot108() = 0;
 virtual void Slot109() = 0;
 virtual void Slot110() = 0;
 virtual void Slot111() = 0;
 virtual void Slot112() = 0;
 virtual void Slot113() = 0;
 virtual void Slot114() = 0;
 virtual void Slot115() = 0;
 virtual void Slot116() = 0;
 virtual void Slot117() = 0;
 virtual void Slot118() = 0;
 virtual void Slot119() = 0;
 virtual void Slot120() = 0;
 virtual void Slot121() = 0;
 virtual void Slot122() = 0;
 virtual void Slot123() = 0;
 virtual void Slot124() = 0;
 virtual void Slot125() = 0;
 virtual void Slot126() = 0;
 virtual void Slot127() = 0;
 virtual void Slot128() = 0;
 virtual void Slot129() = 0;
 virtual void Slot130() = 0;
 virtual void Slot131() = 0;
 virtual void Slot132() = 0;
 virtual void Slot133() = 0;
 virtual void Slot134() = 0;
 virtual void Slot135() = 0;
 virtual void Slot136() = 0;
 virtual void Slot137() = 0;
 virtual void Slot138() = 0;
 virtual void Slot139() = 0;
 virtual void Slot140() = 0;
 virtual void Slot141() = 0;
 virtual void Slot142() = 0;
 virtual void Slot143() = 0;
 virtual void Slot144() = 0;
 virtual void Slot145() = 0;
 virtual void Slot146() = 0;
 virtual void Slot147() = 0;
 virtual void Slot148() = 0;
 virtual void Slot149() = 0;
 virtual void Slot150() = 0;
 virtual void ScrollDown() = 0;
};
struct Third { virtual void Third0()=0; };
class CChangingStateComponent:public Primary,public CObserver,public Third { public: void ChangeState(unsigned long); // 0x0052cf40
};
struct CManagerCalls {
 virtual void Front0()=0; virtual void Front1()=0; virtual void AddObserver(CObserver*)=0;
 virtual void Front3()=0; virtual void Front4()=0; virtual void RemoveObserver(CObserver*)=0;
 static CManagerCalls* __fastcall GetInstance(); // 0x0041e5f2
};
class CSpellContainerList:public CChangingStateComponent { public:
 unsigned char InheritedState[0x164-0x1c];
 unsigned int SelectedChild;
 void* SpellSelectionChild;
 void* SpellIndicationChild;
 void** SpellContainersStart;
 void** SpellContainersFinish;
 void** SpellContainersLimit;
 void ChangeState(unsigned long state);
};
void CSpellContainerList::ChangeState(unsigned long state) {
 if(state!=3) {
  CChangingStateComponent::ChangeState(state);
  CManagerCalls::GetInstance()->RemoveObserver(this);
  while(SelectedChild!=(unsigned)(SpellContainersFinish-SpellContainersStart)-1) ScrollDown();
 } else {
  CChangingStateComponent::ChangeState(3);
  CManagerCalls::GetInstance()->AddObserver(this);
  ObserveEvent(5);ObserveEvent(2);ObserveEvent(16);ObserveEvent(17);
  ObserveEvent(14);ObserveEvent(15);ObserveEvent(18);ObserveEvent(19);
  while(SelectedChild!=(unsigned)(SpellContainersFinish-SpellContainersStart)-1) ScrollDown();
 }
}
}

// END exact source snapshot
#include <stdio.h>
using namespace NUISystem;
static unsigned long stateSeen;
void CChangingStateComponent::ChangeState(unsigned long state){stateSeen=state;}
struct Manager:CManagerCalls {
 int adds,removes; NUISystem::CObserver* observed;
 Manager():adds(0),removes(0),observed(0){}
 void Front0(){} void Front1(){} void Front3(){} void Front4(){}
 void AddObserver(NUISystem::CObserver*p){++adds;observed=p;}void RemoveObserver(NUISystem::CObserver*p){++removes;observed=p;}
} manager;
CManagerCalls* __fastcall CManagerCalls::GetInstance(){return &manager;}
struct Fixture:CSpellContainerList {
 int events[8],n,scrolls;
 Fixture():n(0),scrolls(0){}
 void ObserveEvent(int event){if(n<8)events[n++]=event;}
 void Obs0(){}void Obs1(){}void Obs2(){}void Third0(){}
 void ScrollDown(){++SelectedChild;++scrolls;}
 void Slot000() {}
 void Slot001() {}
 void Slot002() {}
 void Slot003() {}
 void Slot004() {}
 void Slot005() {}
 void Slot006() {}
 void Slot007() {}
 void Slot008() {}
 void Slot009() {}
 void Slot010() {}
 void Slot011() {}
 void Slot012() {}
 void Slot013() {}
 void Slot014() {}
 void Slot015() {}
 void Slot016() {}
 void Slot017() {}
 void Slot018() {}
 void Slot019() {}
 void Slot020() {}
 void Slot021() {}
 void Slot022() {}
 void Slot023() {}
 void Slot024() {}
 void Slot025() {}
 void Slot026() {}
 void Slot027() {}
 void Slot028() {}
 void Slot029() {}
 void Slot030() {}
 void Slot031() {}
 void Slot032() {}
 void Slot033() {}
 void Slot034() {}
 void Slot035() {}
 void Slot036() {}
 void Slot037() {}
 void Slot038() {}
 void Slot039() {}
 void Slot040() {}
 void Slot041() {}
 void Slot042() {}
 void Slot043() {}
 void Slot044() {}
 void Slot045() {}
 void Slot046() {}
 void Slot047() {}
 void Slot049() {}
 void Slot050() {}
 void Slot051() {}
 void Slot052() {}
 void Slot053() {}
 void Slot054() {}
 void Slot055() {}
 void Slot056() {}
 void Slot057() {}
 void Slot058() {}
 void Slot059() {}
 void Slot060() {}
 void Slot061() {}
 void Slot062() {}
 void Slot063() {}
 void Slot064() {}
 void Slot065() {}
 void Slot066() {}
 void Slot067() {}
 void Slot068() {}
 void Slot069() {}
 void Slot070() {}
 void Slot071() {}
 void Slot072() {}
 void Slot073() {}
 void Slot074() {}
 void Slot075() {}
 void Slot076() {}
 void Slot077() {}
 void Slot078() {}
 void Slot079() {}
 void Slot080() {}
 void Slot081() {}
 void Slot082() {}
 void Slot083() {}
 void Slot084() {}
 void Slot085() {}
 void Slot086() {}
 void Slot087() {}
 void Slot088() {}
 void Slot089() {}
 void Slot090() {}
 void Slot091() {}
 void Slot092() {}
 void Slot093() {}
 void Slot094() {}
 void Slot095() {}
 void Slot096() {}
 void Slot097() {}
 void Slot098() {}
 void Slot099() {}
 void Slot100() {}
 void Slot101() {}
 void Slot102() {}
 void Slot103() {}
 void Slot104() {}
 void Slot105() {}
 void Slot106() {}
 void Slot107() {}
 void Slot108() {}
 void Slot109() {}
 void Slot110() {}
 void Slot111() {}
 void Slot112() {}
 void Slot113() {}
 void Slot114() {}
 void Slot115() {}
 void Slot116() {}
 void Slot117() {}
 void Slot118() {}
 void Slot119() {}
 void Slot120() {}
 void Slot121() {}
 void Slot122() {}
 void Slot123() {}
 void Slot124() {}
 void Slot125() {}
 void Slot126() {}
 void Slot127() {}
 void Slot128() {}
 void Slot129() {}
 void Slot130() {}
 void Slot131() {}
 void Slot132() {}
 void Slot133() {}
 void Slot134() {}
 void Slot135() {}
 void Slot136() {}
 void Slot137() {}
 void Slot138() {}
 void Slot139() {}
 void Slot140() {}
 void Slot141() {}
 void Slot142() {}
 void Slot143() {}
 void Slot144() {}
 void Slot145() {}
 void Slot146() {}
 void Slot147() {}
 void Slot148() {}
 void Slot149() {}
 void Slot150() {}
};
int main(){
 Fixture f;void* children[3]={0,0,0};f.SpellContainersStart=children;f.SpellContainersFinish=children+3;f.SelectedChild=0;
 f.ChangeState(3);int expected[8]={5,2,16,17,14,15,18,19};
 if(stateSeen!=3||manager.adds!=1||manager.observed!=static_cast<NUISystem::CObserver*>(&f)||f.SelectedChild!=2||f.scrolls!=2||f.n!=8)return 1;
 for(int i=0;i<8;++i)if(f.events[i]!=expected[i])return 2;
 f.ChangeState(7);if(stateSeen!=7||manager.removes!=1||f.scrolls!=2||f.n!=8)return 3;
 f.SelectedChild=1;f.ChangeState(0);if(f.SelectedChild!=2||f.scrolls!=3)return 4;
 puts("00544350_TEST PASS");return 0;
}
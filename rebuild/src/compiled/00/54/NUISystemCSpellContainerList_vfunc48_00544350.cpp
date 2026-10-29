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
class CManager { public:
 virtual void Front0()=0; virtual void Front1()=0; virtual void AddObserver(CObserver*)=0;
 virtual void Front3()=0; virtual void Front4()=0; virtual void RemoveObserver(CObserver*)=0;
 static CManager* __fastcall GetInstance(); // 0x0041e5f2
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
  CManager::GetInstance()->RemoveObserver(this);
  while(SelectedChild!=(unsigned)(SpellContainersFinish-SpellContainersStart)-1) ScrollDown();
 } else {
  CChangingStateComponent::ChangeState(3);
  CManager::GetInstance()->AddObserver(this);
  ObserveEvent(5);ObserveEvent(2);ObserveEvent(16);ObserveEvent(17);
  ObserveEvent(14);ObserveEvent(15);ObserveEvent(18);ObserveEvent(19);
  while(SelectedChild!=(unsigned)(SpellContainersFinish-SpellContainersStart)-1) ScrollDown();
 }
}
}
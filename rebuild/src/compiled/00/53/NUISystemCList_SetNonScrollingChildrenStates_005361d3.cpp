// NUISystem::CList::SetNonScrollingChildrenStates() -- with scrolling children, each non-scrolling child takes state SelectedChild+8 when it has that state (itself or in its children), else 1; without scrolling children, every non-scrolling child goes to state 1
// Leaf: ?SetNonScrollingChildrenStates@CList@NUISystem@@MAEXXZ
// Grade: MATCH (retry wave 2) -- the entry test is `ScrollingChildren.size() > 0` (unsigned): retail keeps `sar ecx,2 ; je`; `size()` / `!= 0` /
//   `!empty()` let VC7.1 fold the shift into `test ecx,0FFFFFFFCh` (221v218).
// Retail 0x005361D3, 218 bytes, vtable 0x012462E4 slot 180; Ego_r.pdb / FableWin.pdb ?SetNonScrollingChildrenStates@CList@NUISystem@@MAEXXZ (protected virtual).
// Callees on each child (virtual, CComponent vtable 0x0124608C): slot 44 HasState, slot 45 HasStateInChildren (the parked
//   HasStateDefinition / HasStateTransition were invented: CList's slots 44/45 are CChangingStateComponent::HasState 0x0041C5C0 and
//   CComponent::HasStateInChildren 0x00530530, both `bool(unsigned long)` in Ego_r), slot 48 ChangeState(unsigned long) 0x0052E710.
#pragma optimize("s", on)
// CList TU is size-optimised (no inter-function padding, odd addresses): landed CList rows use optimize("s").
// Local model: rebuild/include/engine/CList.h is the donor (Ego_r) flattened layout whose offsets disagree with retail
// (retail SelectedChild +0x15c, ScrollingChildren +0x164, NonScrollingChildren +0x170; landed CList neighbours). The SlotNNN
// fillers only place the three CComponent virtuals the body calls.
namespace NUISystem {
class CComponent { public:
 virtual void Slot000()=0; virtual void Slot001()=0; virtual void Slot002()=0; virtual void Slot003()=0; virtual void Slot004()=0; virtual void Slot005()=0;
 virtual void Slot006()=0; virtual void Slot007()=0; virtual void Slot008()=0; virtual void Slot009()=0; virtual void Slot010()=0; virtual void Slot011()=0;
 virtual void Slot012()=0; virtual void Slot013()=0; virtual void Slot014()=0; virtual void Slot015()=0; virtual void Slot016()=0; virtual void Slot017()=0;
 virtual void Slot018()=0; virtual void Slot019()=0; virtual void Slot020()=0; virtual void Slot021()=0; virtual void Slot022()=0; virtual void Slot023()=0;
 virtual void Slot024()=0; virtual void Slot025()=0; virtual void Slot026()=0; virtual void Slot027()=0; virtual void Slot028()=0; virtual void Slot029()=0;
 virtual void Slot030()=0; virtual void Slot031()=0; virtual void Slot032()=0; virtual void Slot033()=0; virtual void Slot034()=0; virtual void Slot035()=0;
 virtual void Slot036()=0; virtual void Slot037()=0; virtual void Slot038()=0; virtual void Slot039()=0; virtual void Slot040()=0; virtual void Slot041()=0;
 virtual void Slot042()=0; virtual void Slot043()=0;
 virtual bool HasState(unsigned long);            // slot 44 (+0xb0): Ego_r ?HasState@CComponent@NUISystem@@UAE_NK@Z (CList's slot 44 is CChangingStateComponent::HasState 0x0041C5C0)
 virtual bool HasStateInChildren(unsigned long);  // slot 45 (+0xb4): Ego_r ?HasStateInChildren@CComponent@NUISystem@@UAE_NK@Z (0x00530530)
 virtual void Slot046()=0; virtual void Slot047()=0;
 virtual void ChangeState(unsigned long);         // slot 48 (+0xc0): ?ChangeState@CComponent@NUISystem@@UAEXK@Z (0x0052E710)
 virtual void Slot049()=0; virtual void Slot050()=0; virtual void Slot051()=0; virtual void Slot052()=0; virtual void Slot053()=0; virtual void Slot054()=0;
 virtual void Slot055()=0; virtual void Slot056()=0; virtual void Slot057()=0; virtual void Slot058()=0; virtual void Slot059()=0; virtual void Slot060()=0;
 virtual void Slot061()=0; virtual void Slot062()=0; virtual void Slot063()=0; virtual void Slot064()=0; virtual void Slot065()=0; virtual void Slot066()=0;
 virtual void Slot067()=0; virtual void Slot068()=0; virtual void Slot069()=0; virtual void Slot070()=0; virtual void Slot071()=0; virtual void Slot072()=0;
 virtual void Slot073()=0; virtual void Slot074()=0; virtual void Slot075()=0; virtual void Slot076()=0; virtual void Slot077()=0; virtual void Slot078()=0;
 virtual void Slot079()=0; virtual void Slot080()=0; virtual void Slot081()=0; virtual void Slot082()=0; virtual void Slot083()=0; virtual void Slot084()=0;
 virtual void Slot085()=0; virtual void Slot086()=0; virtual void Slot087()=0; virtual void Slot088()=0; virtual void Slot089()=0; virtual void Slot090()=0;
 virtual void Slot091()=0; virtual void Slot092()=0; virtual void Slot093()=0; virtual void Slot094()=0; virtual void Slot095()=0; virtual void Slot096()=0;
 virtual void Slot097()=0; virtual void Slot098()=0; virtual void Slot099()=0; virtual void Slot100()=0; virtual void Slot101()=0; virtual void Slot102()=0;
 virtual void Slot103()=0; virtual void Slot104()=0; virtual void Slot105()=0; virtual void Slot106()=0; virtual void Slot107()=0; virtual void Slot108()=0;
 virtual void Slot109()=0; virtual void Slot110()=0; virtual void Slot111()=0; virtual void Slot112()=0; virtual void Slot113()=0; virtual void Slot114()=0;
 virtual void Slot115()=0; virtual void Slot116()=0; virtual void Slot117()=0; virtual void Slot118()=0; virtual void Slot119()=0; virtual void Slot120()=0;
 virtual void Slot121()=0; virtual void Slot122()=0; virtual void Slot123()=0; virtual void Slot124()=0; virtual void Slot125()=0; virtual void Slot126()=0;
 virtual void Slot127()=0; virtual void Slot128()=0; virtual void Slot129()=0; virtual void Slot130()=0; virtual void Slot131()=0; virtual void Slot132()=0;
 virtual void Slot133()=0; virtual void Slot134()=0; virtual void Slot135()=0; virtual void Slot136()=0; virtual void Slot137()=0; virtual void Slot138()=0;
 virtual void Slot139()=0; virtual void Slot140()=0; virtual void Slot141()=0;
};
template<class T> struct CRetailVector {   // STLport vector<T>: 12 bytes, first/last/end
 T* First; T* Last; T* End;
 unsigned long size() const { return (unsigned long)(Last - First); }
 T& operator[](unsigned long i) { return First[i]; }
};
class CList : public CComponent {
public:
 unsigned char _pad04[0x15c - 4];
 long SelectedChild;                               // +0x15c
 unsigned char _pad160[0x164 - 0x160];
 CRetailVector<CComponent*> ScrollingChildren;     // +0x164
 CRetailVector<CComponent*> NonScrollingChildren;  // +0x170
protected:
 virtual void SetNonScrollingChildrenStates();     // slot 180 (+0x2d0)
};
void CList::SetNonScrollingChildrenStates()
{
 if (ScrollingChildren.size() > 0) {
  for (unsigned long i = 0; i < NonScrollingChildren.size(); ++i)
   if (NonScrollingChildren[i]->HasState(SelectedChild + 8) || NonScrollingChildren[i]->HasStateInChildren(SelectedChild + 8))
    NonScrollingChildren[i]->ChangeState(SelectedChild + 8);
   else
    NonScrollingChildren[i]->ChangeState(1);
 } else {
  for (unsigned long j = 0; j < NonScrollingChildren.size(); ++j)
   NonScrollingChildren[j]->ChangeState(1);
 }
}
}
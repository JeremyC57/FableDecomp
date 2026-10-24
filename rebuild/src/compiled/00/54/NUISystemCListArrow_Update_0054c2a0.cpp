// NUISystem::CListArrow::Update(float) -- base update; show the arrow (state 1) while the parent list's selection lies in [MinToDraw, MaxToDraw], hide it (state 0) otherwise
// Retail 0x0054C2A0, 127 bytes (vtable 0x01248F94 slot 1). Callees: CChangingStateComponent::Update 0x0052C7E0, the parent
// list's GetSelectedChild (slot 142, +0x238, called twice), ChangeState (slot 48). Each switch sets ForceStateChange (+0x18c).
// Name: Ego_r.pdb ?Update@CListArrow@NUISystem@@UAEXM@Z (source order Initialise, ChangeState, Update, SetMinToDraw, SetMaxToDraw).
// Local model (CComponent is quarantined; the engine CList.h / CFrontEndList.h are flattened donor layouts):
// retail CurrentState +0x144, TargetState +0x148 (landed CChangingStateComponent rows), Parent +0xc8.
namespace NUISystem {
struct CComponent {
 virtual void Slot000()=0;
 virtual void Update(float); // slot 1 (+0x4)
 virtual void Slot002()=0; virtual void Slot003()=0; virtual void Slot004()=0; virtual void Slot005()=0; virtual void Slot006()=0; virtual void Slot007()=0;
 virtual void Slot008()=0; virtual void Slot009()=0; virtual void Slot010()=0; virtual void Slot011()=0; virtual void Slot012()=0; virtual void Slot013()=0;
 virtual void Slot014()=0; virtual void Slot015()=0; virtual void Slot016()=0; virtual void Slot017()=0; virtual void Slot018()=0; virtual void Slot019()=0;
 virtual void Slot020()=0; virtual void Slot021()=0; virtual void Slot022()=0; virtual void Slot023()=0; virtual void Slot024()=0; virtual void Slot025()=0;
 virtual void Slot026()=0; virtual void Slot027()=0; virtual void Slot028()=0; virtual void Slot029()=0; virtual void Slot030()=0; virtual void Slot031()=0;
 virtual void Slot032()=0; virtual void Slot033()=0; virtual void Slot034()=0; virtual void Slot035()=0; virtual void Slot036()=0; virtual void Slot037()=0;
 virtual void Slot038()=0; virtual void Slot039()=0; virtual void Slot040()=0; virtual void Slot041()=0; virtual void Slot042()=0; virtual void Slot043()=0;
 virtual void Slot044()=0; virtual void Slot045()=0; virtual void Slot046()=0; virtual void Slot047()=0;
 virtual void ChangeState(unsigned long); // slot 48 (+0xc0)
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
 virtual void Slot133()=0; virtual void Slot134()=0; virtual void Slot135()=0; virtual void Slot136()=0;
 virtual bool ChangedStateLastUpdate(); // slot 137 (+0x224)
 virtual void Slot138()=0; virtual void Slot139()=0; virtual void Slot140()=0; virtual void Slot141()=0;
};
struct CList : CComponent {
 virtual long GetSelectedChild();     // slot 142 (+0x238), 0x0053CDC3
};
struct CChangingStateComponent : CComponent {
 unsigned char _pad04[0xc8 - 4];
 CComponent* Parent;                  // +0xc8
 unsigned char _padcc[0x144 - 0xcc];
 unsigned long CurrentState;          // +0x144
 unsigned long TargetState;           // +0x148
 void Update(float);                  // 0x0052C7E0
};
struct CListArrow : CChangingStateComponent {
 unsigned char _pad14c[0x184 - 0x14c];
 long MinToDraw;                      // +0x184
 long MaxToDraw;                      // +0x188
 bool ForceStateChange;               // +0x18c
 void Update(float);
};
void CListArrow::Update(float time)
{
 CChangingStateComponent::Update(time);
 CList* list = static_cast<CList*>(Parent);
 if (list->GetSelectedChild() <= MaxToDraw && list->GetSelectedChild() >= MinToDraw) {
  if (TargetState != 1) {
   ForceStateChange = true;
   ChangeState(1);
  }
 } else {
  if (TargetState != 0) {
   ForceStateChange = true;
   ChangeState(0);
  }
 }
}
}
// NUISystem::CList::ScrollDown(bool, bool) -- scroll the whole scrolling range: ScrollDown(0, size-1, a, b, false)
// Retail 0x00536088, 41 bytes (vtable 0x012462E4 slot 168). Callee: ScrollDown(ulong, ulong, bool, bool, bool) (slot 167, +0x29C).
// Shape: size TU pass-through `push dword ptr [esp+0x10]` of both bools. Name: Ego_r.pdb ?ScrollDown@CList@NUISystem@@UAEX_N0@Z.
#pragma optimize("s", on)
// CList TU is size-optimised (no inter-function padding, odd addresses): landed CList rows use optimize("s").
// Local model: rebuild/include/engine/CList.h is the donor (Ego_r) flattened layout whose offsets disagree with retail
// (retail ScrollingChildren +0x164, NonScrollingChildren +0x170, DefaultChild +0x1c4; landed 0x005361D3 neighbours).
struct C2DVector {
 float X, Y;
 C2DVector(float x, float y) : X(x), Y(y) {}
 C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}   // user copy ctor: X goes through the FPU, Y through a register (0x0053CDCA)
};
struct CCPPointerInfo;
namespace NUISystem {
struct CAction;
struct CComponent {
 virtual void Slot000()=0; virtual void Slot001()=0; virtual void Slot002()=0; virtual void Slot003()=0; virtual void Slot004()=0; virtual void Slot005()=0;
 virtual void Slot006()=0; virtual void Slot007()=0; virtual void Slot008()=0; virtual void Slot009()=0; virtual void Slot010()=0; virtual void Slot011()=0;
 virtual void Slot012()=0; virtual void Slot013()=0; virtual void Slot014()=0; virtual void Slot015()=0; virtual void Slot016()=0; virtual void Slot017()=0;
 virtual void Slot018()=0; virtual void Slot019()=0; virtual void Slot020()=0; virtual void Slot021()=0; virtual void Slot022()=0; virtual void Slot023()=0;
 virtual void Slot024()=0; virtual void Slot025()=0; virtual void Slot026()=0; virtual void Slot027()=0; virtual void Slot028()=0; virtual void Slot029()=0;
 virtual void Slot030()=0; virtual void Slot031()=0; virtual void Slot032()=0; virtual void Slot033()=0; virtual void Slot034()=0; virtual void Slot035()=0;
 virtual void Slot036()=0; virtual void Slot037()=0; virtual void Slot038()=0; virtual void Slot039()=0; virtual void Slot040()=0; virtual void Slot041()=0;
 virtual void Slot042()=0; virtual void Slot043()=0;
 virtual bool HasStateDefinition(unsigned long); // slot 44 (+0xb0)
 virtual bool HasStateTransition(unsigned long); // slot 45 (+0xb4)
 virtual void Slot046()=0; virtual void Slot047()=0;
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
 virtual void Slot133()=0; virtual void Slot134()=0; virtual void Slot135()=0; virtual void Slot136()=0; virtual void Slot137()=0; virtual void Slot138()=0;
 virtual void Slot139()=0; virtual void Slot140()=0; virtual void Slot141()=0;
};
// CCountedPointer<CAction>: ShareData is the out-of-line template member emitted right after these inline setters.
struct CActionPointer {
 CAction* Data; CCPPointerInfo* Info;
 void ShareData(CAction* data, CCPPointerInfo* info);   // 0x0053CEAE
 void operator=(const CActionPointer& p) { ShareData(p.Data, p.Info); }
};
template<class T> struct CRetailVector {   // VC7.1 vector<T>: 12 bytes, first/last/end
 T* First; T* Last; T* End;
 unsigned long size() const { return (unsigned long)(Last - First); }
 T& operator[](unsigned long i) { return First[i]; }
};
struct CList : CComponent {
 virtual long GetSelectedChild(); // slot 142 (+0x238)
 virtual void Slot143()=0; virtual void Slot144()=0; virtual void Slot145()=0; virtual void Slot146()=0;
 virtual C2DVector GetPositionOffset(); // slot 147 (+0x24c)
 virtual void Slot148()=0; virtual void Slot149()=0; virtual void Slot150()=0;
 virtual void SetNextScrollingChildPosition(C2DVector&); // slot 151 (+0x25c)
 virtual void Slot152()=0; virtual void Slot153()=0;
 virtual void SetActionOnScrollUp(CActionPointer&); // slot 154 (+0x268)
 virtual void SetActionOnScrollDown(CActionPointer&); // slot 155 (+0x26c)
 virtual void Slot156()=0;
 virtual void SetDefaultChild(long); virtual void SetDefaultChild(CComponent*); // overloads: MSVC reverses them, (CComponent*) 157, (long) 158 // slot 157 (+0x274)
 virtual void Slot159()=0; virtual void Slot160()=0; virtual void Slot161()=0; virtual void Slot162()=0; virtual void Slot163()=0; virtual void Slot164()=0;
 virtual void ScrollUp(bool, bool); virtual void ScrollUp(unsigned long, unsigned long, bool, bool, bool); // 165 = 5-arg, 166 = (bool, bool) // slot 165 (+0x294)
 virtual void ScrollDown(bool, bool); virtual void ScrollDown(unsigned long, unsigned long, bool, bool, bool); // 167 = 5-arg, 168 = (bool, bool) // slot 167 (+0x29c)
 virtual void Slot169()=0; virtual void Slot170()=0; virtual void Slot171()=0; virtual void Slot172()=0; virtual void Slot173()=0; virtual void Slot174()=0;
 virtual void Slot175()=0; virtual void Slot176()=0; virtual void Slot177()=0; virtual void Slot178()=0; virtual void Slot179()=0;
 virtual void SetNonScrollingChildrenStates(); // slot 180 (+0x2d0)
 virtual void Slot181()=0; virtual void Slot182()=0; virtual void Slot183()=0; virtual void Slot184()=0; virtual void Slot185()=0; virtual void Slot186()=0;
 virtual void Slot187()=0;
 unsigned char _pad04[0x15c - 4];
 long SelectedChild;                       // +0x15c
 unsigned char _pad160[0x164 - 0x160];
 CRetailVector<CComponent*> ScrollingChildren;     // +0x164
 CRetailVector<CComponent*> NonScrollingChildren;  // +0x170
 unsigned char _pad17c[0x188 - 0x17c];
 C2DVector PositionOffset;                 // +0x188
 unsigned char _pad190[0x1a4 - 0x190];
 C2DVector NextScrollingChildPosition;     // +0x1a4
 unsigned char _pad1ac[0x1b4 - 0x1ac];
 CActionPointer ActionOnScrollUp;          // +0x1b4
 CActionPointer ActionOnScrollDown;        // +0x1bc
 long DefaultChild;                        // +0x1c4
};
void CList::ScrollDown(bool a, bool b)
{
 ScrollDown(0, ScrollingChildren.size() - 1, a, b, false);
}
}
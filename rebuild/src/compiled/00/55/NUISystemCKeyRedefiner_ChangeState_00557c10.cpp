// NUISystem::CKeyRedefiner::ChangeState(unsigned long) -- base state change; on 1/5 register as a manager observer unless selected, on 0/6 cancel the selection and ignore event 0x19
// Retail 0x00557C10, 131 bytes incl. the embedded jump table (vtable 0x0124ADBC slot 48). Callees: CChangingStateComponent::ChangeState
// 0x0052CF40, CManager::GetInstance 0x0041E5F2 + CObservable::AddObserver (slot 2, +8; the this->CObserver* conversion keeps its
// null test), CancelSelection 0x00557BD0, IgnoreEvent (observer slot 4, tail-jumped with the argument slot rewritten to 0x19).
// Jump table: byte index table 00 01 02 02 02 01 00 over {case 0/6, case 1/5, default}.
// Name: Ego_r.pdb ?ChangeState@CKeyRedefiner@NUISystem@@UAEXK@Z.
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include "fable_string.h"
#include <stdlib.h>
// CUIDef: CIVCountedPointee (vtable +0, slot 1 = delete-self; reference count +4 in a second base). Retail offsets:
// right-click target state +0x210, PCStyle +0x221 (landed 0x0053D3B0 / 0x0053DDD0).
struct CIVRefCount { long RefCount; void DecRef() { --RefCount; } long GetRefCount() const { return RefCount; } };
struct CIVDeletable { virtual void Slot0() = 0; virtual void DeleteSelf() = 0; };
struct CUIDef : CIVDeletable, CIVRefCount {
 unsigned char DefinitionData[0x210 - 8]; unsigned long StateOnRightClicked;
 unsigned char MoreData[0x221 - 0x214]; bool PCStyle;
 void Release() { DecRef(); if (GetRefCount() == 0) DeleteSelf(); }
};
struct CUIDefPointer {
 CUIDef* Object;
 __forceinline ~CUIDefPointer() { if (Object) { Object->Release(); Object = 0; } }
 CUIDef* operator->() const { return Object; }
};
// CRGBColour stores B,G,R,A; its ctor assigns R, G, B, A (landed 0x00557A10).
class CRGBColour { public:
 unsigned char B, G, R, A;
 CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class C2DVector { public: float X, Y; };
class CActionInputControl { public:          // 28 bytes (landed ctor 0x00558540)
 long GameAction; long ControllerType; long KeyboardKey; long XboxButton; long MouseButton; C2DVector ControlDirection;
};
inline void* operator new(size_t, void* where) throw() { return where; }
namespace NUISystem {
enum EEvent { EVENT_DUMMY = 0 };
struct CActionList;                    // std::list<CCountedPointer<CAction>>
class CText { public:
 virtual void Slot000()=0; virtual void Slot001()=0; virtual void Slot002()=0; virtual void Slot003()=0; virtual void Slot004()=0; virtual void Slot005()=0;
 virtual void Slot006()=0; virtual void Slot007()=0; virtual void Slot008()=0; virtual void Slot009()=0; virtual void Slot010()=0; virtual void Slot011()=0;
 virtual void Slot012()=0;
 virtual void SetColour(const CRGBColour&); // slot 13 (+0x34)
 virtual void Slot014()=0; virtual void Slot015()=0; virtual void Slot016()=0; virtual void Slot017()=0; virtual void Slot018()=0; virtual void Slot019()=0;
 virtual void Slot020()=0; virtual void Slot021()=0; virtual void Slot022()=0; virtual void Slot023()=0; virtual void Slot024()=0; virtual void Slot025()=0;
 virtual void Slot026()=0; virtual void Slot027()=0; virtual void Slot028()=0; virtual void Slot029()=0; virtual void Slot030()=0; virtual void Slot031()=0;
 virtual void Slot032()=0; virtual void Slot033()=0; virtual void Slot034()=0; virtual void Slot035()=0; virtual void Slot036()=0; virtual void Slot037()=0;
 virtual void Slot038()=0; virtual void Slot039()=0; virtual void Slot040()=0; virtual void Slot041()=0; virtual void Slot042()=0; virtual void Slot043()=0;
 virtual void Slot044()=0; virtual void Slot045()=0; virtual void Slot046()=0; virtual void Slot047()=0; virtual void Slot048()=0; virtual void Slot049()=0;
 virtual void Slot050()=0; virtual void Slot051()=0; virtual void Slot052()=0; virtual void Slot053()=0; virtual void Slot054()=0; virtual void Slot055()=0;
 virtual void Slot056()=0; virtual void Slot057()=0; virtual void Slot058()=0; virtual void Slot059()=0; virtual void Slot060()=0; virtual void Slot061()=0;
 virtual void Slot062()=0; virtual void Slot063()=0; virtual void Slot064()=0; virtual void Slot065()=0; virtual void Slot066()=0; virtual void Slot067()=0;
 virtual void Slot068()=0; virtual void Slot069()=0; virtual void Slot070()=0; virtual void Slot071()=0; virtual void Slot072()=0; virtual void Slot073()=0;
 virtual void Slot074()=0; virtual void Slot075()=0; virtual void Slot076()=0; virtual void Slot077()=0; virtual void Slot078()=0; virtual void Slot079()=0;
 virtual void Slot080()=0; virtual void Slot081()=0; virtual void Slot082()=0; virtual void Slot083()=0; virtual void Slot084()=0; virtual void Slot085()=0;
 virtual void Slot086()=0; virtual void Slot087()=0; virtual void Slot088()=0; virtual void Slot089()=0; virtual void Slot090()=0; virtual void Slot091()=0;
 virtual void Slot092()=0; virtual void Slot093()=0; virtual void Slot094()=0; virtual void Slot095()=0; virtual void Slot096()=0; virtual void Slot097()=0;
 virtual void Slot098()=0; virtual void Slot099()=0; virtual void Slot100()=0; virtual void Slot101()=0; virtual void Slot102()=0; virtual void Slot103()=0;
 virtual void Slot104()=0; virtual void Slot105()=0; virtual void Slot106()=0; virtual void Slot107()=0; virtual void Slot108()=0; virtual void Slot109()=0;
 virtual void Slot110()=0; virtual void Slot111()=0; virtual void Slot112()=0; virtual void Slot113()=0; virtual void Slot114()=0; virtual void Slot115()=0;
 virtual void Slot116()=0; virtual void Slot117()=0; virtual void Slot118()=0; virtual void Slot119()=0; virtual void Slot120()=0; virtual void Slot121()=0;
 virtual void Slot122()=0; virtual void Slot123()=0; virtual void Slot124()=0; virtual void Slot125()=0; virtual void Slot126()=0; virtual void Slot127()=0;
 virtual void Slot128()=0; virtual void Slot129()=0; virtual void Slot130()=0; virtual void Slot131()=0; virtual void Slot132()=0; virtual void Slot133()=0;
 virtual void Slot134()=0; virtual void Slot135()=0; virtual void Slot136()=0; virtual void Slot137()=0; virtual void Slot138()=0; virtual void Slot139()=0;
 virtual void Slot140()=0; virtual void Slot141()=0; virtual void Slot142()=0; virtual void Slot143()=0;
 // MSVC lays overloaded virtuals out in reverse declaration order: CWideString form slot 144 (+0x240), tag form slot 145 (+0x244).
 virtual void SetText(const CCharString&, bool, bool);
 virtual void SetText(const CWideString&, bool, bool);
};
// CObserver (second base, +4): slot 2 n/a, slot 3 ObserveEvent, slot 4 IgnoreEvent.
class CObserver { public:
 virtual void Slot0() = 0; virtual void ProcessEvent(EEvent); virtual void Slot2() = 0;
 virtual void ObserveEvent(EEvent);   // slot 3 (+0xc)
 virtual void IgnoreEvent(EEvent);    // slot 4 (+0x10)
 unsigned char ObserverData[0x10];   // retail CObserver is 0x14 bytes (CClickable's third vptr sits at +0x18)
};
// CManager is a CObservable (vtable 0x01230134): slot 2 AddObserver 0x0055CA40, slot 3 AddExclusiveObserver 0x0055C930,
// slot 4 RemoveExclusiveObserver 0x0055C940.
struct CObservable {
 virtual void Slot0() = 0; virtual void Slot1() = 0;
 virtual void AddObserver(CObserver*);
 virtual void AddExclusiveObserver(CObserver*);
 virtual void RemoveExclusiveObserver(CObserver*);
};
class CManager : public CObservable { public:
 static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
class CComponent { public:
 virtual void Slot000()=0;
 virtual void Update(float); // slot 1 (+0x4)
 virtual void Slot002()=0; virtual void Slot003()=0; virtual void Slot004()=0; virtual void Slot005()=0; virtual void Slot006()=0; virtual void Slot007()=0;
 virtual void Slot008()=0; virtual void Slot009()=0; virtual void Slot010()=0; virtual void Slot011()=0; virtual void Slot012()=0; virtual void Slot013()=0;
 virtual void Slot014()=0; virtual void Slot015()=0; virtual void Slot016()=0; virtual void Slot017()=0; virtual void Slot018()=0; virtual void Slot019()=0;
 virtual void Slot020()=0; virtual void Slot021()=0; virtual void Slot022()=0; virtual void Slot023()=0; virtual void Slot024()=0; virtual void Slot025()=0;
 virtual void Slot026()=0; virtual void Slot027()=0; virtual void Slot028()=0; virtual void Slot029()=0; virtual void Slot030()=0; virtual void Slot031()=0;
 virtual void Slot032()=0; virtual void Slot033()=0; virtual void Slot034()=0; virtual void Slot035()=0; virtual void Slot036()=0; virtual void Slot037()=0;
 virtual void Slot038()=0; virtual void Slot039()=0; virtual void Slot040()=0; virtual void Slot041()=0; virtual void Slot042()=0;
 virtual void Initialise(); // slot 43 (+0xac)
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
 virtual void Slot103()=0; virtual void Slot104()=0; virtual void Slot105()=0; virtual void Slot106()=0; virtual void Slot107()=0;
 virtual CUIDefPointer LoadAndReturnDef() const; // slot 108 (+0x1b0)
 virtual void Slot109()=0; virtual void Slot110()=0; virtual void Slot111()=0; virtual void Slot112()=0; virtual void Slot113()=0; virtual void Slot114()=0;
 virtual void Slot115()=0; virtual void Slot116()=0; virtual void Slot117()=0; virtual void Slot118()=0; virtual void Slot119()=0; virtual void Slot120()=0;
 virtual void Slot121()=0; virtual void Slot122()=0; virtual void Slot123()=0; virtual void Slot124()=0; virtual void Slot125()=0; virtual void Slot126()=0;
 virtual void Slot127()=0; virtual void Slot128()=0; virtual void Slot129()=0; virtual void Slot130()=0;
 virtual void ExecuteActions(CActionList*); // slot 131 (+0x20c)
 virtual void Slot132()=0; virtual void Slot133()=0; virtual void Slot134()=0; virtual void Slot135()=0; virtual void Slot136()=0; virtual void Slot137()=0;
 virtual void Slot138()=0; virtual void Slot139()=0; virtual void Slot140()=0; virtual void Slot141()=0; virtual void Slot142()=0;
 virtual void OnHovered(); // slot 143 (+0x23c)
 virtual void OnUnhovered(); // slot 144 (+0x240)
 virtual void Slot145()=0;
 virtual void OnLeftClicked(); // slot 146 (+0x248)
 virtual void OnLeftUnclicked(); // slot 147 (+0x24c)
 virtual void OnRightClicked(); // slot 148 (+0x250)
 virtual void OnRightUnclicked(); // slot 149 (+0x254)
};
class CChangingStateComponent : public CComponent, public CObserver { public:
 void ChangeState(unsigned long);     // 0x0052CF40
};
struct CHoverableBase { virtual void HoverSlot0() = 0; };   // third vptr at +0x18
// CClickable: retail 0x194 bytes (landed ctor 0x00558540), three vptrs (+0, +4, +0x18).
class CClickable : public CChangingStateComponent, public CHoverableBase { public:
 unsigned char _pad1c[0x144 - 0x1c];
 unsigned long CurrentState;          // +0x144
 unsigned long TargetState;           // +0x148
 unsigned char _pad14c[0x180 - 0x14c];
 unsigned long StateBeforeRightClick; // +0x180
 unsigned char _pad184[0x188 - 0x184];
 CActionList* PActionsOnRightClicked; // +0x188
 unsigned char _pad18c[0x194 - 0x18c];
 void OnHovered();                    // 0x0055AEB0
 void OnUnhovered();                  // 0x0055AEF0
 void OnLeftUnclicked();              // 0x0055ACF0
 void OnRightClicked();
};
// STLport list<EGameAction>: one sentinel pointer, malloc/free node allocator, no size member.
struct CGameActionList {
 struct node { node* next; node* prev; long value; };
 node* head;
 __forceinline void clear()
 {
  node* cur = head->next;
  while (cur != head) { node* tmp = cur; cur = cur->next; free(tmp); }
  head->next = head;
  head->prev = head;
 }
 __forceinline node* create_node(const long& value)
 {
  node* tmp = (node*)malloc(sizeof(node));
  new (&tmp->value) long(value);
  return tmp;
 }
 __forceinline void insert(node* pos, const long& value)
 {
  node* tmp = create_node(value);
  node* prev = pos->prev;
  tmp->next = pos;
  tmp->prev = prev;
  prev->next = tmp;
  pos->prev = tmp;
 }
 __forceinline void push_back(const long& value) { insert(head, value); }
};
class CKeyRedefiner : public CClickable { public:   // vtables 0x0124ADBC / 0x0124AD98 / 0x0124AD90
 CText* PActionText;                  // +0x194
 CText* PKeyText;                     // +0x198
 CGameActionList Actions;             // +0x19c
 long SubType;                        // +0x1a0
 CActionInputControl Definition;      // +0x1a4
 void UpdateKeyText();                // 0x00557A10
 void CancelSelection();              // 0x00557BD0
 long GetSubTypeForAction(const CActionInputControl&) const;     // 0x00557CA0
 void GetActionText(CWideString&, const CCharString&) const;     // 0x00558170
 void OnHovered();
 void OnUnhovered();
 void OnLeftUnclicked();
 void ChangeState(unsigned long);
 void SetAction(const CActionInputControl&, const CCharString&);
};
extern CKeyRedefiner* g_pSelectedKeyRedefiner;   // 0x013B8AC8
void CKeyRedefiner::ChangeState(unsigned long state)
{
 CChangingStateComponent::ChangeState(state);
 switch (state) {
 case 1:
 case 5:
  if (g_pSelectedKeyRedefiner != this)
   CManager::GetInstance()->AddObserver(this);
  break;
 case 0:
 case 6:
  CancelSelection();
  IgnoreEvent((EEvent)0x19);
  break;
 }
}
}
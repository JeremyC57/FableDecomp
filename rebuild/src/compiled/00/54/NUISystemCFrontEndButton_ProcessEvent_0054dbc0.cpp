// NUISystem::CFrontEndButton::ProcessEvent(EEvent) -- debounced: once EventDelay has passed since LastEventTime, PC-style buttons forward the event to CClickable
// Retail 0x0054DBC0, 102 bytes: slot 1 of the CObserver vtable 0x01249530, so `this` is the observer subobject (+4) and
// members read at -4 (CurrentTime +0x2c, LastEventTime +0x190, EventDelay +0x188). Callees: LoadAndReturnDef (primary
// slot 108 on this-4), CClickable::ProcessEvent 0x0055AD60. `test ah,5 ; jnp` = early out on the ordered `<`.
// Name from Ego_r.pdb: ?ProcessEvent@CFrontEndButton@NUISystem@@UAEXW4EEvent@2@@Z.
#ifndef _MSC_VER
#define __forceinline inline
#endif
// CUIDef is a CIVCountedPointee: vtable at +0 (slot 1 = delete-self), reference count at +4 held in a
// second (non-polymorphic) base, so the release is `add eax,4 ; dec [eax]` on that subobject.
// Retail CUIDef::PCStyle sits at +0x221 (donor PDB +0x255; see landed 0x0053D3B0 / 0x0053DDD0).
struct CIVRefCount { long RefCount; void DecRef() { --RefCount; } long GetRefCount() const { return RefCount; } };
struct CIVDeletable { virtual void Slot0() = 0; virtual void DeleteSelf() = 0; };
struct CUIDef : CIVDeletable, CIVRefCount {
 unsigned char DefinitionData[0x221 - 8]; bool PCStyle;
 void Release() { DecRef(); if (GetRefCount() == 0) DeleteSelf(); }
};
// CDefPointer<CUIDef> returned by value (hidden pointer) from CComponent::LoadAndReturnDef.
struct CUIDefPointer {
 CUIDef* Object;
 __forceinline ~CUIDefPointer() { if (Object) { Object->Release(); Object = 0; } }
 CUIDef* operator->() const { return Object; }
};
namespace NUISystem {
enum EEvent { EVENT_DUMMY = 0 };
// Local models: rebuild/include/engine/CObserver.h / CHoverable.h are flattened PODs (`void* __vftable`)
// that cannot carry the virtual dispatch these bodies make, and CComponent is quarantined.
struct CComponent {
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
 virtual void OnObserverRemove(); // slot 109 (+0x1b4)
 virtual void Slot110()=0; virtual void Slot111()=0; virtual void Slot112()=0; virtual void Slot113()=0; virtual void Slot114()=0; virtual void Slot115()=0;
 virtual void Slot116()=0; virtual void Slot117()=0; virtual void Slot118()=0; virtual void Slot119()=0; virtual void Slot120()=0; virtual void Slot121()=0;
 virtual void Slot122()=0; virtual void Slot123()=0; virtual void Slot124()=0; virtual void Slot125()=0; virtual void Slot126()=0; virtual void Slot127()=0;
 virtual void Slot128()=0; virtual void Slot129()=0; virtual void Slot130()=0; virtual void Slot131()=0; virtual void Slot132()=0; virtual void Slot133()=0;
 virtual void Slot134()=0; virtual void Slot135()=0; virtual void Slot136()=0; virtual void Slot137()=0; virtual void Slot138()=0; virtual void Slot139()=0;
 virtual void Slot140()=0; virtual void Slot141()=0; virtual void Slot142()=0;
 virtual void OnHovered(); // slot 143 (+0x23c)
 virtual void OnUnhovered(); // slot 144 (+0x240)
 virtual void Slot145()=0; virtual void Slot146()=0; virtual void Slot147()=0; virtual void Slot148()=0; virtual void Slot149()=0; virtual void Slot150()=0;
};
// CObserver: secondary base at +4 (vtable 0x01249530 for CFrontEndButton): slot 1 ProcessEvent,
// slot 3 ObserveEvent, slot 4 IgnoreEvent; EventsToObserve set + PreventObservation.
struct CObserver {
 virtual void Slot0() = 0;
 virtual void ProcessEvent(EEvent);   // slot 1
 virtual void Slot2() = 0;
 virtual void ObserveEvent(EEvent);   // slot 3 (+0xc)   0x0052DA20
 virtual void IgnoreEvent(EEvent);    // slot 4 (+0x10)  0x0055BD50 (CHoverable override)
 unsigned char ObserverData[0x10];   // retail CObserver is 0x14 bytes (CClickable's third vptr sits at +0x18)
};
struct CChangingStateComponent : CComponent, CObserver {
 void Initialise();                   // 0x0052C730
};
struct CClickable : CChangingStateComponent {
 void Initialise();                   // 0x0055AC90
 void ProcessEvent(EEvent);           // 0x0055AD60 (CObserver-subobject `this`)
};
// CFrontEndButton (primary vtable 0x01249554, observer vtable 0x01249530). Retail offsets.
struct CFrontEndButton : CClickable {
 unsigned char _pad18[0x30 - 0x18];
 float CurrentTime;                   // +0x30  component clock
 unsigned char _pad34[0x160 - 0x34];
 bool WasHovered;                     // +0x160 CHoverable::WasHovered
 unsigned char _pad161[0x18c - 0x161];
 float EventDelay;                    // +0x18c minimum time between processed events
 unsigned char _pad190[0x194 - 0x190];
 float LastEventTime;                 // +0x194
 void Initialise();
 void ProcessEvent(EEvent);
 void OnHovered();
 void OnUnhovered();
 void OnObserverRemove();
};
void CFrontEndButton::ProcessEvent(EEvent event)
{
 if (CurrentTime - LastEventTime < EventDelay)
  return;
 if (LoadAndReturnDef()->PCStyle)
  CClickable::ProcessEvent(event);
}
}
// NUISystem::CZoomingComponent::~CZoomingComponent() -- unregisters the component's observer from the front-end UI manager, then destroys the CChangingStateComponent base
// Leaf: ??1CZoomingComponent@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546660, 48 bytes; stores the three CZoomingComponent vtables, calls
//   CManager::GetInstance and the manager's slot 5 with the CObserver subobject, then tail-jumps to the base destructor.
// Identity: the first stored vtable 0x01247DC4 is NUISystem::CZoomingComponent's (RTTI; its slot 0 is the scalar deleting
//   destructor 0x00546C20, which inlines this body); no direct caller (the derived/deleting destructors reach it).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (static __fastcall, landed; RTTI vtable 0x01230134 =
//   CManager : CObservable), CObservable slot 5 (+0x14) = ?RemoveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z
//   (vtable 0x01230134 slot 5 = 0x0055C9C0, landed name), ~CChangingStateComponent 0x0052CCF0.
// Layout: CChangingStateComponent is 0x15C bytes in retail (CComponent 0x134; see 0x0052CCA0); CObserver at +4.
// Shape: the observer subobject is formed once (`lea edi,[esi+4]`) before the vtable stores; the base destructor is a
//   tail `jmp` after the register pops.
namespace NUISystem {

enum EEvent { EEVENT_NONE = 0 };

class CObserver;

class CObservable {                             // NUISystem::CManager's base (vtable 0x01230134)
public:
    virtual ~CObservable();                     // slot 0
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);   // slot 1
    virtual void AddObserver(CObserver* observer);                   // slot 2
    virtual void _v3();
    virtual void _v4();
    virtual void RemoveObserver(CObserver* observer);                // slot 5 (+0x14)
};

class CManager : public CObservable {           // the front-end UI manager singleton (0xD0 bytes)
public:
    static CManager* __fastcall GetInstance();  // 0x0041E5F2
};

class CUIInterface {                            // +0x00
public:
    virtual ~CUIInterface();
};
class CObserver {                               // +0x04, 0x14 bytes in retail
public:
    virtual ~CObserver();
    unsigned char EventsToObserve[0xc];         // +0x04 std::set<EEvent>
    bool          PreventObservation;           // +0x10
};
class CActionParam {                            // +0x18
public:
    virtual ~CActionParam();
    long ActionParamType;                       // +0x1C
};

class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    unsigned char _pad020[0x134 - 0x20];        // retail sizeof 0x134
};

class CChangingStateComponent : public CComponent {
public:
    virtual ~CChangingStateComponent();         // 0x0052CCF0
    unsigned char _pad134[0x15c - 0x134];       // StatesBeingDone .. PreviousState (retail sizeof 0x15C)
};

} // namespace NUISystem

namespace NUISystem {

class CZoomingComponent : public CChangingStateComponent {
public:
    virtual ~CZoomingComponent();
};

CZoomingComponent::~CZoomingComponent()
{
    CManager::GetInstance()->RemoveObserver(this);
}

} // namespace NUISystem
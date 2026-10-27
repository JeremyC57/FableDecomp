// NUISystem::CZoomingComponent::`scalar deleting destructor'(unsigned int flags) -- runs ~CZoomingComponent (inlined) and frees the object when bit 0 of flags is set
// Leaf: ??_GCZoomingComponent@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546C20, 69 bytes, `ret 4`. NUISystem::CZoomingComponent vtable 0x01247DC4 slot 0
//   (RTTI); the two thunks 0x00546C00 / 0x00546C10 (vtables 0x01247DA0 / 0x01247D98) adjust `this` and jump here.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2, CObservable slot 5 (+0x14, RemoveObserver), ~CChangingStateComponent
//   0x0052CCF0, operator delete 0x00BFE9BC.
// Shape: VC7.1 expands the user destructor ~CZoomingComponent (defined out of line in this file, 0x00546660) inside the
//   compiler-generated ??_G: the three vtable stores, the RemoveObserver call, the base destructor call, then
//   `test byte ptr [esp+0Ch],1` and `operator delete(this)`; `this` is returned in eax.
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
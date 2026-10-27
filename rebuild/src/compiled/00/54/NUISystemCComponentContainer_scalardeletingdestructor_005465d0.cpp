// NUISystem::CComponentContainer::`scalar deleting destructor'(unsigned int flags) -- run the (empty) CComponentContainer destructor and free the control when bit 0 is set
// Leaf: ??_GCComponentContainer@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005465D0, 50 bytes, thiscall, `ret 4`.
// Identity (proven): it stores CComponentContainer's three vtables 0x01247AF4 / 0x01247ACC / 0x01247AC4 (RTTI .?AVCComponentContainer@NUISystem@@; the
//   CUIInterface, CObserver and CActionParam sub-objects at +0, +4, +0x18) and calls NUISystem::CChangingStateComponent::~CChangingStateComponent 0x0052CCF0; it
//   is slot 0 of the primary vtable and the target of the two this-adjusting thunks just before it.
// Callees: NUISystem::CChangingStateComponent::~CChangingStateComponent() 0x0052CCF0, operator delete 0x00BFE9BC.
// Shape (family M125, representative 0x005465D0): the compiler-generated virtual `??_G` with CComponentContainer's empty inline destructor
//   (the three vptr stores, then the base destructor); MakeOne's `new` emits the vtable and with it this function (not part
//   of the retail body).
// Header debt: the NUISystem control hierarchy is modelled by its three polymorphic bases and the base class's retail size.
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
namespace NUISystem {
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _data[0x10];                              // EventsToObserve, PreventObservation
};
class CActionParam { public: virtual ~CActionParam(); };
class CChangingStateComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    CChangingStateComponent();
    virtual ~CChangingStateComponent();                                       // 0x0052CCF0
};
class CComponentContainer : public CChangingStateComponent {
public:
    CComponentContainer() { }
    virtual ~CComponentContainer() { }
};
}
NUISystem::CComponentContainer* MakeOne()   // emits the vtable (and the scalar deleting destructor); not part of the retail body
{
    return new NUISystem::CComponentContainer;
}
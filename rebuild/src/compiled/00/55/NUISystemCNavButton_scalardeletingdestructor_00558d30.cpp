// NUISystem::CNavButton::`scalar deleting destructor'(unsigned int flags) -- run the (empty) CNavButton destructor and free the control when bit 0 is set
// Leaf: ??_GCNavButton@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00558D30, 50 bytes, thiscall, `ret 4`.
// Identity (proven): it stores CNavButton's three vtables 0x0124B04C / 0x0124B024 / 0x0124B01C (RTTI .?AVCNavButton@NUISystem@@; the
//   CUIInterface, CObserver and CActionParam sub-objects at +0, +4, +0x18) and calls NUISystem::CClickable::~CClickable 0x0055B760; it
//   is slot 0 of the primary vtable and the target of the two this-adjusting thunks just before it.
// Callees: NUISystem::CClickable::~CClickable() 0x0055B760, operator delete 0x00BFE9BC.
// Shape (family M125, representative 0x005465D0): the compiler-generated virtual `??_G` with CNavButton's empty inline destructor
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
class CClickable : public CUIInterface, public CObserver, public CActionParam {
public:
    CClickable();
    virtual ~CClickable();                                       // 0x0055B760
};
class CNavButton : public CClickable {
public:
    CNavButton() { }
    virtual ~CNavButton() { }
};
}
NUISystem::CNavButton* MakeOne()   // emits the vtable (and the scalar deleting destructor); not part of the retail body
{
    return new NUISystem::CNavButton;
}
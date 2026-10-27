// NUISystem::CFrontEndList::`scalar deleting destructor'(unsigned int flags) -- run the (empty) CFrontEndList destructor and free the control when bit 0 is set
// Leaf: ??_GCFrontEndList@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054DA70, 50 bytes, thiscall, `ret 4`.
// Identity (proven): it stores CFrontEndList's three vtables 0x01249224 / 0x012491FC / 0x012491F4 (RTTI .?AVCFrontEndList@NUISystem@@; the
//   CUIInterface, CObserver and CActionParam sub-objects at +0, +4, +0x18) and calls NUISystem::CList::~CList 0x0053B7F0; it
//   is slot 0 of the primary vtable and the target of the two this-adjusting thunks just before it.
// Callees: NUISystem::CList::~CList() 0x0053B7F0, operator delete 0x00BFE9BC.
// Shape (family M125, representative 0x005465D0): the compiler-generated virtual `??_G` with CFrontEndList's empty inline destructor
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
class CList : public CUIInterface, public CObserver, public CActionParam {
public:
    CList();
    virtual ~CList();                                       // 0x0053B7F0
};
class CFrontEndList : public CList {
public:
    CFrontEndList() { }
    virtual ~CFrontEndList() { }
};
}
NUISystem::CFrontEndList* MakeOne()   // emits the vtable (and the scalar deleting destructor); not part of the retail body
{
    return new NUISystem::CFrontEndList;
}
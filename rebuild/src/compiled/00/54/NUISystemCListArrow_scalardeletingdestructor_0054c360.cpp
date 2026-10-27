// NUISystem::CListArrow::`scalar deleting destructor'(unsigned int flags) -- run the (empty) CListArrow destructor and free the control when bit 0 is set
// Leaf: ??_GCListArrow@NUISystem@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054C360, 50 bytes, thiscall, `ret 4`.
// Identity (proven): it stores CListArrow's three vtables 0x01248F94 / 0x01248F6C / 0x01248F64 (RTTI .?AVCListArrow@NUISystem@@; the
//   CUIInterface, CObserver and CActionParam sub-objects at +0, +4, +0x18) and calls NUISystem::CSprite::~CSprite 0x0041AB40; it
//   is slot 0 of the primary vtable and the target of the two this-adjusting thunks just before it.
// Callees: NUISystem::CSprite::~CSprite() 0x0041AB40, operator delete 0x00BFE9BC.
// Shape (family M126, representative 0x00546F00): the compiler-generated virtual `??_G` with CListArrow's empty inline destructor
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
class CSprite : public CUIInterface, public CObserver, public CActionParam {
public:
    CSprite();
    virtual ~CSprite();                                       // 0x0041AB40
};
class CListArrow : public CSprite {
public:
    CListArrow() { }
    virtual ~CListArrow() { }
};
}
NUISystem::CListArrow* MakeOne()   // emits the vtable (and the scalar deleting destructor); not part of the retail body
{
    return new NUISystem::CListArrow;
}
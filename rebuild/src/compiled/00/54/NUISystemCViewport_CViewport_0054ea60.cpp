// NUISystem::CViewport::CViewport(const CViewport& other) -- copy constructor: copy the changing-state component base, then start a fresh viewport (empty primitive handle, no fake children, graphic index 0) and reload its data from the def
// Leaf: ??0CViewport@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe @ 0x0054EA60, 96 bytes, thiscall `ret 4`.
// Identity (proven): stores CViewport's three vtables 0x01249A5C / 0x01249A38 / 0x01249A30 (RTTI NUISystem::CViewport)
// after calling CChangingStateComponent's copy constructor 0x0052CCA0; Ego_r.pdb holds exactly two CViewport
// constructors, (long) = 0x0054EA00 (landed; same body over CChangingStateComponent(long) 0x0052CC50) and this
// ??0CViewport@NUISystem@@QAE@ABV01@@Z. Same shape as the landed CSprite copy constructor 0x0041B870.
// Callees: CChangingStateComponent(const CChangingStateComponent&) 0x0052CCA0, CBaseClass::CBaseClass() 0x0099A2D0
// (the handle's root), CViewport::GetDataFromDef() 0x0054E5D0 (Ego_r.pdb ?GetDataFromDef@CViewport@NUISystem@@QAEXXZ;
// landed at that address under that name).
// Shape: the base copy, the three vptrs, then the members in declaration order: Handle (inline CEnginePrimitiveHandle():
// root ctor on `lea edi,[esi+15Ch]`, vtable 0x0122F598, Data = 0), the empty STLport FakeChildren vector, GraphicIndex = 0,
// then GetDataFromDef().
// Layout: retail CChangingStateComponent is 0x15C bytes (RTTI bases CUIInterface @0, CObserver @4, CActionParam @0x18);
// CViewport members at Ego_r.pdb offset - 0x30: Handle +0x15C, FakeChildren +0x164 (STLport vector, 12 bytes),
// GraphicIndex +0x170. No engine header for CViewport (local polymorphic partial view as the landed CSprite rows).
class CIVCountedPointeeBase;
class CEnginePersistentPrimitiveBase;

class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                          // +0x00 (Ego_r.pdb)
    CIVCountedPointer() : Object(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass {          // vtable 0x0122F598
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    CEnginePrimitiveHandle() { }
    virtual ~CEnginePrimitiveHandle();
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}

namespace NUISystem {
class CUIInterface {                                        // RTTI base @0 (primary vtable)
public:
    virtual ~CUIInterface();
};
class CObserver {                                           // RTTI base @4, 0x14 bytes
public:
    virtual ~CObserver();
    unsigned char EventsToObserve[0xc];                     // +0x04 set<NUISystem::EEvent> (STLport, 12 bytes)
    bool PreventObservation;                                // +0x10
};
class CActionParam {                                        // RTTI base @0x18, 8 bytes
public:
    virtual ~CActionParam();
    long ActionParamType;                                   // +0x04 EActionParamType
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {   // partial view
public:
    CComponent(const CComponent& other);
    unsigned char ComponentBody[0x15c - 0x20];              // CComponent / CChangingStateComponent members (not modelled)
};
class CChangingStateComponent : public CComponent {         // partial view, 0x15C bytes
public:
    CChangingStateComponent(const CChangingStateComponent& other);   // 0x0052CCA0
};
class CViewport : public CChangingStateComponent {
public:
    CEnginePrimitiveHandle Handle;                          // +0x15C
    std::vector<CComponent*> FakeChildren;                  // +0x164
    unsigned long GraphicIndex;                             // +0x170
    CViewport(const CViewport& other);
    virtual ~CViewport();
    void GetDataFromDef();                                  // 0x0054E5D0
};

CViewport::CViewport(const CViewport& other)
    : CChangingStateComponent(other),
      GraphicIndex(0)
{
    GetDataFromDef();
}
}
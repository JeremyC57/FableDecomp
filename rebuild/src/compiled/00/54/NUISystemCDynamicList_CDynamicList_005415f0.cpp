// NUISystem::CDynamicList::CDynamicList(long) -- CChangingStateComponent(def) base, the three vtables, the list state and the scrolling-children vector cleared, then GetDataFromDef()
// Leaf: ??0CDynamicList@NUISystem@@QAE@J@Z
// Retail: Fable.exe @ 0x005415F0, 90 bytes, thiscall `ret 4`, returns this.
// Identity: the stored vtables 0x01246E44 / 0x01246E20 / 0x01246E18 are RTTI .?AVCDynamicList@NUISystem@@ (bases
// CChangingStateComponent -> CComponent, CObserver at +4, CActionParam at +0x18); PDB `??0CDynamicList@NUISystem@@QAE@J@Z`;
// caller CManager::CreateComponent(long) 0x0041D783.
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) 0x0052CC50 (`??0CChangingStateComponent@NUISystem@@QAE@J@Z`);
// CDynamicList::GetDataFromDef() 0x00541390 (PDB `?GetDataFromDef@CDynamicList@NUISystem@@IAEXXZ`, protected).
// Layout: Ego_r.pdb CDynamicList minus 0x30 (retail CChangingStateComponent is 0x15C bytes): BInitialised +0x15C,
// PositionOffset +0x160 (left to GetDataFromDef), TopIndex +0x168, SelectionIndex +0x16C, NumVisible +0x170,
// ScrollingChildren vector<CComponent*> +0x174. Bases as RTTI .?AVCDynamicList@NUISystem@@ lists them
// (CChangingStateComponent -> CComponent -> CUIInterface @0, CObserver @4, CActionParam @0x18).
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) {}
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) {}
};
}
namespace NUISystem {
class CUIInterface { public:                     // primary vtable at +0
    virtual void v000();
};
class CObserver { public:                        // vtable at +4, then its 0x10 bytes of state
    virtual void ProcessEvent();
    unsigned char _body[0x10];
};
class CActionParam { public:                     // vtable at +0x18
    virtual void v0();
};
struct C2DVector {
    float X, Y;
};
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
};
class CChangingStateComponent : public CComponent { public:   // retail 0x15C bytes
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long definition);    // 0x0052CC50
};
class CDynamicList : public CChangingStateComponent { public:    // vtables 0x01246E44 / 0x01246E20 / 0x01246E18
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
    CDynamicList(long definition);
    long                      BInitialised;      // +0x15C
    C2DVector                 PositionOffset;    // +0x160
    long                      TopIndex;          // +0x168
    long                      SelectionIndex;    // +0x16C
    long                      NumVisible;        // +0x170
    std::vector<CComponent*>  ScrollingChildren; // +0x174
protected:
    void GetDataFromDef();                       // 0x00541390
};
CDynamicList::CDynamicList(long definition)
    : CChangingStateComponent(definition)
    , BInitialised(0)
    , TopIndex(0)
    , SelectionIndex(0)
    , NumVisible(0)
{
    GetDataFromDef();
}
}
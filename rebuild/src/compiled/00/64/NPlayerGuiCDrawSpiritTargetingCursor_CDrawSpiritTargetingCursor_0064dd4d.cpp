// NPlayerGui::CDrawSpiritTargetingCursor::CDrawSpiritTargetingCursor(CPlayerGui& gui, std::vector<CDrawBase*>& drawers) -- register the spirit targeting cursor drawer and give it four empty targeted-thing slots
// Leaf: ??0CDrawSpiritTargetingCursor@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064DD4D, 77 bytes, thiscall, ebp frame, `leave ; ret 8`, /O1 TU.
// Identity (proven): the vptr stored is 0x0125A5E8, whose RTTI locator 0x0133D028 names .?AVCDrawSpiritTargetingCursor@NPlayerGui@@
//   (the catalog label "CDrawQuickAccessIconsPC" is wrong); Ego_r.pdb ??0CDrawSpiritTargetingCursor@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector...
// Callees: CDrawBase::CDrawBase(CPlayerGui&, vector<CDrawBase*>&) 0x0064CC65, CIntelligentPointer<CThing>::CIntelligentPointer(CThing*)
//   0x004A96F0 (the `_Tp()` of resize: a null pointer), vector<CIntelligentPointer<CThing>>::resize(size_type, const value_type&)
//   0x00656D20, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (the temporary).
// Layout: CDrawBase 0x28 bytes; PTargetedThings (vector<CIntelligentPointer<CThing>>, STLport 0xC) +0x28 (Ego_r.pdb +0x2c).
#pragma optimize("s", on)
class CPlayerGui;
class CThing;

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    void resize(size_type __new_size, const _Tp& __x);
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

namespace NPlayerGui {
class CDrawBase {
public:
    CDrawBase(CPlayerGui& gui, std::vector<CDrawBase*>& drawers);   // 0x0064CC65
    virtual void ClearPrimitivesIfNecessary();     // slot 0
    virtual void EmptyVirtual_00442760();          // slot 1: a folded empty `ret` body (descriptive; never called by name here)
    unsigned char _base[0x28 - 4];
};
}

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();            // 0x00A01C10
    void* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* p = 0);                 // 0x004A96F0 for <CThing>
};

namespace NPlayerGui {

class CDrawSpiritTargetingCursor : public CDrawBase {
public:
    CDrawSpiritTargetingCursor(CPlayerGui& gui, std::vector<CDrawBase*>& drawers);
    virtual void ClearPrimitivesIfNecessary();
    std::vector<CIntelligentPointer<CThing> > PTargetedThings;   // +0x28
};

CDrawSpiritTargetingCursor::CDrawSpiritTargetingCursor(CPlayerGui& gui, std::vector<CDrawBase*>& drawers)
    : CDrawBase(gui, drawers)
{
    CIntelligentPointer<CThing> none;
    PTargetedThings.resize(4, none);
}

}
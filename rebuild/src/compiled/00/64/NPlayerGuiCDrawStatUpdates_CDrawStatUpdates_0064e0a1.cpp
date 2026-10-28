// NPlayerGui::CDrawStatUpdates::CDrawStatUpdates(CPlayerGui& gui, std::vector<CDrawBase*>& drawers) -- register the stat-update drawer with no renown or morality change seen yet, not initialised, and six zeroed previous-stat slots
// Leaf: ??0CDrawStatUpdates@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064E0A1, 57 bytes, thiscall, `ret 8`, /O1 TU.
// Identity (proven): the vptr stored is 0x0125A5F4 (RTTI .?AVCDrawStatUpdates@NPlayerGui@@, slot 0 the landed
//   ClearPrimitivesIfNecessary 0x00644393); Ego_r.pdb ??0CDrawStatUpdates@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector...
// Callees: CDrawBase::CDrawBase(CPlayerGui&, vector<CDrawBase*>&) 0x0064CC65, vector<long>::resize(size_type) 0x004433F0 (out of line).
// Layout (Ego_r.pdb names, retail offsets 4 lower): LastRenownChange +0x28, LastMoralityChange +0x2c, Initialised +0x30,
//   PreviousStats (vector<long>, STLport 0xC) +0x34.
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
    void resize(size_type __new_size);             // 0x004433F0 for <long>
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

namespace NPlayerGui {

class CDrawStatUpdates : public CDrawBase {
public:
    CDrawStatUpdates(CPlayerGui& gui, std::vector<CDrawBase*>& drawers);
    virtual void ClearPrimitivesIfNecessary();
    long              LastRenownChange;            // +0x28
    long              LastMoralityChange;          // +0x2c
    bool              Initialised;                 // +0x30
    std::vector<long> PreviousStats;               // +0x34
};

CDrawStatUpdates::CDrawStatUpdates(CPlayerGui& gui, std::vector<CDrawBase*>& drawers)
    : CDrawBase(gui, drawers), LastRenownChange(0), LastMoralityChange(0), Initialised(false)
{
    PreviousStats.resize(6);
}

}
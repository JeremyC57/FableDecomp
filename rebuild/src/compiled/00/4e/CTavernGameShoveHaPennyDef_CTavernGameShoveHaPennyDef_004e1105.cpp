// CTavernGameShoveHaPennyDef::CTavernGameShoveHaPennyDef() -- the definition's default constructor: CTavernGameDef, the vtable, then the empty member vector
// Leaf: ??0CTavernGameShoveHaPennyDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1105, 34 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x01242524 (RTTI .?AVCTavernGameShoveHaPennyDef@@); the zeroed vector at +0x1a4 is Ego_r.pdb
//   CTavernGameShoveHaPennyDef's vector member Regions (12-byte retail vectors after the 0x1A4-byte CTavernGameDef part).
// Callees: CTavernGameDef::CTavernGameDef() 0x004E1049.
// Shape (family M040, representative 0x00457073): `optimize("s", on)` -- the base constructor out of line, the vtable
//   store, then each STLport vector's inline default constructor (three zero stores through `lea eax,[esi+N]`).
// Header debt: the base is modelled by its retail size, the scalar members before the first vector as opaque bytes
//   (Ego_r.pdb: none).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
class CShoveHaPennyRegionDef;
class CTavernGameDef {
public:
    CTavernGameDef();                                              // 0x004E1049
    virtual ~CTavernGameDef();
    unsigned char _base[0x1a4 - 4];
};
class CTavernGameShoveHaPennyDef : public CTavernGameDef {
public:
    CTavernGameShoveHaPennyDef();
    std::vector<CShoveHaPennyRegionDef> Regions; // +0x1a4
};
CTavernGameShoveHaPennyDef::CTavernGameShoveHaPennyDef()
{
}
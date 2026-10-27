// CBossDef::CBossDef() -- the definition's default constructor: CSubDefClassBase, the vtable, then the empty member vectors
// Leaf: ??0CBossDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE8C2, 47 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x0124185C (RTTI .?AVCBossDef@@); the zeroed vectors at +0x28, +0x34, +0x40 are Ego_r.pdb
//   CBossDef's vector members PhaseHealthPercent, Shots, TwinBladeMissesBeforeLunge (12-byte retail vectors after the 0x25-byte CSubDefClassBase part).
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0.
// Shape (family M041, representative 0x00457973): `optimize("s", on)` -- the base constructor out of line, the vtable
//   store, then each STLport vector's inline default constructor (three zero stores).
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
class CSubDefClassBase {
public:
    CSubDefClassBase();                                              // 0x0044C0C0
    virtual ~CSubDefClassBase();
    unsigned char _base[0x25 - 4];
};
class CBossDef : public CSubDefClassBase {
public:
    CBossDef();
    std::vector<long> PhaseHealthPercent;        // +0x28
    std::vector<long> Shots;                     // +0x34
    std::vector<long> TwinBladeMissesBeforeLunge; // +0x40
};
CBossDef::CBossDef()
{
}
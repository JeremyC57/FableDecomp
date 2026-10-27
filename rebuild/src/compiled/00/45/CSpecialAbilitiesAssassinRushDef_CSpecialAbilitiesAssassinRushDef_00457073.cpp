// CSpecialAbilitiesAssassinRushDef::CSpecialAbilitiesAssassinRushDef() -- the definition's default constructor: CParentDefClassBase, the vtable, then the empty member vector
// Leaf: ??0CSpecialAbilitiesAssassinRushDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00457073, 34 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x0123405C (RTTI .?AVCSpecialAbilitiesAssassinRushDef@@); the zeroed vector at +0x80 is Ego_r.pdb
//   CSpecialAbilitiesAssassinRushDef's vector member DashDistance (12-byte retail vectors after the 0x39-byte CParentDefClassBase part).
// Callees: CParentDefClassBase::CParentDefClassBase() 0x00430370.
// Shape (family M040, representative 0x00457073): `optimize("s", on)` -- the base constructor out of line, the vtable
//   store, then each STLport vector's inline default constructor (three zero stores through `lea eax,[esi+N]`).
// Header debt: the base is modelled by its retail size, the scalar members before the first vector as opaque bytes
//   (Ego_r.pdb: DistanceAwayFromtargetToAssassinRushTo, SafeCollideRadiusPercentage, DistFromFloor, AssassinStartSeconds, AssassinEndSeconds, AssassinThreshold, AssassinLength, AssassinIntensity, AssassinFade, AssassinRange, AssassinTwist, RunSpeed, TrailDelaySecs, SlowTimeLength, AssassinAlpha, SlowTimeMultiplier, RushDelay).
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
class CParentDefClassBase {
public:
    CParentDefClassBase();                                              // 0x00430370
    virtual ~CParentDefClassBase();
    unsigned char _base[0x39 - 4];
};
class CSpecialAbilitiesAssassinRushDef : public CParentDefClassBase {
public:
    CSpecialAbilitiesAssassinRushDef();
    unsigned char _scalars[0x44];                       // +0x3c .. scalars, not constructed here
    std::vector<float> DashDistance;             // +0x80
};
CSpecialAbilitiesAssassinRushDef::CSpecialAbilitiesAssassinRushDef()
{
}
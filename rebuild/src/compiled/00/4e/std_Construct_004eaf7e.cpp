// std::_Construct(CShoveHaPennyRegionDef* p, const CShoveHaPennyRegionDef& value) -- copy-construct the definition record in place
// Leaf: ??$_Construct@VCShoveHaPennyRegionDef@@V1@@std@@YIXPAVCShoveHaPennyRegionDef@@ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EAF7E, 37 bytes, `__fastcall` (p ecx, value edx), plain `ret`.
// Identity (proven): stores vtable 0x0124383C (RTTI .?AVCShoveHaPennyRegionDef@@); the copied members match Ego_r.pdb CShoveHaPennyRegionDef; its callers are vector<CShoveHaPennyRegionDef>'s operator= 0x004EAE1E and _M_allocate_and_copy 0x004EAFE8.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) 0x0046644C (out of line).
// Shape (family M101, representative 0x004EAC30): STLport `new (__p) _T1(__val);` with CShoveHaPennyRegionDef's implicit copy constructor
//   inline: the base copy, the vtable store, then the members through eax -- a size-optimised module, so the member-wise
//   copy constructor is spelled out and `__forceinline` (VC7.1 /Os does not inline it otherwise).
// Header debt: the def base is modelled by its retail size (0x25 bytes, packed) with its out-of-line copy constructor.
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
#pragma pack(push, 1)
class CDefClassBase {
public:
    CDefClassBase(const CDefClassBase& other);                          // 0x0046644C
    virtual ~CDefClassBase();
    unsigned char _base[0x25 - 4];
};
#pragma pack(pop)
class CShoveHaPennyRegionDef : public CDefClassBase {
public:
    __forceinline CShoveHaPennyRegionDef(const CShoveHaPennyRegionDef& other) : CDefClassBase(other), Start(other.Start), Score(other.Score) { }
    float Start;                                          // +0x28
    float Score;                                          // +0x2c
};
namespace std {
template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}
template void __fastcall _Construct<CShoveHaPennyRegionDef, CShoveHaPennyRegionDef>(CShoveHaPennyRegionDef* __p, const CShoveHaPennyRegionDef& __val);
}
// CAppearanceModifierDef::GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>& scaling) const -- Goodstrength scaled by the modifier's scaling type (when the five-entry table is present)
// Leaf: ?GetTypeBalancedGoodstrength@CAppearanceModifierDef@@QBEMABV?$vector@VCAppearanceModifierScalingDef@@V?$allocator@VCAppearanceModifierScalingDef@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057FBF0, 53 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): Ego_r.pdb ?GetTypeBalancedGoodstrength@CAppearanceModifierDef@@QBEM...; the entry field +0x44 and the def member
//   +0x6C are Goodstrength in both Ego_r.pdb layouts (CAppearanceModifierScalingDef - 4, CAppearanceModifierDef - 8: the 12-byte
//   STLport vectors); the three GetTypeBalanced... methods are the consecutive 0x0057FB86 / 0x0057FBBB / 0x0057FBF0.
// Callees: CAppearanceModifierDef::GetModifierScalingType() const 0x0057A43E (landed).
// Shape (family M142, representative 0x0057FB86): `if (scaling.size() == 5) return scaling[GetModifierScalingType()].Goodstrength *
//   Goodstrength; return Goodstrength;` -- size() as the signed 72-byte divide (`push 48h ; pop ecx ; cdq ; idiv`), `optimize("s", on)`.
// Header debt: both defs modelled by the retail offsets of the members read.
#pragma optimize("s", on)
enum EAppearanceModifierScalingType { APPEARANCE_MODIFIER_SCALING_TYPE_0 = 0 };
class CAppearanceModifierScalingDef {                       // 72 bytes in retail (Ego_r.pdb 76)
public:
    unsigned char _pad00[0x3C];
    float Attractiveness;                                   // +0x3C
    float Scariness;                                        // +0x40
    float Goodstrength;                                     // +0x44
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CAppearanceModifierDef {
public:
    float GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>& scaling) const;
private:
    __declspec(noinline) EAppearanceModifierScalingType GetModifierScalingType() const   // 0x0057A43E (the landed body, read-only)
    {
        if (SuitPart == 0) {
            switch (CoversBodyAreaFlags) {
            case 0x10:  return (EAppearanceModifierScalingType)1;
            case 0x80:  return (EAppearanceModifierScalingType)2;
            case 0x100: return (EAppearanceModifierScalingType)3;
            }
        }
        return APPEARANCE_MODIFIER_SCALING_TYPE_0;
    }
public:
    unsigned char _pad00[0x40];
    long CoversBodyAreaFlags;                               // +0x40
    unsigned char _pad44[0xC];
    long SuitPart;                                          // +0x50
    unsigned char _pad54[0x10];
    float Scariness;                                        // +0x64
    float Attractiveness;                                   // +0x68
    float Goodstrength;                                     // +0x6C
};
float CAppearanceModifierDef::GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>& scaling) const
{
    if (scaling.size() == 5)
        return scaling[GetModifierScalingType()].Goodstrength * Goodstrength;
    return Goodstrength;
}
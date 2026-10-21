// CTattooDef::GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>&) const -- attractiveness scaled by the tattoo-type entry when all five type scalings exist
// Leaf: ?GetTypeBalancedAttractiveness@CTattooDef_Methods@@QBEMABV?$vector@VCAppearanceModifierScalingDef@@@std@@@Z
// Retail: Fable.exe @ 0x0057FC46, 43 bytes, `ret 4`, result on the x87 stack; Ego_r.pdb
// ?GetTypeBalancedAttractiveness@CTattooDef@@QBEMABV?$vector@VCAppearanceModifierScalingDef@@...@Z. The siblings
// GetTypeBalancedScariness (0x0057FC71, +0x58 / entry +0x40) and GetTypeBalancedGoodstrength (0x0057FC9C, +0x5c / entry
// +0x44) follow with the same shape. Body: if the vector holds 5 entries, scalings[4].Attractiveness * Attractiveness,
// else Attractiveness.
// Layout: engine/CTattooDef.h (Attractiveness +0x54, as Ego_r.pdb). CAppearanceModifierScalingDef is 0x48 bytes in
// retail (`idiv 48h`; Ego_r.pdb 0x4c) with Attractiveness at +0x3c (entry 4 at +0x15c; Ego_r.pdb +0x40), the same
// 4-byte-smaller def prefix as the other retail defs, so it is local. The vector is STLport's 12-byte {start, finish, eos}.
// Opt: size TU (unaligned entry, `push 48h ; pop edi` for the divisor).
#pragma optimize("s", on)
#include "engine/CTattooDef.h"
class CAppearanceModifierScalingDef {
public:
    unsigned char _base[0x3c];                           // CDefClassBase and the leading members
    float         Attractiveness;                        // +0x3c
    float         Scariness;                             // +0x40
    float         Goodstrength;                          // +0x44
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
}
struct CTattooDef_Methods : CTattooDef {
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>& scalings) const;
};
float CTattooDef_Methods::GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>& scalings) const
{
    if (scalings.size() == 5)
        return scalings[4].Attractiveness * Attractiveness;
    return Attractiveness;
}
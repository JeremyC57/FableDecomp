// CTattooDef::GetTypeBalancedGoodstrength(const vector<CAppearanceModifierScalingDef>& scalings) -- the tattoo's Goodstrength scaled by the tattoo entry (index 4) of the appearance-modifier scaling table when the table holds all five entries, else the raw Goodstrength
// Leaf: GetTypeBalancedGoodstrength
// Retail: Fable.exe @ 0x0057fc9c, 43 bytes (no padding: a size-optimised def module). No callees.
// Pragma: #pragma optimize("s", on) -- `push 48h ; cdq ; pop edi ; idiv edi` for the element-count division and the
// unaligned function start are the /Os module shape.
// Label: the manifest gives all three siblings 0x0057FC46 / 0x0057FC71 / 0x0057FC9C the name GetTypeBalancedAttractiveness;
// only 0x0057FC46 reads Attractiveness (+0x54). This one reads Goodstrength (+0x5c) -- CTattooDef's PDB member at that
// offset -- and its caller (0x0057D488 / 0x0057D49B) sums the three results into separate totals.
// Layout: engine/CTattooDef.h (Attractiveness +0x54, Scariness +0x58, Goodstrength +0x5c). The retail scaling def is
// 0x48 bytes (the element stride of the division): the 0x3c-byte CParentDefClassBase (see the landed copy constructor
// 0x004784B3) then the PDB's Attractiveness / Scariness / Goodstrength floats, so element 4's Goodstrength is at +0x164
// from the vector's first element. The VC6-era vector keeps first/last at +0/+4.
// Shape: `fld [element] ; fmul [this]` is `Goodstrength * element.Goodstrength`; the size test is the signed pointer
// difference divided by 0x48 compared with 5.
#pragma optimize("s", on)
#include "engine/CTattooDef.h"

class CAppearanceModifierScalingDef { public:             // retail 0x48 bytes (PDB 0x4c)
    unsigned char _base[0x3c];                     // +0x00 CParentDefClassBase (vptr + def header)
    float         Attractiveness;                  // +0x3c
    float         Scariness;                       // +0x40
    float         Goodstrength;                    // +0x44
};

template <class T> struct CRetailVector {
    T* _First;                                     // +0x00
    T* _Last;                                      // +0x04
    T* _End;                                       // +0x08
    unsigned long size() const { return _Last - _First; }
    const T& operator[](unsigned long index) const { return _First[index]; }
};

enum { APPEARANCE_MODIFIER_TYPE_COUNT = 5, APPEARANCE_MODIFIER_TATTOO = 4 };

struct CTattooDef_Methods : CTattooDef {
    float GetTypeBalancedGoodstrength(const CRetailVector<CAppearanceModifierScalingDef>& scalings) const;
};

float CTattooDef_Methods::GetTypeBalancedGoodstrength(const CRetailVector<CAppearanceModifierScalingDef>& scalings) const
{
    if (scalings.size() == APPEARANCE_MODIFIER_TYPE_COUNT)
        return Goodstrength * scalings[APPEARANCE_MODIFIER_TATTOO].Goodstrength;
    return Goodstrength;
}
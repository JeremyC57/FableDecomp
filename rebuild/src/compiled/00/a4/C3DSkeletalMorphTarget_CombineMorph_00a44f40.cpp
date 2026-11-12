// C3DSkeletalMorphTarget::CombineMorph(const C3DSkeletalMorphTarget&, float) -- merge another target's bone scalings into this one (both sorted by bone-name CRC): matching bones add (scaling - 1) * weight, missing bones are inserted at the cursor as (1,1,1) + (scaling - 1) * weight
// Leaf: ?CombineMorph@C3DSkeletalMorphTarget@@QAEXABV1@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A44F40, 525 bytes (ret 8). Identity: Ego_r.pdb 0x00BA0000 / FableWin.pdb 0x0321CE20, same mangled name.
// Callees: CCharString::CCharString() 0x0099E4B0, CCharString::~CCharString() 0x0099EAE0,
//   std::vector<C3DSkeletalBoneMorph>::insert(iterator, const T&) 0x00A46B50 (out of line: FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE).
//   Constant 1.0f @ 0x0122DED8.
// Layout: C3DSkeletalMorphTarget (Ego_r) Name CCharString +0x00, BoneMorphs CArray<C3DSkeletalBoneMorph> +0x04 (12-byte STLport vector);
//   C3DSkeletalBoneMorph (Ego_r, 20 bytes) BoneName +0x00, BoneNameCRC +0x04, Scaling C3DVector +0x08. No shared header exists for either.
// Shape notes (each load-bearing): the source CRC is a named local at the top of the loop body (retail reloads it from the source
//   element in the inner loop and loads it first in the hit test: `mov ecx,[ebx+4] ; cmp [eax+4],ecx`); the miss arm re-binds the
//   source element in its own scope before constructing the new bone morph (retail keeps the default ctor's dead `BoneNameCRC = 0`
//   store and emits the CRC / (1,1,1) stores before any source float load -- VC7.1 treats a pointer bound after another local's
//   scope opens as possibly aliasing it); `++i` is its own statement after the insert.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"

class C3DSkeletalBoneMorph                          // Ego_r.pdb, 0x14 bytes
{
public:
    CCharString BoneName;                           // +0x00
    unsigned long BoneNameCRC;                      // +0x04
    C3DVector Scaling;                              // +0x08
    C3DSkeletalBoneMorph() : BoneNameCRC(0) { }
};

class C3DSkeletalMorphTarget
{
public:
    CCharString Name;                               // +0x00
    CArray<C3DSkeletalBoneMorph> BoneMorphs;        // +0x04
    void CombineMorph(const C3DSkeletalMorphTarget& target, float weight);
};

void C3DSkeletalMorphTarget::CombineMorph(const C3DSkeletalMorphTarget& target, float weight)
{
    unsigned long i = 0;
    for (unsigned long t = 0; t < target.BoneMorphs.size(); ++t)
    {
        const C3DSkeletalBoneMorph& source = target.BoneMorphs[t];
        const unsigned long crc = source.BoneNameCRC;
        while (i < BoneMorphs.size() && BoneMorphs[i].BoneNameCRC < crc)
            ++i;
        if (i < BoneMorphs.size() && BoneMorphs[i].BoneNameCRC == crc)
        {
            BoneMorphs[i].Scaling += (source.Scaling - C3DVector(1.0f, 1.0f, 1.0f)) * weight;
        }
        else
        {
            const C3DSkeletalBoneMorph& added = target.BoneMorphs[t];
            C3DSkeletalBoneMorph morph;
            morph.BoneNameCRC = added.BoneNameCRC;
            morph.Scaling = C3DVector(1.0f, 1.0f, 1.0f);
            morph.Scaling += (added.Scaling - C3DVector(1.0f, 1.0f, 1.0f)) * weight;
            BoneMorphs.insert(BoneMorphs.begin() + i, morph);
            ++i;
        }
    }
}
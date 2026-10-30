// CSpecialAbilitiesMultiArrowDef::operator=(const CSpecialAbilitiesMultiArrowDef& other) -- memberwise copy: CParentDefClassBase part, then NumberOfShots[4], NumberInVolley[4], AmmoEffectCreationDelaySecs, CastSound, BowHighlightWidth, BowHighlightColor (vector<long>), MultiArrowPerArrowDamageMultiplier (vector<float>); returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045F7C7, length 111 bytes.
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886, vector<float>::operator= @ 0x0045EF2D
// (catalog "CActiveFile::operator=" / "CCombatWheel::operator=" are propagated labels).
// Identity: slot 19 (Copy) of CSpecialAbilitiesMultiArrowDef's vtable (0x01234194, rtti-vtables.tsv) is the 5-byte `jmp` right before this body.
// Layout: Ego_r.pdb members shifted 4 bytes down (retail CParentDefClassBase is 0x39 bytes, vectors 12 bytes), arrays at +0x3c/+0x4c.
// TU: `optimize("y", off)` + `("s", on)` (ebp frame): the two 4-element arrays copy as explicit element `for` loops through ebx,
// the shared stride 4 in edx and the count homed in the dead `other` argument slot (`dec dword ptr [ebp+8]`).
// Leaf: operator=
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

class CDefString { public: long TablePos; };

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first; long* m_last; long* m_end;
    CLongVector& operator=(const CLongVector& other);     // 0x00454886
};
struct CFloatVector {                   // retail std::vector<float> (12 bytes)
    float* m_first; float* m_last; float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};

class CSpecialAbilitiesMultiArrowDef : public CParentDefClassBase { public:
    long         NumberOfShots[4];                      // +0x3c
    long         NumberInVolley[4];                     // +0x4c
    float        AmmoEffectCreationDelaySecs;           // +0x5c
    CDefString   CastSound;                             // +0x60
    float        BowHighlightWidth;                     // +0x64
    CLongVector  BowHighlightColor;                     // +0x68
    CFloatVector MultiArrowPerArrowDamageMultiplier;    // +0x74
    CSpecialAbilitiesMultiArrowDef& operator=(const CSpecialAbilitiesMultiArrowDef& other);
};

CSpecialAbilitiesMultiArrowDef& CSpecialAbilitiesMultiArrowDef::operator=(const CSpecialAbilitiesMultiArrowDef& other)
{
    CParentDefClassBase::operator=(other);
    for (int i = 0; i < 4; ++i)
        NumberOfShots[i] = other.NumberOfShots[i];
    for (int j = 0; j < 4; ++j)
        NumberInVolley[j] = other.NumberInVolley[j];
    AmmoEffectCreationDelaySecs = other.AmmoEffectCreationDelaySecs;
    CastSound = other.CastSound;
    BowHighlightWidth = other.BowHighlightWidth;
    BowHighlightColor = other.BowHighlightColor;
    MultiArrowPerArrowDamageMultiplier = other.MultiArrowPerArrowDamageMultiplier;
    return *this;
}
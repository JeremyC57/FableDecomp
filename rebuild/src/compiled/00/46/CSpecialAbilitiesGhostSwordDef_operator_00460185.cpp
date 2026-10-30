// CSpecialAbilitiesGhostSwordDef::operator=(const CSpecialAbilitiesGhostSwordDef&) -- memberwise copy: CParentDefClassBase part, the four per-level summon times (x87 element loop) and three floats; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460185, length 60 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised.
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. CSpecialAbilitiesGhostSwordDef's
// vtable (0x012344D4) slot 19 (Copy) is the 5-byte `jmp 0x00460185` right before this body, and the PDB
// layout (TimeOfSummon float[4] +0x40, FadeInTime +0x50, FadeOutTime +0x54, CreationDistance +0x58) is
// the retail one shifted by the 4-byte-smaller retail SubDefInfoMap.
// Shape: the float ARRAY is copied element by element through the FPU (`fld [ecx+eax] ; fstp [eax]`
// with the source addressed as a distance from the destination and a `dec edx` counter of 4), while
// the scalar floats copy as dword moves.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes); the retail base is 0x39 bytes.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

class CSpecialAbilitiesGhostSwordDef : public CParentDefClassBase { public:   // vtable 0x012344D4
    float TimeOfSummon[4];                  // +0x3c one per ability level
    float FadeInTime;                       // +0x4c
    float FadeOutTime;                      // +0x50
    float CreationDistance;                 // +0x54
    CSpecialAbilitiesGhostSwordDef& operator=(const CSpecialAbilitiesGhostSwordDef& other);
};

CSpecialAbilitiesGhostSwordDef& CSpecialAbilitiesGhostSwordDef::operator=(const CSpecialAbilitiesGhostSwordDef& other)
{
    CParentDefClassBase::operator=(other);
    for (int i = 0; i < 4; ++i)
        TimeOfSummon[i] = other.TimeOfSummon[i];
    FadeInTime = other.FadeInTime;
    FadeOutTime = other.FadeOutTime;
    CreationDistance = other.CreationDistance;
    return *this;
}
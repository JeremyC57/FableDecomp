// CSpecialAbilitiesHealLifeDef::operator=(const CSpecialAbilitiesHealLifeDef&) -- memberwise copy: CParentDefClassBase part and three per-level float vectors; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460131, length 57 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D (three times).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesHealLifeDef's vtable 0x0123446C is the 5-byte `jmp 0x00460131` right before this body, and
// the PDB layout of CSpecialAbilitiesHealLifeDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x40.. -> retail +0x3c..; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally as a packed polymorphic base
// whose operator= is out of line; the derived members follow at their natural alignment.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

struct CFloatVector {                   // retail std::vector<float> (12 bytes)
    float* m_first;
    float* m_last;
    float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};

class CSpecialAbilitiesHealLifeDef : public CParentDefClassBase { public: // vtable 0x0123446C
    CFloatVector         HealthIncreasePerFrame;      // +0x3c
    CFloatVector         TotalHealthIncrease;         // +0x48
    CFloatVector         RadiusOfEffect;              // +0x54
    CSpecialAbilitiesHealLifeDef& operator=(const CSpecialAbilitiesHealLifeDef& other);
};

CSpecialAbilitiesHealLifeDef& CSpecialAbilitiesHealLifeDef::operator=(const CSpecialAbilitiesHealLifeDef& other)
{
    CParentDefClassBase::operator=(other);
    HealthIncreasePerFrame = other.HealthIncreasePerFrame;
    TotalHealthIncrease = other.TotalHealthIncrease;
    RadiusOfEffect = other.RadiusOfEffect;
    return *this;
}
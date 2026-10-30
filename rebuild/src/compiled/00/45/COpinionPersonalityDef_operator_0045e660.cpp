// COpinionPersonalityDef::operator=(const COpinionPersonalityDef&) -- copies the default flag, the per-attitude enable map and the hit tolerance, then the owned personality-trait table in place; the base part is NOT assigned; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E660, length 52 bytes.
// Pragma: #pragma optimize("s", on) -- `push 2Dh ; pop ecx` before the `rep movsd`.
// Callees: map<EOpinionAttitudeType,bool>::operator= @ 0x0045E694 (manifest "CTCCarryable_AssignClonedTree4", propagated).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. COpinionPersonalityDef's vtable
// (0x01233F8C) slot 19 (Copy) is the 5-byte `jmp 0x0045E660` right before this body, and the PDB layout
// (PersonalityTraits pointer +0x40, AttitudeEnabledDefaultForNotNone +0x44, AttitudeEnabled map +0x48,
// ToleranceToBeingHit +0x58) is the retail one (+0x3c, +0x40, +0x44, +0x50) with the retail 12-byte
// SubDefInfoMap and map.
// Shape: a hand-written assignment (the class owns the trait table through a pointer): there is no
// call of CParentDefClassBase::operator=, and the table is copied last, through both pointers, as one
// 0xb4-byte block (`rep movsd` of 0x2d dwords) -- five 36-byte COpinionPersonalityTraits records (the
// PDB record is nine floats), one per opinion attitude.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes); the retail base is 0x39 bytes.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

class COpinionPersonalityTraits { public:          // PDB: nine floats (36 bytes)
    float BaseOffset;
    float HeroStatScaling;
    float VillageOpinionScaling;
    float TimeScalingRunIn;
    float TimeScalingRunOut;
    float PeakScalingIfPos;
    float PeakScalingIfNeg;
    float PersistScalingIfPos;
    float PersistScalingIfNeg;
};

struct CPersonalityTraitTable {             // the owned table: one record per attitude (0xb4 bytes)
    COpinionPersonalityTraits Attitude[5];
};

struct CAttitudeEnabledMap {                // retail map<EOpinionAttitudeType,bool> (12 bytes)
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CAttitudeEnabledMap& operator=(const CAttitudeEnabledMap& other);   // 0x0045E694
};

class COpinionPersonalityDef : public CParentDefClassBase { public:   // vtable 0x01233F8C
    CPersonalityTraitTable* PersonalityTraits;          // +0x3c owned
    bool                    AttitudeEnabledDefaultForNotNone;   // +0x40
    CAttitudeEnabledMap     AttitudeEnabled;            // +0x44
    float                   ToleranceToBeingHit;        // +0x50
    COpinionPersonalityDef& operator=(const COpinionPersonalityDef& other);
};

COpinionPersonalityDef& COpinionPersonalityDef::operator=(const COpinionPersonalityDef& other)
{
    AttitudeEnabledDefaultForNotNone = other.AttitudeEnabledDefaultForNotNone;
    AttitudeEnabled = other.AttitudeEnabled;
    ToleranceToBeingHit = other.ToleranceToBeingHit;
    *PersonalityTraits = *other.PersonalityTraits;
    return *this;
}
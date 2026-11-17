// CSpecialAbilitiesTurncoatSpellDef::operator=(const CSpecialAbilitiesTurncoatSpellDef&) -- memberwise copy: CParentDefClassBase part and five per-level float vectors; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004601EA, length 81 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D (five times).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesTurncoatSpellDef's vtable 0x0123453C is the 5-byte `jmp 0x004601EA` right before this body, and
// the PDB layout of CSpecialAbilitiesTurncoatSpellDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x40.. -> retail +0x3c..; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally as a packed polymorphic base
// whose operator= is out of line; the derived members follow at their natural alignment.
// NF48 (round 7): the member vectors are STLport 4.6.2 std::vector<float> (was CFloatVector; operator= 0x0045EF2D) from the shared stlport/fable_containers.h
// (local stand-ins before); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares the out-of-line operator=.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)


class CSpecialAbilitiesTurncoatSpellDef : public CParentDefClassBase { public: // vtable 0x0123453C
    std::vector<float>         StaminaCostPerSec;           // +0x3c
    std::vector<float>         ResistanceDamagePerSec;      // +0x48
    std::vector<float>         TurncoatTime;                // +0x54
    std::vector<float>         TurncoatRange;               // +0x60
    std::vector<float>         TurncoatXPUpEveryNSecs;      // +0x6c
    CSpecialAbilitiesTurncoatSpellDef& operator=(const CSpecialAbilitiesTurncoatSpellDef& other);
};

CSpecialAbilitiesTurncoatSpellDef& CSpecialAbilitiesTurncoatSpellDef::operator=(const CSpecialAbilitiesTurncoatSpellDef& other)
{
    CParentDefClassBase::operator=(other);
    StaminaCostPerSec = other.StaminaCostPerSec;
    ResistanceDamagePerSec = other.ResistanceDamagePerSec;
    TurncoatTime = other.TurncoatTime;
    TurncoatRange = other.TurncoatRange;
    TurncoatXPUpEveryNSecs = other.TurncoatXPUpEveryNSecs;
    return *this;
}
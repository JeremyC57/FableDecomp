// CSpecialAbilitiesCreatureTintDef::operator=(const CSpecialAbilitiesCreatureTintDef&) -- memberwise copy: CParentDefClassBase part, two fade times and the per-level colour vector; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460A19, length 45 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesCreatureTintDef's vtable 0x0123494C is the 5-byte `jmp 0x00460A19` right before this body, and
// the PDB layout of CSpecialAbilitiesCreatureTintDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x40.. -> retail +0x3c..; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
// `add edi,44h` forms the last member's address and the +0x40 store sinks below it (source order).
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally as a packed polymorphic base
// whose operator= is out of line; the derived members follow at their natural alignment.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#include "/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig3/work/x/chain.h"
#pragma pack(pop)

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

class CSpecialAbilitiesCreatureTintDef : public CParentDefClassBase { public: // vtable 0x0123494C
    float                FadeInTimeSecs;              // +0x3c
    float                FadeOutTimeSecs;             // +0x40
    CLongVector          Color;                       // +0x44
    CSpecialAbilitiesCreatureTintDef& operator=(const CSpecialAbilitiesCreatureTintDef& other);
};

CSpecialAbilitiesCreatureTintDef& CSpecialAbilitiesCreatureTintDef::operator=(const CSpecialAbilitiesCreatureTintDef& other)
{
    CParentDefClassBase::operator=(other);
    FadeInTimeSecs = other.FadeInTimeSecs;
    FadeOutTimeSecs = other.FadeOutTimeSecs;
    Color = other.Color;
    return *this;
}
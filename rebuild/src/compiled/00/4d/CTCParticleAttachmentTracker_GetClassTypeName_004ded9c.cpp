// CTCParticleAttachmentTracker::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCParticleAttachmentTracker::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCParticleAttachmentTracker@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DED9C, 16 bytes: mov ecx,[esp+4]; call CTCParticleAttachmentTracker::StatGetClassTypeName 0x004D5B5E; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCParticleAttachmentTracker@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01241AB4 (RTTI .?AVCTCParticleAttachmentTracker@@) slot 22 (0x01241B0C -> 0x004DED9C) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5B5E, the landed CTCParticleAttachmentTracker::StatGetClassTypeName (?StatGetClassTypeName@CTCParticleAttachmentTracker@@SI?AVCCharString@@XZ; literal "CTCParticleAttachmentTracker").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCParticleAttachmentTracker_GetClassTypeName@4 calling the made-up ?Init_004d5b5e@T@@QAEXXZ.
// Header: no engine header defines CTCParticleAttachmentTracker; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCParticleAttachmentTracker@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCParticleAttachmentTracker : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5B5E
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCParticleAttachmentTracker::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

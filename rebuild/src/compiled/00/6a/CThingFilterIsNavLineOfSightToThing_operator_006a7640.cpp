// CThingFilter_IsNavLineOfSightToThing::operator()(CThing* thing) -- true when the creature-navigation component reports a navigation line of sight from Pos to the thing's position
// Leaf: ??RCThingFilter_IsNavLineOfSightToThing
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7640, length 28 bytes.
// Callees: CThing::GetPos @ 0x004C73D0 (out of line), CTCCreatureNavigation line-of-sight query @ 0x006CB610
// (`__fastcall(const C3DVector* from, const C3DVector& to)`; the manifest's "CScriptThing::operator==" is propagated).
// Layout: PDB CThingFilter_IsNavLineOfSightToThing { C3DVector* Pos; CTCCreatureNavigation* PTCNav; }.
// Callee respelled (NF154, PRIORITY q5159): 0x006CB610 is ?IsPosNavLineOfSightToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0@Z (Ego_r.pdb
//   0x007010D0 / FableWin.pdb 0x02477C30: public const, two const C3DVector&; a PNavigator slot-10 forward); was the stand-in
//   ?IsNavLineOfSight@CTCCreatureNavigation@@QAE_NPBVC3DVector@@ABV2@@Z. The local model's declaration is respelled in place.
// Shape: right-to-left: the thing's position first, then PTCNav into ecx, the pushes, Pos last.
#include "engine/C3DVector.h"

#include "engine/CThing.h"

class CTCCreatureNavigation { public:
    bool IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const;   // 0x006CB610
};

class CThingFilter_IsNavLineOfSightToThing { public:
    C3DVector*             Pos;                     // +0x00
    CTCCreatureNavigation* PTCNav;                  // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsNavLineOfSightToThing::operator()(CThing* thing) const
{
    return PTCNav->IsPosNavLineOfSightToPos(*Pos, thing->GetPos());
}
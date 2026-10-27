// CThingPhysical::IsSolidToThing(const CThingPhysical* thing) const -- a physical thing is solid to every thing except itself
// Leaf: ?IsSolidToThing@CThingPhysical@@UBE_NPBV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529720, 14 bytes, `ret 4`: xor eax,eax ; cmp edx,ecx ; setne al.
// Identity: CThingPhysical vtable 0x01244E84 slot 46 (inherited unchanged by 13 more physical-thing vtables). Ego_r.pdb's
//   only CThingPhysical bool virtual taking one pointer is ?IsSolidToThing@CThingPhysical@@UBE_NPBV1@@Z (const
//   CThingPhysical*), and the body compares that pointer with `this` -- probable (signature + semantics; the PDB gives
//   no slot numbers). The catalog "CThingPhysical::vfunc_46" is the slot name.
// Shape: `return thing != this;` -- a bool result built with `xor eax,eax ; setne al` (no member read).
// Layout: engine/CThingPhysical.h through the layout-rename include; the header carries the vptr as its __vftable data
//   member, so the virtual declared here adds VC's own vptr in front of it (no member is read: the bytes do not depend
//   on it -- header debt, see NOTES.md).
#define CThingPhysical CThingPhysicalDonor
#include "engine/CThingPhysical.h"
#undef CThingPhysical

class CThingPhysical : public CThingPhysicalDonor {
public:
    virtual bool IsSolidToThing(const CThingPhysical* thing) const;   // slot 46 (+0xB8)
};

bool CThingPhysical::IsSolidToThing(const CThingPhysical* thing) const
{
    return thing != this;
}
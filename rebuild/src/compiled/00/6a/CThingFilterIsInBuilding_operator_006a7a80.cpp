// CThingFilter_IsInBuilding::operator()(CThing* thing) -- true when the thing's position lies inside the filter's Building (CThingBuilding::IsPointInside)
// Leaf: ??RCThingFilter_IsInBuilding
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7A80, length 24 bytes.
// Callees: CThing::GetPos @ 0x004C73D0 (`PhysicsTC + 0x0C`, out of line; the manifest's "DoSizeof" label is
// propagated), CThingBuilding::IsPointInside @ 0x0082D000.
// Layout: PDB CThingFilter_IsInBuilding { CThingBuilding* Building; } (+0).
// Shape: the argument is evaluated before the object pointer (`call GetPos ; mov ecx,[esi] ; push eax`).
#include "engine/C3DVector.h"

#include "engine/CThing.h"

class CThingBuilding { public:
public:
    bool IsPointInside(const C3DVector&) const;   // 0x0082D000
};

struct CThingFilter_IsInBuilding {
    CThingBuilding* Building;                       // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsInBuilding::operator()(CThing* thing) const
{
    return Building->IsPointInside(thing->GetPos());
}
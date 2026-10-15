// CTCPerceiveThing::SetExtendedSightRadius(float radius) -- an explicit radius is stored as is; -1 means "use the def's ExtendedSightRadius, but never less than SightRadius"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744500, 57 bytes (catalogued as SetSightRadius; it writes ExtendedSightRadius +0x44 -- the
//   0x00744540 twin writes SightRadius +0x40). No callees. 0x0122DEE0 = -1.0f.
// Layout: engine/CTCPerceiveThing.h (PDef_Object +0x34, SightRadius +0x40, ExtendedSightRadius +0x44) and
//   engine/CPerceivedThingDef.h (ExtendedSightRadius +0x34).
// Shape: `radius != -1` (fucompp, `test ah,44h ; jnp`) with its own return; the def value stored with `fst` and compared
//   against SightRadius (`test ah,5 ; jp` = `<`), the clamp copying SightRadius as a dword.
// Leaf: SetExtendedSightRadius
#include "engine/CTCPerceiveThing.h"
#include "engine/CPerceivedThingDef.h"

extern float g_fMinusOne;                   // 0x0122DEE0 (-1.0f)

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    const CPerceivedThingDef* GetDef() const { return reinterpret_cast<const CPerceivedThingDef*>(PDef_Object); }
    void SetExtendedSightRadius(float radius);
};

void CTCPerceiveThing_Methods::SetExtendedSightRadius(float radius)
{
    if (radius != g_fMinusOne) {
        ExtendedSightRadius = radius;
        return;
    }
    ExtendedSightRadius = GetDef()->ExtendedSightRadius;
    if (ExtendedSightRadius < SightRadius)
        ExtendedSightRadius = SightRadius;
}

// CTCPerceiveThing::SetSightRadius(float radius) -- an explicit radius is stored as is; -1 means "use the def's SightRadius" and raises ExtendedSightRadius to it when it is larger
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744540, 59 bytes. No callees. 0x0122DEE0 = -1.0f.
// Layout: engine/CTCPerceiveThing.h (PDef_Object +0x34, SightRadius +0x40, ExtendedSightRadius +0x44) and
//   engine/CPerceivedThingDef.h (SightRadius +0x30).
// Shape: `radius != -1` with its own return; the def value compared against ExtendedSightRadius with `fcom` before the `fst`
//   into SightRadius, the larger value stored from the x87 (`fstp`) or popped (`fstp st(0)`): the comparison uses the def value.
// Leaf: SetSightRadius
#include "engine/CTCPerceiveThing.h"
#include "engine/CPerceivedThingDef.h"

extern float g_fMinusOne;                   // 0x0122DEE0 (-1.0f)

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    const CPerceivedThingDef* GetDef() const { return reinterpret_cast<const CPerceivedThingDef*>(PDef_Object); }
    void SetSightRadius(float radius);
};

void CTCPerceiveThing_Methods::SetSightRadius(float radius)
{
    if (radius != g_fMinusOne) {
        SightRadius = radius;
        return;
    }
    SightRadius = GetDef()->SightRadius;
    if (SightRadius > ExtendedSightRadius)
        ExtendedSightRadius = SightRadius;
}
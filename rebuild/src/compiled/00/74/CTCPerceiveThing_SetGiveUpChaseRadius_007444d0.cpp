// CTCPerceiveThing::SetGiveUpChaseRadius(float radius) -- Use the definition default for the -1 sentinel.
// Leaf: ?SetGiveUpChaseRadius@CTCPerceiveThing@@QAEXM@Z
// Layout: shared CTCPerceiveThing header preserves PDef_Object+0x34 and GiveUpChaseRadius+0x4c; CPerceivedThingDef::GiveUpChaseRadius+0x2c is independently confirmed by retail loads.
#define CTCPerceiveThing CTCPerceiveThing_HeaderLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
#include "engine/CPerceivedThingDef.h"
class CTCPerceiveThing : public CTCPerceiveThing_HeaderLayout {
public: void SetGiveUpChaseRadius(float radius);
};
void CTCPerceiveThing::SetGiveUpChaseRadius(float radius) {
    if (radius != -1.0f) GiveUpChaseRadius=radius;
    else GiveUpChaseRadius=reinterpret_cast<const CPerceivedThingDef*>(PDef_Object)->GiveUpChaseRadius;
}
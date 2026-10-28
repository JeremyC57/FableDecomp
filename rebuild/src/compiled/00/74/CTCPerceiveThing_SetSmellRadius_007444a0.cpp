// CTCPerceiveThing::SetSmellRadius(float radius) -- Use the definition default for the -1 sentinel.
// Leaf: ?SetSmellRadius@CTCPerceiveThing@@QAEXM@Z
// Layout: shared CTCPerceiveThing header preserves PDef_Object+0x34 and SmellRadius+0x48; CPerceivedThingDef::SmellRadius+0x3c is independently confirmed by retail loads.
#define CTCPerceiveThing CTCPerceiveThing_HeaderLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
#include "engine/CPerceivedThingDef.h"
class CTCPerceiveThing : public CTCPerceiveThing_HeaderLayout {
public: void SetSmellRadius(float radius);
};
void CTCPerceiveThing::SetSmellRadius(float radius) {
    if (radius != -1.0f) SmellRadius=radius;
    else SmellRadius=reinterpret_cast<const CPerceivedThingDef*>(PDef_Object)->SmellRadius;
}
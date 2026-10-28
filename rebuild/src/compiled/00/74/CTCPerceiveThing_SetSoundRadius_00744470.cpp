// CTCPerceiveThing::SetSoundRadius(float radius) -- Use the definition default for the -1 sentinel.
// Leaf: ?SetSoundRadius@CTCPerceiveThing@@QAEXM@Z
// Layout: shared CTCPerceiveThing header preserves PDef_Object+0x34 and SoundRadius+0x3c; CPerceivedThingDef::SoundRadius+0x38 is independently confirmed by retail loads.
#define CTCPerceiveThing CTCPerceiveThing_HeaderLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
#include "engine/CPerceivedThingDef.h"
class CTCPerceiveThing : public CTCPerceiveThing_HeaderLayout {
public: void SetSoundRadius(float radius);
};
void CTCPerceiveThing::SetSoundRadius(float radius) {
    if (radius != -1.0f) SoundRadius=radius;
    else SoundRadius=reinterpret_cast<const CPerceivedThingDef*>(PDef_Object)->SoundRadius;
}
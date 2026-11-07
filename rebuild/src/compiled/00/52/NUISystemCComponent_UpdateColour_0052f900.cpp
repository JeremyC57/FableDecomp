// NUISystem::CComponent::UpdateColour(float deltaTime) -- advance the colour transition (eased from the initial to the target colour over ColourTime, clamped, snapping to the target at the end), then rebuild the render colour (own colour times the parent colour unless the component is independent)
// Leaf: ?UpdateColour@CComponent@NUISystem@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F900, 1730 bytes, thiscall `ret 4`, frame `sub esp,0x58 ; push esi`.
// Identity (proven): (1) slot 40 (+0xA0) of the NUISystem::CSprite vtable 0x0122F5D4 is the CComponent slot UpdateColour
//   (engine/NUISystem_CComponent.h, slot 40 UpdateColour(float)); (2) Ego_r.pdb + FableWin.pdb ?UpdateColour@CComponent@NUISystem@@UAEXM@Z,
//   in both PDBs between UpdateZoom and UpdatePosition, as retail 0x0052F5C0 (UpdateZoom, landed) -> 0x0052F900 -> 0x0052FFD0;
//   (3) the body reads ColourTimeElapsed +0xA8 / ColourTime +0xAC and writes Colour +0x84 / InitialColour +0x8C / RenderColour +0x94.
//   Catalog label "GameAnimation_UpdatePosition" is wrong (LABELS.tsv).
// Callees: __ftol2 0x00BFEA70 (the eight GetRGBColour channel conversions), the virtual IsIndependent (vtable +0x194, slot 101).
//   Data: 1/255 0x01231724, -2.0f 0x01246050, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 0.5f 0x0122F59C, 255.0f 0x01230014 (paired by relocation).
// Inline helpers (FableWin.pdb, the TU's first emissions after UpdateColour): CRGBFloatColour ??H (+), ??G (-), ??D(float) (*float);
//   ?pow@@YAMMH@Z (math.h float/int overload, the _Pow_int loop, four times for ColourTime^2 and once for elapsed^2) sits just
//   before UpdateColour in FableWin. Clamp (FableWin ?Clamp@CRGBFloatColour@@QAEXXZ, no retail copy) is expanded in place:
//   CRGBFloatColour.h opt-in FABLE_RETAIL_CRGBFLOATCOLOUR_CLAMP (__forceinline: plain inline is called out of line here).
// Shape: acceleration and the result are assigned (temporary then copy), velocity is scaled per channel by 1/ColourTime (the
//   operator*=(float) spelling keeps the reciprocal one value longer on the x87 stack); the render product is a third named
//   colour assigned from `colour * parent` with the parent declared first (the frame slots and the parent-first conversion).
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_CLAMP
#include <math.h>
#include "engine/NUISystem_CComponent.h"
#include "engine/CRGBFloatColour.h"

namespace NUISystem {

void CComponent::UpdateColour(float deltaTime)
{
    if (ColourTimeElapsed < ColourTime) {
        ColourTimeElapsed += deltaTime;
        CRGBFloatColour initial(InitialColour);
        CRGBFloatColour target(TargetColour);
        CRGBFloatColour acceleration;
        CRGBFloatColour velocity;
        CRGBFloatColour colour;
        acceleration = (target - initial) * -2.0f;
        acceleration.R /= pow(ColourTime, 2);
        acceleration.G /= pow(ColourTime, 2);
        acceleration.B /= pow(ColourTime, 2);
        acceleration.A /= pow(ColourTime, 2);
        velocity = (target - initial) * 2.0f;
        velocity.R *= 1.0f / ColourTime;
        velocity.G *= 1.0f / ColourTime;
        velocity.B *= 1.0f / ColourTime;
        velocity.A *= 1.0f / ColourTime;
        colour = acceleration * 0.5f * pow(ColourTimeElapsed, 2) + velocity * ColourTimeElapsed + initial;
        colour.Clamp();
        Colour = colour.GetRGBColour();
        if (ColourTimeElapsed >= ColourTime) {
            Colour = TargetColour;
            InitialColour = Colour;
        }
    }
    if (!IsIndependent()) {
        CRGBFloatColour parent(ParentColour);
        CRGBFloatColour colour(Colour);
        CRGBFloatColour render_colour;
        render_colour = colour * parent;
        RenderColour = render_colour.GetRGBColour();
    } else
        RenderColour = Colour;
}

}
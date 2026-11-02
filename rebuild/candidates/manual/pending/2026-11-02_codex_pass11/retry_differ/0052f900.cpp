// NUISystem::CComponent::UpdateColour(float) -- advance the colour transition and compose parent colour.
// Leaf: ?UpdateColour@CComponent@NUISystem@@UAEXM@Z
// Grade: DIFFER(1323v1730) -- inline budget: retail expands the four CRGBFloatColour(const CRGBColour&) conversions and both GetRGBColour calls (movzx byte reads / fistp packs inline), VC7.1 RTM keeps them as calls in this TU; 10 shapes (pow as products, one-expression render colour, declaration order, if/else clamps, branch order, shared difference local, four alternative ctor/GetRGBColour bodies in an overlay) never inline them. Graded in Codex's header context
#include "engine/CRGBFloatColour.h"
#include "engine/NUISystem_CComponent.h"
#include <math.h>
namespace NUISystem {
void CComponent::UpdateColour(float tick)
{
    if (ColourTimeElapsed < ColourTime) {
        ColourTimeElapsed += tick;
        CRGBFloatColour initial_colour(InitialColour);
        CRGBFloatColour target_colour(TargetColour);
        CRGBFloatColour colour_velocity;
        CRGBFloatColour colour_acceleration;
        CRGBFloatColour colour;
        colour_acceleration=(target_colour-initial_colour)*-2.0f;
        colour_acceleration.R/=pow(ColourTime,2);
        colour_acceleration.G/=pow(ColourTime,2);
        colour_acceleration.B/=pow(ColourTime,2);
        colour_acceleration.A/=pow(ColourTime,2);
        colour_velocity=(target_colour-initial_colour)*2.0f;
        colour_velocity.R/=ColourTime;
        colour_velocity.G/=ColourTime;
        colour_velocity.B/=ColourTime;
        colour_velocity.A/=ColourTime;
        colour=colour_acceleration*0.5f*pow(ColourTimeElapsed,2)+colour_velocity*ColourTimeElapsed+initial_colour;
        if(colour.R>1.0f) colour.R=1.0f; if(colour.R<0.0f) colour.R=0.0f;
        if(colour.G>1.0f) colour.G=1.0f; if(colour.G<0.0f) colour.G=0.0f;
        if(colour.B>1.0f) colour.B=1.0f; if(colour.B<0.0f) colour.B=0.0f;
        if(colour.A>1.0f) colour.A=1.0f; if(colour.A<0.0f) colour.A=0.0f;
        Colour=colour.GetRGBColour();
        if(ColourTimeElapsed>=ColourTime) {
            Colour=TargetColour;
            InitialColour=TargetColour;
        }
    }
    if(!IsIndependent()) {
        CRGBFloatColour render_colour;
        CRGBFloatColour parent_colour(ParentColour);
        CRGBFloatColour colour(Colour);
        render_colour=colour*parent_colour;
        RenderColour=render_colour.GetRGBColour();
    } else {
        RenderColour=Colour;
    }
}
}

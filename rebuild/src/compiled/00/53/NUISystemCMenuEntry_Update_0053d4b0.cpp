// NUISystem::CMenuEntry::Update(float tick) -- update the menu entry visual state.
// Retail: 0x0053d4b0,139 bytes,thiscall.
// Leaf: ?Update@CMenuEntry@NUISystem@@UAEXM@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CMENUENTRY_UPDATE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CMenuEntry.h"
void NUISystem::CMenuEntry::Update(float tick)
{
    CRGBColour old_render_colour=RenderColour;
    if(LoadAndReturnDef()->PCStyle)
        CClickable::Update(tick);
    else
        CChangingStateComponent::Update(tick);
    if(RenderColour.A && !old_render_colour.A)
        ExecuteActions(PActionsOnVisible);
}
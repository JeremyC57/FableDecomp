// NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float interpolate) const -- find the focal point or use parent render position.
// Leaf: ?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z
// Retail: 004c1060,126 bytes,thiscall hidden result.
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
extern const CCharString MAIN_FOCAL_POSITION_NAME;
extern const CCharString TARGETING_CURSOR_POSITION_NAME;
#include "engine/NGraphicAppearance_CTCGraphicAppearance_HelperPoint.h"
namespace NGraphicAppearance {
C3DVector CTCGraphicAppearance::GetFocalPos(float interpolate)const {
 C3DVector pos;
 if(GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,pos,interpolate))return pos;
 if(GetHelperPoint(TARGETING_CURSOR_POSITION_NAME,0,0,pos,interpolate))return pos;
 pos=GetRenderPositionForThing(GetParentThing(),interpolate);
 return pos;
}
}
// CTCEmoteIcon::DrawEmoteTypeIcon -- maintain the emote sprite within drawing distance.
// Inline-budget: /d2inlT109 expands both original handle Clear calls;108 retains the first call.
// Retail:0071ad20,365 bytes,thiscall,ret4.
// Leaf: ?DrawEmoteTypeIcon@CTCEmoteIcon@@AAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /d2inlT109
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/GFWorldDistance.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitiveSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
void CTCEmoteIcon::DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo& info)
{
 if(GFWorld3DDistanceSmallerThan(PeekWorld().GetGameLODCentre(),GetParentThing().GetPos(),GetDefinitionManager().PeekGlobalDef().EmoteIconDrawDistance)) {
  C3DVector position_to_draw;
  if(GetParentThing().GetTCGraphicAppearance()->GetStaticFocalPosIfLoaded(position_to_draw)) {
   position_to_draw.Z+=0.5f;
   CEnginePrimitiveSprite prim(position_to_draw,EmoteIcon,0,0x90,0.0f,EmoteScale,EmoteScale,(EEnginePrimitiveBlendMode)3,EmoteAlpha,255,255,255,0.0f,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0);
   if(!PrimitiveHandle.IsNull())PrimitiveHandle.Clear();
   info.Engine.SetPersistentPrimitive(PrimitiveHandle,prim,0);
  }
 }else if(!PrimitiveHandle.IsNull())PrimitiveHandle.Clear();
}
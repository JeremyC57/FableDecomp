// CTCStealth::DrawPreparePrimitivesForRendering -- draw active perception state letters.
// Retail: 007471f0,308 bytes.
// Leaf: ?DrawPreparePrimitivesForRendering@CTCStealth@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
#define FABLE_RETAIL_STEALTH_DRAW
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CIENGINE
#include "engine/C3DVector.h"
#include "engine/CTCStealth.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CEnginePrimitiveText.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
void CTCStealth::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&info)
{
 if(GetThingManager().IsDrawingPerceptionDebug()) {
  C3DVector up(0.0f,0.0f,1.0f);
  CCharString debug_text;
  if(IsViewable())debug_text+="V ";
  if(IsHearable())debug_text+="H ";
  if(IsSmellable())debug_text+="S ";
  if(IsSneaking())debug_text+="Sn ";
  if(debug_text!="")
   info.Engine.AddPrimitive(CEnginePrimitiveText(0,GetParentThing().GetPos()+up*3.0f,1.0f,debug_text.ToWideString(),PeekComponent().PeekDebugFont(),CRGBColour(0xa6,0xa6,0xa6),false,(EEngineInterpolationSet)0));
 }
}

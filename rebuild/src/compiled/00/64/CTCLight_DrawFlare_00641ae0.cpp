// CTCLight::DrawFlare -- allocate a persistent flare slot and draw the light halo.
// Retail:00641ae0,331 bytes,thiscall,ret16.
// Leaf: ?DrawFlare@CTCLight@@IAEXJABVC3DVector@@AAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@4@@Z
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CLIGHTDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#include "engine/CThing.h"
#include "engine/CTCLight.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitiveFlareSprite.h"
void CTCLight::DrawFlare(long flare_no,const C3DVector& pos,NGraphicAppearance::CTCGraphicAppearance& tc_appearance,
 const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
 float fade_dist=tc_appearance.GetFadeDistance();
 if((unsigned int)flare_no>=FlareSprites.size()) {
  FlareSprites.resize(flare_no+1);
  FlareSprites[flare_no]=tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive((NGraphicAppearance::EPrimitiveType)3));
 }
 long halo_graphic=GetHaloGraphic();
 if(halo_graphic>0) {
  tc_appearance.SetPrimitiveGraphics(FlareSprites[flare_no],CEnginePrimitiveFlareSprite(GetParentThing().GetUniqueID(),pos,halo_graphic,0,
   0.0f,1.0f,2.5f,2.5f,0.33f,ENGINE_PRIMITIVE_BLEND_ADDSMOOTH,(unsigned long)GetParentThing().GetUniqueID(),(unsigned char)fade_dist,0.0f,ENGINE_INTERPOLATION_SET_0),0);
 }
}
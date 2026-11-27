// 007070d0 CTCHeroAttachableAppearanceModifiers::UpdateGraphics
// Leaf: ?UpdateGraphics@CTCHeroAttachableAppearanceModifiers@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_TCBASE
#define FABLE_RETAIL_CTCHEROMORPH
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGOBJECTDEF_SIZE_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "fable_assert.h"
#include "engine/EHeroMorphType.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CTextureReplacementDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"

// Inline-budget: the linked 162-byte CAppearanceModifierDef specialization was compiled for size with a frame pointer. Its shared template body is unchanged.
#if defined(_MSC_VER)
#pragma optimize("y", off)
#pragma optimize("s", on)
template<> __declspec(noinline) inline
#else
template<> inline
#endif
bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out)const
{
 const CSubDefInfo* info=GetPSubDefInfo(CAppearanceModifierDef::StatGetTypeName());
 if(info){
  CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
  out->SetDynamicCast(def);
  return true;
 }
 return false;
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif

void CTCHeroAttachableAppearanceModifiers::UpdateGraphics()
{
    RecreatePrimitives = true;
    Graphics.clear();
    CTCHeroMorph* ptc_morph = 0;
    if (!GetParentThing().GetTC(&ptc_morph)) {
        FABLE_RETAIL_ASSERT(false);
    }
    for (long appearance_type = 0; appearance_type < AppearanceModifiers.size(); ++appearance_type) {
        std::vector<CModifierEntry>& object_set = AppearanceModifiers[appearance_type];
        for (std::vector<CModifierEntry>::iterator it_modifier = object_set.begin();
             it_modifier != object_set.end(); ++it_modifier) {
            if (it_modifier->Rendered) {
                CDefPointer<const CThingObjectDef> pobject_def;
                GetDefinitionManager().GetDef(it_modifier->DefIndex, &pobject_def);
                CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                if (!pobject_def->GetSubDef(&pappearance_modifier_def)) {
                    FABLE_RETAIL_ASSERT(false);
                }
                CDefPointer<const CTextureReplacementDef> ptexture_replacement_def;
                pobject_def->GetSubDef(&ptexture_replacement_def);
                for (long morph_graphic_index = 0;
                     morph_graphic_index < pappearance_modifier_def->MorphGraphics.GetNoGraphics(); ++morph_graphic_index) {
                    const CAppearanceModifierGraphics::CEntry& graphic =
                        pappearance_modifier_def->MorphGraphics.GetGraphic(morph_graphic_index);
                    if (graphic.MorphType == HERO_MORPH_NONE) {
                        AddGraphic(CModifierGraphic(graphic.BankIndex, graphic.RenderSizeX, ptexture_replacement_def));
                    } else {
                        float morph_strength = ptc_morph->GetMorphStrength(graphic.MorphType);
                        if (morph_strength >= graphic.StartMorphStrength && morph_strength < graphic.EndMorphStrength)
                            AddGraphic(CModifierGraphic(graphic.BankIndex, graphic.RenderSizeX, ptexture_replacement_def));
                    }
                }
            }
        }
    }
}
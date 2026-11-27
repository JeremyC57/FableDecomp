// 00707440 CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw
// Flags: none
// Leaf: ?OnAppearanceDraw@CTCHeroAttachableAppearanceModifiers@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_TCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDTEXTUREMORPH_PASS26
#define FABLE_RETAIL_CCOMPOSITEBLENDDESC_ORIGINAL_CTOR_PASS26
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#include "engine/CThing.h"
#include "engine/CTextureReplacementDef.h"
#include "engine/NGraphicAppearance_CPrimitive.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/CEnginePrimitiveAnimatingMesh.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
bool CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& tc_appearance,const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
    float scale=tc_appearance.GetScale();
    float fade_dist=tc_appearance.GetFadeDistance();
    if(RecreatePrimitives){
        while(!PrimHandles.empty()){
            tc_appearance.RemovePrimitive(PrimHandles.back());
            PrimHandles.pop_back();
        }
        PrimHandles.reserve(Graphics.size());
    }
    long handle_no=0;
    for(std::vector<CModifierGraphic>::const_iterator it_graphic=Graphics.begin();it_graphic!=Graphics.end();++it_graphic){
        if(!RecreatePrimitives && (draw_info.ReasonsForDraw==2 || draw_info.ReasonsForDraw==1 || draw_info.ReasonsForDraw==3)) {
            tc_appearance.RefreshPrimitiveAnimationAndPos(PrimHandles[handle_no],*draw_info.Anim,*draw_info.Pos,*draw_info.Orientation);
            ++handle_no;
        }else{
            CEngineTextureMorphSet morph_set;
            if(it_graphic->PTextureReplacement.Get()){
                for(long c1=0;c1<it_graphic->PTextureReplacement->GetNoEntries();++c1){
                    const CTextureReplacementDef::CEntry& entry=it_graphic->PTextureReplacement->GetEntry(c1);
                    morph_set.AddTextureMorph(entry.OriginalTexture,CCompositeBlendDesc(COMPOSITE_BLEND_SOLID,entry.NewTexture,CRGBColour(0,0,0),CRGBColour(0,0,0,0)));
                }
            }
            if(const std::vector<CEngineTextureMorph>* vect=draw_info.PTextureMorphs){
                for(long c1=0;c1<vect->size();++c1)
                    for(long c2=0;c2<(*vect)[c1].Blends.size();++c2)
                        morph_set.AddTextureMorph((*vect)[c1].OriginalTextureIndex,(*vect)[c1].Blends[c2]);
            }
            if(RecreatePrimitives)
                PrimHandles.push_back(tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive(NGraphicAppearance::GAPT_ANIMATING_MESH,it_graphic->BankIndex)));
            std::set<CCharString> hide_materials;
            CEnginePrimitiveMorphedAnimatingMesh mesh_prim(ParentThing->GetUniqueID(),it_graphic->BankIndex,morph_set.PeekEngineMorphs(),hide_materials,*draw_info.Anim,*draw_info.Pos,*draw_info.Orientation,it_graphic->RenderSizeX*scale,true,tc_appearance.GetAlpha(),tc_appearance.GetTintColour(),fade_dist,(EEngineInterpolationSet)0,tc_appearance.PeekLightingChannel());
            mesh_prim.MeshEffect=tc_appearance.GetPMeshEffect();
            tc_appearance.SetPrimitiveGraphics(PrimHandles[handle_no],mesh_prim,0);
            ++handle_no;
        }

    }
    RecreatePrimitives=false;
    return true;
}
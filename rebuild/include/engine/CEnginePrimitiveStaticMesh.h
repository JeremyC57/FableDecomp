#ifndef FABLE_ENGINE_CENGINEPRIMITIVESTATICMESH_H
#define FABLE_ENGINE_CENGINEPRIMITIVESTATICMESH_H
#ifdef FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CRGBColour.h"
class CEnginePrimitiveStaticMesh:public CEnginePrimitive {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_EYEDRAW_PASS24
    // The native eye draw expands the base and the default mesh fields in place.
    CEnginePrimitiveStaticMesh() : CEnginePrimitive(static_cast<EEnginePrimitiveType>(8), static_cast<EEngineInterpolationSet>(0)) {}
#endif
    CEnginePrimitiveStaticMesh(unsigned __int64,unsigned long,const C3DVector&,const CRightHandedSet&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&,bool);
    unsigned long MeshIndex;
    C3DVector WorldPos;
    float Size,FadeStart;
    CRightHandedSet RightHandedSet;
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
    CRGBColour TintColour;
    unsigned char Alpha;
    unsigned char Lit:1;
    bool BulletTimeAffectsCloth;
};
FABLE_STATIC_ASSERT_AT(24, sizeof(CEnginePrimitiveStaticMesh)==76);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_INLINE_CTOR
// Retail opt-in (p25N, rows 0x0083B6B0 / 0x0083DC80 / 0x00837B30): the constructor is an inline header function in the original. FableWin.pdb
// compiles it as a COMDAT in a game TU (tc_graphic_appearance_static_mesh.obj #10, 0x0263C1E0, debug length 217), and retail's copy
// 0x0077BD70 sits with that TU (landed CTCGraphicAppearanceStaticMeshFadeOutOnProximity::OnAppearanceDraw 0x0077BEC0 defines it there).
// Callers in other TUs call it out of line but see the body: with it in view the three DrawPreparePrimitivesForRendering rows share the
// interpolated-position temp with its difference temp (frame 0x80); declaration-only gives 0x8C. Parameter names: Ego_d.pdb.
// Needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR (the base (type, uid, set) constructor).
inline CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 uid,unsigned long mesh_index,const C3DVector& world_pos,const CRightHandedSet& rhset,float size,bool lit,unsigned char alpha,CRGBColour tint_colour,float fade_start,EEngineInterpolationSet interp_set,const CEnginePrimitiveThemeChannelDesc& theme_channel_desc,bool bullet_time_affects_cloth)
    :CEnginePrimitive((EEnginePrimitiveType)8,uid,interp_set),MeshIndex(mesh_index),WorldPos(world_pos),Size(size),FadeStart(fade_start),RightHandedSet(rhset),ThemeChannelDesc(theme_channel_desc),TintColour(tint_colour),Alpha(alpha),Lit(lit),BulletTimeAffectsCloth(bullet_time_affects_cloth)
{
}
#endif
#endif
#endif
// Retail opt-in (Codex pass22): Original Ego_r size76 and complete fields; FableWin uses8-byte CIVCountedPointer and size80. Native0077BD70 ctor177.

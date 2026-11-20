#ifndef FABLE_ENGINE_CENGINEPRIMITIVE2DSPRITE_H
#define FABLE_ENGINE_CENGINEPRIMITIVE2DSPRITE_H
// New shared header (chunk 779, row 0x00542A30 NUISystem::CIconText::Draw): Ego_r.pdb CEnginePrimitive2DSprite, sizeof 0x54
// (ghidra_out/struct_layouts_egor.tsv; the retail frame of Draw homes one at [esp+0B4h] up to the saved registers at 0x108):
// CEnginePrimitive base +0 (Type 0x22, InterpolationSet, ThemeChannelDesc), ClipBox (C2DBoxF) +0xC, Layer +0x1C, BankIndex +0x20,
// Frame +0x24, StartAngle +0x28, EndAngle +0x2C, BlendMode +0x30, Colour (CRGBColour) +0x34, ClipBoxIsPos +0x38, Flags +0x3C,
// Texture +0x40, UVOffsetsStart +0x44, UVOffsetsEnd +0x4C. The 16-parameter constructor is the landed 0x0041BEB0
// (FableWin.pdb ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@JKKJJMMEEEEKABVC2DVector@@2@Z, `ret 44h`): its stores place the
// parameters as (unused u64, box, layer, bank index, frame, blend mode -> +0x30, interpolation set -> +0x4, start angle, end angle,
// a -> +0x37, r -> +0x36, g -> +0x35, b -> +0x34, flags -> +0x3C, uv start, uv end); ClipBoxIsPos and Texture are zeroed.
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
// Use the same original enum before the base-class declaration, also valid C++03.
#include "engine/EEngineInterpolationSet.h"
#endif
#include "engine/CEnginePrimitive.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
class CTexture;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#ifndef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER
// (hand3c3, row 0x0041AFA0: a TU that also includes engine/NUISystem_CManager.h takes EEngine2DLayer from there -- the same
// enumerator list -- and sets FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER so that it is not defined twice.)
// Original values recovered from both raw donor TPI sets; see ORIGINAL_RENDER_ENUMS.json.
enum EEngine2DLayer {
    ENGINE_2D_RANGE_SIZE=16,
    ENGINE_2D_RANGE_0_START=0,
    ENGINE_2D_RANGE_1_START=16,
    ENGINE_2D_RANGE_2_START=32,
    ENGINE_2D_RANGE_3_START=48,
    ENGINE_2D_RANGE_4_START=64,
    ENGINE_2D_LAYER_M3=64,
    ENGINE_2D_LAYER_M2=65,
    ENGINE_2D_LAYER_M1=66,
    ENGINE_2D_LAYER_0=67,
    ENGINE_2D_LAYER_1=68,
    ENGINE_2D_LAYER_2=69,
    ENGINE_2D_LAYER_3=70,
    ENGINE_2D_LAYER_4=71,
    ENGINE_2D_MESH_LAYER=72,
    ENGINE_2D_LAYER_5=73,
    ENGINE_2D_LAYER_6=74,
    ENGINE_2D_LAYER_7=75,
    ENGINE_2D_LAYER_8=76,
    ENGINE_2D_LAYER_9=77,
    ENGINE_2D_LAYER_10=78,
    ENGINE_2D_LAYER_11=79,
    ENGINE_2D_LAYER_12=80,
    ENGINE_2D_LAYER_13=81,
    ENGINE_2D_LAYER_SPELLS=82,
    ENGINE_NUM_2D_LAYERS=83,
    ENGINE_2D_NULL_LAYER=-1,
    ENGINE_2D_LAYER_FIRST=0
};
#endif
enum EEnginePrimitiveBlendMode {
    ENGINE_PRIMITIVE_BLEND_SOLID=0,
    ENGINE_PRIMITIVE_BLEND_BOOLEAN_ALPHA=1,
    ENGINE_PRIMITIVE_BLEND_ALPHA=2,
    ENGINE_PRIMITIVE_BLEND_ADDITIVE=3,
    ENGINE_PRIMITIVE_BLEND_ADDSMOOTH=4,
    ENGINE_PRIMITIVE_BLEND_CONST_COLOUR=5,
    ENGINE_PRIMITIVE_BLEND_COUNT=6
};
#ifndef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
enum EEngineInterpolationSet {
    ENGINE_INTERPOLATION_SET_0=0,
    ENGINE_INTERPOLATION_SET_1=1,
    ENGINE_INTERPOLATION_SET_2=2,
    ENGINE_INTERPOLATION_SET_NUM=3,
    ENGINE_INTERPOLATION_SET_ALWAYSUPDATE=0,
    ENGINE_INTERPOLATION_SET_PAUSED=1,
    ENGINE_INTERPOLATION_SET_BULLETTIME=2,
    ENGINE_INTERPOLATION_SET_DEFAULT=0
};
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_HIGHLIGHT_BODY
#ifdef _MSC_VER
typedef unsigned __int64 SpriteUniqueId;
#else
typedef unsigned long long SpriteUniqueId;
#endif
#endif
class CEnginePrimitive2DSprite : public CEnginePrimitive {
public:
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_POS_CTOR
    // Retail opt-in (wfA2, row 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine): Ego_r.pdb / FableWin.pdb
    // ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@AAVCTexture@@W4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z,
    // out of line in retail at 0x0043FA20 (landed; the caller pushes 15 argument words: u64 id, &pos, layer, &texture, blend, set,
    // two angles, four colour bytes, flags, &uv start, &uv end, 0x004399C4..0x00439A29). Needs ENUM_CTORS. Declaration only.
    CEnginePrimitive2DSprite(unsigned __int64,const C2DVector&,EEngine2DLayer,CTexture&,EEnginePrimitiveBlendMode,EEngineInterpolationSet,
        float,float,unsigned char,unsigned char,unsigned char,unsigned char,unsigned long,const C2DVector&,const C2DVector&);   // 0x0043FA20
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_BOX_CTOR
    // Retail opt-in (hand3c3, row 0x0041AFA0 NUISystem::CSprite::Draw): Ego_r.pdb / FableWin.pdb
    // ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@AAVCTexture@@W4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEKABVC2DVector@@6@Z,
    // out of line in retail at 0x0041BF60 (landed; `ret 40h`; the caller pushes 16 argument words, 0x0041B209..0x0041B26B). FableWin
    // emits it right after CSprite::Draw (0x01D2EBC0), so the body is an inline header function visible to that TU: it follows the
    // class. Needs ENUM_CTORS.
    CEnginePrimitive2DSprite(unsigned __int64,const C2DBoxF&,EEngine2DLayer,CTexture&,EEnginePrimitiveBlendMode,EEngineInterpolationSet,
        float,float,unsigned char,unsigned char,unsigned char,unsigned char,unsigned long,const C2DVector&,const C2DVector&);   // 0x0041BF60
#endif
    // Retail opt-in (Codex 2026-11-09): both PDBs retain enum parameter types;
    // native0041beb0 takes a box,00493ac2 takes a position. Same16 arguments.
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_HIGHLIGHT_BODY
// Retail opt-in (Codex pass22): original constructor composition from both PDBs and donor01A662C6.
CEnginePrimitive2DSprite(SpriteUniqueId uid,const C2DBoxF& clip_box,EEngine2DLayer layer,
 unsigned long bank_index,unsigned long frame,EEnginePrimitiveBlendMode blend_mode,EEngineInterpolationSet interp_set,
 float start_angle,float end_angle,unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,
 unsigned long sprite_2d_flags,const C2DVector& uv_offsets_start=C2DVector(0.0f,0.0f),const C2DVector& uv_offsets_end=C2DVector(0.0f,0.0f))
 :CEnginePrimitive((EEnginePrimitiveType)0x22,uid,interp_set),ClipBox(clip_box),Layer(layer),BankIndex(bank_index),Frame(frame),
 StartAngle(start_angle),EndAngle(end_angle),BlendMode(blend_mode),Colour(r,g,b,alpha),ClipBoxIsPos(false),Flags(sprite_2d_flags),
 Texture(0),UVOffsetsStart(uv_offsets_start),UVOffsetsEnd(uv_offsets_end)
{}
#else
    CEnginePrimitive2DSprite(unsigned __int64,const C2DBoxF&,EEngine2DLayer,unsigned long,unsigned long,
        EEnginePrimitiveBlendMode,EEngineInterpolationSet,float,float,unsigned char,unsigned char,unsigned char,unsigned char,
        unsigned long,const C2DVector&,const C2DVector&);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
    // Retail opt-in (Codex pass20): original FW01a663fc; all fields and input reads retained.
    CEnginePrimitive2DSprite(unsigned __int64 id,const C2DVector&pos,EEngine2DLayer layer,unsigned long bank,unsigned long frame,
        EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float start_angle,float end_angle,
        unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,unsigned long flags,const C2DVector&uv_start,const C2DVector&uv_end)
    :CEnginePrimitive((EEnginePrimitiveType)34,id,interpolation),ClipBox(pos,pos),Layer(layer),BankIndex(bank),Frame(frame),StartAngle(start_angle),EndAngle(end_angle),BlendMode(blend),Colour(r,g,b,alpha),ClipBoxIsPos(true),Flags(flags),Texture(0),UVOffsetsStart(uv_start),UVOffsetsEnd(uv_end){}
#else
    CEnginePrimitive2DSprite(unsigned __int64,const C2DVector&,EEngine2DLayer,unsigned long,unsigned long,
        EEnginePrimitiveBlendMode,EEngineInterpolationSet,float,float,unsigned char,unsigned char,unsigned char,unsigned char,
        unsigned long,const C2DVector&,const C2DVector&);
#endif
#else
    CEnginePrimitive2DSprite(unsigned __int64, const C2DBoxF&, long, unsigned long, unsigned long, long, long, float, float,
                             unsigned char, unsigned char, unsigned char, unsigned char, unsigned long,
                             const C2DVector&, const C2DVector&);                        // 0x0041BEB0
#endif
    C2DBoxF ClipBox;                                                                      // +0x0c
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
    // Both raw donor records retain the original enum member type at this offset.
    EEngine2DLayer Layer; // +0x1c
#else
    long Layer;                                                                           // +0x1c
#endif
    unsigned long BankIndex;                                                              // +0x20
    unsigned long Frame;                                                                  // +0x24
    float StartAngle;                                                                     // +0x28
    float EndAngle;                                                                       // +0x2c
#ifdef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
    // Both raw donor records retain the original enum member type at this offset.
    EEnginePrimitiveBlendMode BlendMode; // +0x30
#else
    long BlendMode;                                                                       // +0x30
#endif
    CRGBColour Colour;                                                                    // +0x34
    bool ClipBoxIsPos;                                                                    // +0x38
    unsigned long Flags;                                                                  // +0x3c
    CTexture* Texture;                                                                    // +0x40
    C2DVector UVOffsetsStart;                                                             // +0x44
    C2DVector UVOffsetsEnd;                                                               // +0x4c
};
FABLE_STATIC_ASSERT_AT(117, sizeof(CEnginePrimitive2DSprite) == 0x54);
#if defined(FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_POS_CTOR) && defined(FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_POS_CTOR_BODY)
// Retail opt-in (wfA2 successor 4, row 0x00439070): the inline body of the texture/position ctor declared above. Retail emits it as
// an out-of-line COMDAT in the CPlayerGui TU (0x0043FA20; FableWin places ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@...K11@Z at
// 0x01A7E8F0, beside DrawPreparePrimitivesForEngine 0x01A7D4F0), so the body was visible to the caller. This text compiled on its own
// grades MATCH against 0x0043FA20 (probe ctor.cpp, 167 bytes). Same member-init style as ..._CRIME_CTOR. Needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR,
// FABLE_RETAIL_C2DBOXF_CORNERS, FABLE_RETAIL_CRGBCOLOUR.
inline CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id,const C2DVector&pos,EEngine2DLayer layer,CTexture&texture,
        EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float start_angle,float end_angle,
        unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,unsigned long flags,const C2DVector&uv_start,const C2DVector&uv_end)
    :CEnginePrimitive((EEnginePrimitiveType)34,id,interpolation),ClipBox(pos,pos),Layer(layer),BankIndex(0),Frame(0),StartAngle(start_angle),
     EndAngle(end_angle),BlendMode(blend),Colour(r,g,b,alpha),ClipBoxIsPos(true),Flags(flags),Texture(&texture),UVOffsetsStart(uv_start),
     UVOffsetsEnd(uv_end){}
#endif
#if defined(FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS) && defined(FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_BOX_CTOR)
// The inline body of the texture / box ctor declared above (same member-init style as the other two bodies: ClipBox(box), bank and
// frame zero, ClipBoxIsPos false, Texture(&texture)). Compiled out of line from the CSprite::Draw row it grades RELOCATION_MATCH
// against 0x0041BF60 (167 bytes). Needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR, FABLE_RETAIL_C2DBOXF_COPY, FABLE_RETAIL_CRGBCOLOUR.
// Parameter names as Ego_d.pdb lists them for this overload (the 195-byte debug copy, one line-table entry: header line 1191).
inline CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 uid,const C2DBoxF&clip_box,EEngine2DLayer layer,CTexture&texture,
        EEnginePrimitiveBlendMode blend_mode,EEngineInterpolationSet interp_set,float start_angle,float end_angle,
        unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,unsigned long sprite_2d_flags,const C2DVector&uv_offsets_start,
        const C2DVector&uv_offsets_end)
    :CEnginePrimitive((EEnginePrimitiveType)34,uid,interp_set),ClipBox(clip_box),Layer(layer),BankIndex(0),Frame(0),StartAngle(start_angle),
     EndAngle(end_angle),BlendMode(blend_mode),Colour(r,g,b,alpha),ClipBoxIsPos(false),Flags(sprite_2d_flags),Texture(&texture),
     UVOffsetsStart(uv_offsets_start),UVOffsetsEnd(uv_offsets_end){}
#endif
#if defined(FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS) && defined(FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY)
// Retail opt-in (wfA2-5, row 0x0048E78E CSubtitleRenderer::DrawButtonGraphics): the inline body of the box ctor declared above
// (Ego_r.pdb / FableWin.pdb ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@
// W4EEngineInterpolationSet@@MMEEEEKABVC2DVector@@5@Z). Retail emits it as an out-of-line COMDAT (0x0041BEB0); FableWin places it at
// 0x01A662C6 in the CSubtitleRenderer TU (beside the position ctor 0x01A663FC, DrawButtonText 0x01A665C5), so the body was visible to
// the subtitle rows. This text compiled on its own grades MATCH against 0x0041BEB0 (wfA2 probe/s5/ctors3.cpp). Same member-init style
// as ..._CRIME_CTOR (ClipBox(box), ClipBoxIsPos false, Texture 0). Needs FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR, FABLE_RETAIL_C2DBOXF_COPY,
// FABLE_RETAIL_CRGBCOLOUR.
inline CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id,const C2DBoxF&box,EEngine2DLayer layer,unsigned long bank,
        unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float start_angle,float end_angle,
        unsigned char alpha,unsigned char r,unsigned char g,unsigned char b,unsigned long flags,const C2DVector&uv_start,const C2DVector&uv_end)
    :CEnginePrimitive((EEnginePrimitiveType)34,id,interpolation),ClipBox(box),Layer(layer),BankIndex(bank),Frame(frame),StartAngle(start_angle),
     EndAngle(end_angle),BlendMode(blend),Colour(r,g,b,alpha),ClipBoxIsPos(false),Flags(flags),Texture(0),UVOffsetsStart(uv_start),
     UVOffsetsEnd(uv_end){}
#endif
#endif

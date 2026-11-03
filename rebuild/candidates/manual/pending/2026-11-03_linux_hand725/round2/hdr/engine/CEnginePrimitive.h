#ifndef FABLE_ENGINE_CENGINEPRIMITIVE_H
#define FABLE_ENGINE_CENGINEPRIMITIVE_H
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
// New header (chunk 724b, from the Codex pass-12 handoff model): CEnginePrimitive (Ego_r sizeof 0xC: Type +0, InterpolationSet +4,
// ThemeChannelDesc +8) and CEnginePrimitiveLine (Ego_r sizeof 0x3C: Start +0xC, End +0x18, Colour +0x24, Width +0x28, BankIndex
// +0x2C, BlendMode +0x30, BlendOp +0x34, UseZBuffer +0x38). Enumerators: Ego_r TPI. Constructors: the FableWin.pdb spellings
// ??0CEnginePrimitive@@QAE@W4EEnginePrimitiveType@@_KW4EEngineInterpolationSet@@@Z and the ten-argument
// ??0CEnginePrimitiveLine@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MW4EEnginePrimitiveBlendMode@@K_NW4EEngineInterpolationSet@@
// W4EEnginePrimitiveBlendOp@@@Z (inline: NCameraModes::DrawSquare 0x00875FB0 builds the line in place, Type 6, the three theme
// bytes zeroed). All class-keyed (pdb_class_keys V).
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0=0, ENGINE_INTERPOLATION_SET_1=1, ENGINE_INTERPOLATION_SET_2=2, ENGINE_INTERPOLATION_SET_NUM=3, ENGINE_INTERPOLATION_SET_ALWAYSUPDATE=0, ENGINE_INTERPOLATION_SET_PAUSED=1, ENGINE_INTERPOLATION_SET_BULLETTIME=2, ENGINE_INTERPOLATION_SET_DEFAULT=0 };
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_SOLID=0, ENGINE_PRIMITIVE_BLEND_BOOLEAN_ALPHA=1, ENGINE_PRIMITIVE_BLEND_ALPHA=2, ENGINE_PRIMITIVE_BLEND_ADDITIVE=3, ENGINE_PRIMITIVE_BLEND_ADDSMOOTH=4, ENGINE_PRIMITIVE_BLEND_CONST_COLOUR=5, ENGINE_PRIMITIVE_BLEND_COUNT=6 };
enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_ADD=0, ENGINE_PRIMITIVE_BLEND_OP_SUB=1, ENGINE_PRIMITIVE_BLEND_OP_REVSUB=2, ENGINE_PRIMITIVE_BLEND_OP_ADDSIGNED=3, ENGINE_PRIMITIVE_BLEND_OP_REVSUBSIGNED=4, ENGINE_PRIMITIVE_BLEND_OP_COUNT=5 };
enum EEnginePrimitiveType { ENGINE_PRIMITIVE_NULL=0, ENGINE_PRIMITIVE_GROUP=1, ENGINE_PRIMITIVE_CHANGER=2, ENGINE_PRIMITIVE_FILLED_RECT=3, ENGINE_PRIMITIVE_OUTLINE_RECT=4, ENGINE_PRIMITIVE_TEXT=5, ENGINE_PRIMITIVE_LINE=6, ENGINE_PRIMITIVE_SPRITE=7, ENGINE_PRIMITIVE_STATIC_MESH=8, ENGINE_PRIMITIVE_ANIMATING_MESH=9, ENGINE_PRIMITIVE_MORPHED_STATIC_MESH=10, ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH=11, ENGINE_PRIMITIVE_STATIC_MESH_FROM_FILE=12, ENGINE_PRIMITIVE_ANIMATING_MESH_FROM_FILE=13, ENGINE_PRIMITIVE_TARGET=14, ENGINE_PRIMITIVE_LIGHT=15, ENGINE_PRIMITIVE_SHADOWED_SPOT_LIGHT=16, ENGINE_PRIMITIVE_FLARE_SPRITE=17, ENGINE_PRIMITIVE_GAME_MESH=18, ENGINE_PRIMITIVE_WEAPON_TRAIL=19, ENGINE_PRIMITIVE_SPRITE_TRAIL=20, ENGINE_PRIMITIVE_PARTICLE_LINE=21, ENGINE_PRIMITIVE_PARTICLE_SPRITE=22, ENGINE_PRIMITIVE_PARTICLE_MESH=23, ENGINE_PRIMITIVE_PARTICLE_GROUP=24, ENGINE_PRIMITIVE_PARTICLE_DECAL=25, ENGINE_PRIMITIVE_PARTICLE_LINE_GROUP=26, ENGINE_PRIMITIVE_PARTICLE_SPRITE_GROUP=27, ENGINE_PRIMITIVE_PARTICLE_MESH_GROUP=28, ENGINE_PRIMITIVE_PARTICLE_SPRITE_TRAIL_GROUP=29, ENGINE_PRIMITIVE_PARTICLE_DECAL_GROUP=30, ENGINE_PRIMITIVE_WEATHER_MASK=31, ENGINE_PRIMITIVE_2D_PARTICLE_GROUP=32, ENGINE_PRIMITIVE_2D_VIEWPORT=33, ENGINE_PRIMITIVE_2D_SPRITE=34, ENGINE_PRIMITIVE_2D_CLOCK_SPRITE=35, ENGINE_PRIMITIVE_2D_STATIC_MESH=36, ENGINE_PRIMITIVE_2D_ANIMATING_MESH=37, ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH=38, ENGINE_PRIMITIVE_2D_TEXT=39, ENGINE_PRIMITIVE_DECAL=40, ENGINE_PRIMITIVE_MAX=41 };
enum EEnginePrimitiveStatIndex { ENGINE_PRIM_STAT_VERTICES=0, ENGINE_PRIM_STAT_TRIANGLES=1, ENGINE_PRIM_STAT_TEXTURES=2 };
class CEnginePrimitiveThemeChannelDesc {
public:
    unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;
    CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){}
};
class CEnginePrimitive {
protected:
    EEnginePrimitiveType Type;
    EEngineInterpolationSet InterpolationSet;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
public:
    CEnginePrimitive(EEnginePrimitiveType type,unsigned __int64 id,EEngineInterpolationSet interpolation_set):Type(type),InterpolationSet(interpolation_set){}
    EEnginePrimitiveType GetType()const{return Type;}
};
class CEnginePrimitiveLine : public CEnginePrimitive {
public:
    C3DVector Start,End;
    CRGBColour Colour;
    float Width;
    unsigned long BankIndex;
    EEnginePrimitiveBlendMode BlendMode;
    EEnginePrimitiveBlendOp BlendOp;
    bool UseZBuffer;
    CEnginePrimitiveLine(unsigned __int64 id,const C3DVector& start,const C3DVector& end,const CRGBColour& colour,float width,EEnginePrimitiveBlendMode blend_mode,unsigned long bank_index,bool use_zbuffer,EEngineInterpolationSet interpolation_set,EEnginePrimitiveBlendOp blend_op)
    :CEnginePrimitive((EEnginePrimitiveType)6,id,interpolation_set),Start(start),End(end),Colour(colour),Width(width),BankIndex(bank_index),BlendMode(blend_mode),BlendOp(blend_op),UseZBuffer(use_zbuffer){}
};
#endif

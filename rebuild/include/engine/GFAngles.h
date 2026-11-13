#ifndef FABLE_ENGINE_GFANGLES_H
#define FABLE_ENGINE_GFANGLES_H
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(fmod)
class C3DVector;
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
// Original FableWin01ACF300: float atan result, division by promoted float two-pi.
inline float FABLE_FASTCALL GFArcTan(double opposite,double adjacent){return (float)atan2(opposite,adjacent)/6.2831855f;}
#endif
bool FABLE_FASTCALL GFIsSignedAngleWithinRange(float,float);
#ifndef FABLE_RETAIL_GFANGLES_ANGLEXYTO_BODY
float FABLE_FASTCALL GFGetAngleXYTo(const C3DVector&,const C3DVector&);
#endif
float FABLE_FASTCALL GFGetSignedAngleDifference(float,float);
// Both original PDBs: these are file-local floating tunables used by CTCLook.
extern float BODY_ANGULAR_ACCELERATION_XY;
extern float BODY_ANGULAR_VELOCITY_MAX_XY;
// Original FableWin 01a824b0 and 01acf220 bodies, debug assertions omitted.
inline long FABLE_FASTCALL GFGetSign(float value) { return value<0.0f ? -1 : 1; }
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
FABLE_FORCEINLINE
#else
inline
#endif
float FABLE_FASTCALL GET_ROUNDED_ANGLE(float angle) {
 if(angle<0.0f || angle>=1.0f) {
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
  angle=fmod(angle,1.0f);
#else
  angle=(float)fmod((double)angle,1.0);
#endif
  if(angle<0.0f) angle+=1.0f;
 }
 return angle;
}
#ifdef FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
// Retail opt-in (chunk 2041, row 0x006AB820 CThingPlayerCreature::ResolveFacingDirection): the FableWin.pdb COMDATs listed right after
// ?ResolveFacingDirection@CThingPlayerCreature@@IAEXXZ (0x01DF5810): ?GFGetRoundedSignedAngle@@YIMM@Z 0x01DF7300, ?GFArcCos@@YIMN@Z
// 0x01DF7340, ??$GFWrapReturn@M@@YIMABM00@Z 0x01DF7380, ??$GFMod@M@@YIMABM0@Z 0x01DF7450 (GFWrapReturn<float> also in Ego_r.pdb).
// Retail keeps unreferenced out-of-line copies of two of them: GFGetRoundedSignedAngle 0x006ADDB0 (the row's TU; GFWrapReturn and
// GFMod expanded in it: `x < -0.5f` -> fmod(x - 0.5f, 1.0) + 0.5f, `x >= 0.5f` -> fmod(x + 0.5f, 1.0) - 0.5f) and GFArcCos 0x005DB180
// (`fld qword [esp+4] ; call _CIacos ; fmul 0.15915494f ; ret 8`: arc cosine in turns). Bodies checked against both copies.
template<class T> inline T FABLE_FASTCALL GFMod(const T& value, const T& divisor) { return (T)fmod(value, divisor); }
template<class T> inline T FABLE_FASTCALL GFWrapReturn(const T& value, const T& lower, const T& upper)
{
    if (value < lower) return GFMod(value - upper, upper - lower) + upper;
    if (value >= upper) return GFMod(value - lower, upper - lower) + lower;
    return value;
}
FABLE_FORCEINLINE float FABLE_FASTCALL GFGetRoundedSignedAngle(float angle) { return GFWrapReturn(angle, -0.5f, 0.5f); }
#pragma intrinsic(acos)
inline float FABLE_FASTCALL GFArcCos(double value) { return (float)acos(value) * 0.15915494f; }
#endif
#ifdef FABLE_RETAIL_GFANGLES_ANGLEXYTO_BODY
// Retail opt-in (chunk 2041, row 0x006AB820 CThingPlayerCreature::ResolveFacingDirection): Ego_r.pdb / FableWin.pdb
// ?GFGetAngleXYTo@@YIMABVC3DVector@@0@Z as a plain inline header function (needs engine/C3DVector.h first). Retail calls one COMDAT copy,
// 0x00661540 (91 bytes, landed; 9 call sites in four TUs), so every TU sees the body without expanding it (cost): the row's ternary
// target position (0x006ABB1A..0x006ABB2C) does not escape and shares its frame word with the later direction (Y / Z at [esp+48h] /
// [esp+4Ch], 0x006ABB7D / 0x006ABB91). Body = the landed row's (atan2 of the XY difference in turns, wrapped into [0, 1)); compiled out
// of line it grades against 0x00661540 (QLEN check, NOTES.md); the wrap is written out as the landed row's forced WrapAngle
// expands (a GET_ROUNDED_ANGLE call stays out of line under VC7.1's budget).
inline float FABLE_FASTCALL GFGetAngleXYTo(const C3DVector& from, const C3DVector& to)
{
    float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
#endif
#endif

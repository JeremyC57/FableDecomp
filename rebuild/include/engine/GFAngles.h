#ifndef FABLE_ENGINE_GFANGLES_H
#define FABLE_ENGINE_GFANGLES_H
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(fmod)
class C3DVector;
#ifdef FABLE_RETAIL_GFARCTAN_CUTSCENE_PASS30
// FableWin's public overload and the two calls in dene_scripting_tools identify this helper.
// Both retail SlideTeleport arms multiply the x87 result by one binary32 reciprocal.
// Inline-budget: both arms expand this helper. Plain inline and higher caller thresholds left calls.
FABLE_FORCEINLINE float FABLE_FASTCALL GFArcTan(double value)
{
#if defined(_MSC_VER) && _MSC_VER == 1310 && defined(_M_IX86) && !defined(FABLE_PORTABLE)
    // VC7.1 keeps this cast on the x87 stack and multiplies from the original float literal.
    return (float)atan(value) * 0.15915494f;
#else
    // Preserve x87 precision through the multiply on targets which round an intermediate float.
    return (float)(atan(value) * 0.15915494f);
#endif
}
#endif
#ifdef FABLE_RETAIL_SHOVE_FRAME_PASS23
// FableWin 01ACF300, donor GetAngleXY 0273AB10; retail 007D0186..194.
inline float FABLE_FASTCALL GFArcTan(double opposite,double adjacent) { return (float)atan2(opposite,adjacent)/6.2831855f; }
#endif
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
// Original FableWin01ACF300: float atan result, division by promoted float two-pi.
inline float FABLE_FASTCALL GFArcTan(double opposite,double adjacent){return (float)atan2(opposite,adjacent)/6.2831855f;}
#endif
#ifdef FABLE_RETAIL_CTCLOOK_SET_HEAD_LOOKING_PASS30
inline bool FABLE_FASTCALL GFIsSignedAngleWithinRange(float angle, float range) { return fabs(angle) * 2.0f <= range; }
#else
bool FABLE_FASTCALL GFIsSignedAngleWithinRange(float,float);
#endif
#ifndef FABLE_RETAIL_GFANGLES_ANGLEXYTO_BODY
float FABLE_FASTCALL GFGetAngleXYTo(const C3DVector&,const C3DVector&);
#endif
float FABLE_FASTCALL GFGetSignedAngleDifference(float,float);
// Both original PDBs: these are file-local floating tunables used by CTCLook.
extern float BODY_ANGULAR_ACCELERATION_XY;
extern float BODY_ANGULAR_VELOCITY_MAX_XY;
// Original FableWin 01a824b0 and 01acf220 bodies, debug assertions omitted.
#ifdef FABLE_RETAIL_NAV_ROUTE_SIGN_PASS30
long FABLE_FASTCALL GFGetSign(float value);
#else
inline long FABLE_FASTCALL GFGetSign(float value) { return value<0.0f ? -1 : 1; }
#endif
#if defined(FABLE_RETAIL_SHOVE_FRAME_PASS23) || defined(FABLE_RETAIL_JACK_STAGE1_UPDATE) || defined(FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE)
// (FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE, wfA2 row 0x0068CB90: same body, expanded at all eight sites of that row,
// fmod 0x00BFEB28 called inline at 0x0068CE4C..; inline-budget device, default text unchanged.)
FABLE_FORCEINLINE
#else
inline
#endif
float FABLE_FASTCALL GET_ROUNDED_ANGLE(float angle) {
 if(angle<0.0f || angle>=1.0f) {
#if defined(FABLE_RETAIL_SHOVE_FRAME_PASS23) || defined(FABLE_RETAIL_JACK_STAGE1_UPDATE)
  angle=fmod(angle,1.0f);
#else
  angle=(float)fmod((double)angle,1.0);
#endif
  if(angle<0.0f) angle+=1.0f;
 }
 return angle;
}
#ifdef FABLE_RETAIL_GFANGLES_UNSIGNED_DIFFERENCE
// Retail opt-in (landing gm, row 0x00702E00 CThingObject::GetAlpha): FableWin.pdb ?GFGetUnsignedAngleDifference@@YIMMM@Z (0x01D86050, the
// COMDAT right after ?GetAlpha@CThingObject@@ 0x01D85F30; no Ego_r.pdb or Fable.exe copy, expanded in retail). Body INFERRED from the
// expansion at 0x00702E00: both angles wrapped into [0, 1) again, the absolute difference, folded above half a turn. Forced inline
// (inline-budget device, as GET_ROUNDED_ANGLE_FORCEINLINE): plain `inline` stays an out-of-line call under VC7.1's budget there.
FABLE_FORCEINLINE float FABLE_FASTCALL GFGetUnsignedAngleDifference(float angle1, float angle2)
{
    float difference = (float)fabs(GET_ROUNDED_ANGLE(angle1) - GET_ROUNDED_ANGLE(angle2));
    if (difference > 0.5f)
        difference = 1.0f - difference;
    return difference;
}
#endif
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
#ifdef FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST
// Retail opt-in (row 0x007602F0 NShape::IsShapeConvex, tc_geometric_shapes.obj): the same body with the narrowed arc cosine in its own
// parentheses. VC7.1 keeps a parenthesised operand as a unit: one no-code float copy (IL 0x269) between the narrowing and the fmul by
// 1/(2 pi) in each expansion. Retail's list scheduler needs that node at all three sites of the row: the loop's fmul after the three
// last_line copy loads (0x007604A1), and two epilogue pops before `fadd [esp+10h]` (0x00760726). The out-of-line copy compiled from
// this body is byte-equal to retail 0x005DB180, as the default body's is; /Od sizes are equal, so Ego_d (GFArcCos line 151, 35 B)
// cannot tell the two apart. Inferred (ADVISOR_DECISIONS_B 2026-11-21 0x007602F0, the PAREN family: the parentheses are
// not evidenced by Ego_d, only by retail's schedule); every landed GFArcCos user regrades equal with it. The default body stays for the
// other includers.
inline float FABLE_FASTCALL GFArcCos(double value) { return ((float)acos(value)) * 0.15915494f; }
#else
inline float FABLE_FASTCALL GFArcCos(double value) { return (float)acos(value) * 0.15915494f; }
#endif
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

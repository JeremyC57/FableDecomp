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
float FABLE_FASTCALL GFGetAngleXYTo(const C3DVector&,const C3DVector&);
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
#endif

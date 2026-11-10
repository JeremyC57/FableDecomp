#ifndef FABLE_ENGINE_GFANGLES_H
#define FABLE_ENGINE_GFANGLES_H
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(fmod)
class C3DVector;
bool FABLE_FASTCALL GFIsSignedAngleWithinRange(float,float);
float FABLE_FASTCALL GFGetAngleXYTo(const C3DVector&,const C3DVector&);
float FABLE_FASTCALL GFGetSignedAngleDifference(float,float);
// Both original PDBs: these are file-local floating tunables used by CTCLook.
extern float BODY_ANGULAR_ACCELERATION_XY;
extern float BODY_ANGULAR_VELOCITY_MAX_XY;
// Original FableWin 01a824b0 and 01acf220 bodies, debug assertions omitted.
inline long FABLE_FASTCALL GFGetSign(float value) { return value<0.0f ? -1 : 1; }
inline float FABLE_FASTCALL GET_ROUNDED_ANGLE(float angle) {
 if(angle<0.0f || angle>=1.0f) {
  angle=(float)fmod((double)angle,1.0);
  if(angle<0.0f) angle+=1.0f;
 }
 return angle;
}
#endif

// 008fcc60 NCameraModes::CPhysicsCoordRigid::FrameUpdate -- smooth the target and update linked position/velocity.
// Leaf: ?FrameUpdate@CPhysicsCoordRigid@NCameraModes@@UAEXXZ
// Flags: none
// Retail:008fcc60,263 bytes.
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMTOOLS_TYPED
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#include "engine/NCameraModes_CPhysicsCoordRigid.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(tanh)
#endif
inline double FABLE_FASTCALL GFTanh(double n){return tanh(n);}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<class T>T NCameraModes::CTools::GetRollingAverage(const T& current_val,const T& new_val,float weight)const
{return (current_val*weight+new_val)/(weight+1.0);}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void NCameraModes::CPhysicsCoordRigid::FrameUpdate()
{
 C3DVector velocity;
 C3DVector wanted_pos=GetAcceleration();
 if(!LinkedState.CullPhysics){
  float arg=(wanted_pos-GetPosition()).GetSquaredMagnitude();
  float arg_scale=0.075f;
  float weight=GFSqr((float)(GFTanh(arg_scale*arg)))*5.0f;
  wanted_pos=GetCameraManager().GetCameraTools().GetRollingAverage(GetPosition(),wanted_pos,weight);
 }
 velocity=wanted_pos-GetPosition();
 LinkedState.CullPhysics=false;
 SetPosition(wanted_pos);
 SetVelocity(velocity);
}
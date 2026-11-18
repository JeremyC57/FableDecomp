// NCameraModes::CProjectileWeapon::GetHeightParamForPostionWithPhi(C3DVector const &,float) const -- choose the camera height from the two projected solutions.
// Grade: DIFFER(1249v1251) -- The later two-vector cross products now agree. At 0x008792C1 the retail body keeps the quadratic factor live through a second x87 pop.
// Retail: 0x00879070, 1251 bytes.
// Leaf: ?GetHeightParamForPostionWithPhi@CProjectileWeapon@NCameraModes@@ABEMABVC3DVector@@M@Z
// Agent724 (2026-11-03): regrade of this Codex pass-11 draft did not finish -- cl.exe ran at 100% CPU past the 600 s q.py timeout (twice for 0x00879070, once killed at 9.5 min for 0x00875fb0) on the current tree; loop prevention stopped further tries. Codex's Grade above is the last known residue.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_ALL_GROUP_PASS23
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_NCAMERAMODES_CPROJECTILEWEAPON_HEIGHT
#include "engine/NCameraModes_CProjectileWeapon.h"
#include "engine/CCameraModeDef.h"
#include "engine/CThingGameObject.h"
extern "C" {double __cdecl sin(double);double __cdecl cos(double);double __cdecl sqrt(double);double __cdecl fabs(double);}
#pragma intrinsic(sin,cos,sqrt,fabs)
extern const double TWO_PI;
extern const float DELTA_NOTIONAL_ZERO;
namespace {
float __fastcall GetHeightParamFromHeightOffset(float height_offset){return (height_offset-0.5f)*0.5f;}
}
float NCameraModes::CProjectileWeapon::GetHeightParamForPostionWithPhi(const C3DVector& pos,float target_phi)const {
 float target_height_param=HeightParam;
 C3DVector target_pos(pos);
 C3DVector base_pos(GetObservedThing()->GetPos());
 const C3DVector& observed=GetObservedThing()->GetPos();
 C3DVector focus_pos(observed.X,observed.Y,observed.Z+1.6f);
 const C3DVector& xAxis=XAxis;
 const C3DVector& yAxis=YAxis;
 const C3DVector& zAxis=ZAxis;
 C3DVector t_prime=target_pos-base_pos;
 float tx=t_prime.Z*xAxis.Z;tx+=t_prime.Y*xAxis.Y;tx+=t_prime.X*xAxis.X;
 float ty=t_prime.Z*yAxis.Z;ty+=t_prime.Y*yAxis.Y;ty+=t_prime.X*yAxis.X;
 float tz=t_prime.Z*zAxis.Z;tz+=t_prime.Y*zAxis.Y;tz+=t_prime.X*zAxis.X;
 C3DVector t_prime_again(tx,ty,tz);
 float ki=-(GetCameraModeDef()->ProjectileCameraInitialPhiOffsetForCamera*(1.0f/360.0f)-0.25f);
 C3DVector f_prime=focus_pos-base_pos;
 float x=f_prime.Z*xAxis.Z; x+=f_prime.Y*xAxis.Y; x+=f_prime.X*xAxis.X;
 float y=f_prime.Z*yAxis.Z; y+=f_prime.Y*yAxis.Y; y+=f_prime.X*yAxis.X;
 float z=f_prime.Z*zAxis.Z; z+=f_prime.Y*zAxis.Y; z+=f_prime.X*zAxis.X;
 C3DVector f_prime_again(x,y,z);
 C3DVector cam_pos_XY((float)sin(target_phi*TWO_PI),(float)cos(target_phi*TWO_PI),0.0f);
 C3DVector crazy_vector_bloke=f_prime_again-cam_pos_XY;
 float c_beta=f_prime_again.Z*f_prime_again.Z;
 float c_alpha=crazy_vector_bloke.GetSquaredMagnitude()-c_beta;
 float rad,factor_x;
 if((float)fabs(t_prime_again.X)>(float)fabs(t_prime_again.Y)){
  rad=t_prime_again.X-sin(target_phi*TWO_PI);rad*=rad;
  factor_x=cos((target_phi+ki)*TWO_PI);factor_x*=factor_x;
 }else{
  rad=t_prime_again.Y-cos(target_phi*TWO_PI);rad*=rad;
  factor_x=sin((target_phi+ki)*TWO_PI);factor_x*=factor_x;
 }
 if((float)fabs(factor_x)>DELTA_NOTIONAL_ZERO){
  float c_gamma=(float)(rad/(c_alpha*factor_x));
  float ca=(float)(c_gamma-1.0f);
  float cb=(float)(2.0f*(t_prime_again.Z-c_gamma*f_prime_again.Z));
  float cc=c_beta*c_gamma-t_prime_again.Z*t_prime_again.Z;
  float descriminator=cb*cb-cc*ca*4.0f;
  if(descriminator>=0.0f && (float)fabs(ca)>DELTA_NOTIONAL_ZERO){
   float root=(float)sqrt(descriminator);
   float height_1=(root-cb)/(ca+ca);
   float height_2=(-cb-root)/(ca+ca);
   float height_param_1=GetHeightParamFromHeightOffset(height_1);
   if(height_param_1<0.0f)height_param_1=0.0f;else if(height_param_1>1.0f)height_param_1=1.0f;
   float height_param_2=GetHeightParamFromHeightOffset(height_2);
   if(height_param_2<0.0f)height_param_2=0.0f;else if(height_param_2>1.0f)height_param_2=1.0f;
   C3DVector view_line_1=GetLookDirectionFromParams(Phi,height_param_1);
   C3DVector view_line_2=GetLookDirectionFromParams(Phi,height_param_2);
   view_line_1.AccurateNormalise();view_line_2.AccurateNormalise();
   C3DVector pos_1=GetPosFromParams(Phi,height_param_1);
   C3DVector pos_2=GetPosFromParams(Phi,height_param_2);
   C3DVector target_line_1=target_pos-pos_1;
   C3DVector target_line_2=target_pos-pos_2;
   target_line_1.AccurateNormalise();target_line_2.AccurateNormalise();
   C3DVector vec_compare_1=target_line_1.Cross(view_line_1);
   C3DVector vec_compare_2=target_line_2.Cross(view_line_2);
   float magnitude1=vec_compare_1.X*vec_compare_1.X;
   magnitude1+=vec_compare_1.Z*vec_compare_1.Z;
   magnitude1+=vec_compare_1.Y*vec_compare_1.Y;
   float magnitude2=vec_compare_2.X*vec_compare_2.X;
   magnitude2+=vec_compare_2.Z*vec_compare_2.Z;
   magnitude2+=vec_compare_2.Y*vec_compare_2.Y;
   if(magnitude1<magnitude2)target_height_param=height_param_1;
   else target_height_param=height_param_2;
  }
 }
 return target_height_param;
}

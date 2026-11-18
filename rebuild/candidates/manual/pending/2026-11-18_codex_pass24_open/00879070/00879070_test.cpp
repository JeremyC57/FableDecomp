#include <math.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif

const double TWO_PI = 6.2831853071795864769;
const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X-other.X,Y-other.Y,Z-other.Z); }
    float GetSquaredMagnitude() const { return X*X + Y*Y + Z*Z; }
    void AccurateNormalise()
    {
        float length = (float)sqrt(GetSquaredMagnitude());
        X /= length; Y /= length; Z /= length;
    }
    C3DVector Cross(const C3DVector& v) const { return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X); }
};
struct CThing {
    C3DVector Pos;
    const C3DVector& GetPos() const { return Pos; }
};
struct CCameraModeDef {
    float ProjectileCameraInitialPhiOffsetForCamera;
    CCameraModeDef() : ProjectileCameraInitialPhiOffsetForCamera(0) {}
};
namespace NCameraModes {
struct CProjectileWeapon {
    C3DVector XAxis, YAxis, ZAxis, ScenarioTarget;
    CThing Observed;
    CCameraModeDef Def;
    float HeightParam, Phi;
    mutable int Positions;
    bool PreferTall;
    CProjectileWeapon()
        : XAxis(1,0,0),YAxis(0,1,0),ZAxis(0,0,1),HeightParam(0.375f),Phi(0),Positions(0),PreferTall(true) {}
    const CThing* GetObservedThing() const { return &Observed; }
    const CCameraModeDef* GetCameraModeDef() const { return &Def; }
    C3DVector GetLookDirectionFromParams(float, float) const { return C3DVector(1,0,0); }
    C3DVector GetPosFromParams(float, float height) const
    {
        ++Positions;
        return ScenarioTarget - (((height > 0.5f) == PreferTall) ? C3DVector(1,0,0) : C3DVector(0,1,0));
    }
    float GetHeightParamForPostionWithPhi(const C3DVector&,float) const;
};
}

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

int main()
{
    NCameraModes::CProjectileWeapon camera;
    if (camera.GetHeightParamForPostionWithPhi(C3DVector(2,0,0),0) != 0.375f || camera.Positions)
        return 1;
    if (camera.GetHeightParamForPostionWithPhi(C3DVector(0,2,0),0) != 0.375f || camera.Positions)
        return 2;
    camera.ScenarioTarget = C3DVector(0,1,1.5f);
    if (fabs(camera.GetHeightParamForPostionWithPhi(camera.ScenarioTarget,0) - 0.5f) > 0.001f || camera.Positions != 2)
        return 3;
    camera.ScenarioTarget = C3DVector(0,2.5f,0);
    if (camera.GetHeightParamForPostionWithPhi(camera.ScenarioTarget,0) != 1.0f || camera.Positions != 4)
        return 4;
    camera.PreferTall = false;
    if (fabs(camera.GetHeightParamForPostionWithPhi(camera.ScenarioTarget,0) - 0.23f) > 0.001f || camera.Positions != 6)
        return 5;
    puts("00879070_TEST PASS");
    return 0;
}

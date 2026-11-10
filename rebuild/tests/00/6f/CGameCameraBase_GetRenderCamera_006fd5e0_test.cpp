#include <stdio.h>
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z){}};
static int pointCalls;
class CCamera{public:C3DVector WorldPosition,Target;long Flags;float FOV;CCamera():Flags(0),FOV(0){}CCamera(const CCamera& c):WorldPosition(c.WorldPosition),Target(c.Target),Flags(c.Flags),FOV(c.FOV){}const C3DVector& PeekWorldPos()const{return WorldPosition;}void MoveTo(const C3DVector& p){WorldPosition=p;}void PointAt(const C3DVector& p){++pointCalls;Target=p;}};
class CGameCameraBase{public:CCamera InternalCam,LastInternalCam;C3DVector LastPointingAt,PointingAt;CCamera GetRenderCamera(float)const;};
inline float GFGetInterpolatedValue(float old_val,float new_val,float interp){return (new_val-old_val)*interp+old_val;}
CCamera CGameCameraBase::GetRenderCamera(const float interpolate)const {
 CCamera passcam(InternalCam);
 C3DVector vecman;
 vecman.X=(GFGetInterpolatedValue(LastInternalCam.PeekWorldPos().X,InternalCam.PeekWorldPos().X,interpolate));
 vecman.Y=(GFGetInterpolatedValue(LastInternalCam.PeekWorldPos().Y,InternalCam.PeekWorldPos().Y,interpolate));
 vecman.Z=(GFGetInterpolatedValue(LastInternalCam.PeekWorldPos().Z,InternalCam.PeekWorldPos().Z,interpolate));
 passcam.MoveTo(vecman);
 vecman.X=(GFGetInterpolatedValue(LastPointingAt.X,PointingAt.X,interpolate));
 vecman.Y=(GFGetInterpolatedValue(LastPointingAt.Y,PointingAt.Y,interpolate));
 vecman.Z=(GFGetInterpolatedValue(LastPointingAt.Z,PointingAt.Z,interpolate));
 passcam.PointAt(vecman);
 return passcam;
}

static bool equals(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static C3DVector interpolate(const C3DVector& a,const C3DVector& b,float t){return C3DVector(a.X+(b.X-a.X)*t,a.Y+(b.Y-a.Y)*t,a.Z+(b.Z-a.Z)*t);}
int main(){
 const float times[]={-0.5f,0,0.25f,0.5f,1,1.5f};
 for(int old=0;old<4;++old)for(int now=0;now<4;++now)for(int la=0;la<4;++la)for(int na=0;na<4;++na)for(int t=0;t<6;++t){
  CGameCameraBase row;row.LastInternalCam.WorldPosition=C3DVector(old*2.0f,old*-4.0f,old*8.0f);row.InternalCam.WorldPosition=C3DVector(now*4.0f,now*2.0f,now*-2.0f);row.LastPointingAt=C3DVector(la*8.0f,la*4.0f,la*2.0f);row.PointingAt=C3DVector(na*-2.0f,na*8.0f,na*-4.0f);row.InternalCam.Flags=1234;row.InternalCam.FOV=0.5f;
  pointCalls=0;CCamera got=row.GetRenderCamera(times[t]);
  C3DVector position=interpolate(row.LastInternalCam.WorldPosition,row.InternalCam.WorldPosition,times[t]);C3DVector target=interpolate(row.LastPointingAt,row.PointingAt,times[t]);
  if(!equals(got.WorldPosition,position)||!equals(got.Target,target)||pointCalls!=1||got.Flags!=1234||got.FOV!=0.5f||row.InternalCam.WorldPosition.X!=now*4.0f)return 1;
 }
 puts("006fd5e0_TEST PASS");return 0;
}
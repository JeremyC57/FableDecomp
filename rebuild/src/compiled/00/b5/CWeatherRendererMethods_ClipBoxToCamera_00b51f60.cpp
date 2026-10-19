// CWeatherRendererMethods::ClipBoxToCamera(const CWeatherVector&,float) -- rejects boxes outside distance/frustum limits.
// Retail00b51f60:292bytes. Actual renderer/camera headers match accessed native fields.
// Nested settings ParticleBoxSpan+192 and CameraDesc WorldPosition+64 from PDB.
// Vector/matrix/settings/camera-desc standalone headers unavailable. Native minimum
// uses ordered less-than (including NaNs); distance rejection treats unordered as outside.
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineCamera.h"
#include <math.h>
struct CWeatherVector {float X,Y,Z;};
struct CWeatherMatrix {float XX,XY,XZ,YX,YY,YZ,ZX,ZY,ZZ,PX,PY,PZ;};
struct CWeatherCameraMethods:CEngineCamera {
 bool ClipBox(const CWeatherMatrix&)const; //00b30610
 const CWeatherVector& Position()const{return *reinterpret_cast<const CWeatherVector*>(CameraDesc+64);}
};
extern CWeatherCameraMethods* gWeatherCamera; //01436ea0
struct CWeatherRendererMethods:CEngineWeatherRenderer {
 int ClipBoxToCamera(const CWeatherVector&,float);
 const CWeatherVector& Span()const{return *reinterpret_cast<const CWeatherVector*>(Settings+192);}
};
#define WEATHER_MIN(a,b) ((a)<(b)?(a):(b))
int CWeatherRendererMethods::ClipBoxToCamera(const CWeatherVector& p,float range){
 CWeatherCameraMethods* camera=gWeatherCamera;
 float radius=WEATHER_MIN(WEATHER_MIN(Span().X,Span().Y),Span().Z);
 float x=camera->Position().X-p.X,y=camera->Position().Y-p.Y,z=camera->Position().Z-p.Z;
 float squared=y*y;squared+=z*z;squared+=x*x;
 if(sqrt(squared)-radius<=range){
 CWeatherMatrix m;
 m.XX=Span().X;m.YY=Span().Y;m.ZZ=Span().Z;
 m.XY=0;m.XZ=0;m.YX=0;m.YZ=0;m.ZX=0;m.ZY=0;
 m.PX=p.X;m.PY=p.Y;m.PZ=p.Z;
 if(camera->ClipBox(m))return 0;
 }
 return 1;
}
#undef WEATHER_MIN
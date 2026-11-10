// NFractal::CHeteroTerrainFractal::GetFractalHeight -- evaluate the octave-weighted terrain height.
// Flags:
// Retail: 00f36a30,361 bytes, const thiscall with a point reference.
// Leaf: ?GetFractalHeight@CHeteroTerrainFractal@NFractal@@UBENABVC2DVector@@@Z
#define FABLE_RETAIL_C2DVECTOR
#include "rebuild_abi.h"
#include "engine/NFractal.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
template<class T,class L,class U> inline void FABLE_FASTCALL GFLimit(T& value,const L& lower,const U& upper){if(value<lower)value=lower;else if(value>upper)value=upper;}
inline double FABLE_FASTCALL GFCos(float angle){return cos(angle*(double)6.2831855f);}

inline float NFractal::CNoise::SCurve(float t)const{return t*t*(3.0-2.0*t);}
inline float NFractal::CNoise::Lerp(float t,float a,float b)const{return a+t*(b-a);}
inline float NFractal::CNoise::At2(const C2DVector& point,float rx,float ry)const{return rx*point.X+ry*point.Y;}
float NFractal::CNoise::PerlinNoise(const C2DVector& point)const
{
 float t=point.X+4096.0;
 long bx0=((long)t)&255;long bx1=(bx0+1)&255;
 float rx0=t-(long)t;float rx1=rx0-1.0;
 t=point.Y+4096.0;
 long by0=((long)t)&255;long by1=(by0+1)&255;
 float ry0=t-(long)t;float ry1=ry0-1.0;
 long i=P[bx0];long j=P[bx1];
 long b00=P[i+by0],b10=P[j+by0],b01=P[i+by1],b11=P[j+by1];
 float sx=SCurve(rx0),sy=SCurve(ry0);
 float u=At2(G2[b00],rx0,ry0),v=At2(G2[b10],rx1,ry0);
 float a=Lerp(sx,u,v);
 u=At2(G2[b01],rx0,ry1);v=At2(G2[b11],rx1,ry1);
 float b=Lerp(sx,u,v);
 return Lerp(sy,a,b);
}

double NFractal::CHeteroTerrainFractal::GetFractalHeight(const C2DVector& map_point)const
{
 C2DVector point=map_point;
 const NFractal::CNoise& noise=Noise;
 double offset=0.0;
 double value=offset+noise.PerlinNoise(point);
 point.X*=Lacunarity;point.Y*=Lacunarity;
 long i;
 for(i=1;i<Octaves;++i){
  double increment=noise.PerlinNoise(point)+offset;
  increment*=ExponentArray[i];increment*=value;
  value+=increment;
  point.X*=Lacunarity;point.Y*=Lacunarity;
 }
 double remainder=Octaves-(long)Octaves;
 if(remainder!=0.0){
  double increment=(noise.PerlinNoise(point)+offset)*ExponentArray[i];
  value+=remainder*increment*value;
 }
 value+=0.48;value/=1.5;
 GFLimit(value,0.0,1.0);
 return value;
}
// NFractal::CHybridMultifractal::GetFractalHeight -- evaluate the octave-weighted terrain height.
// Flags:
// Retail: 00f36860,451 bytes, const thiscall with a point reference.
// Leaf: ?GetFractalHeight@CHybridMultifractal@NFractal@@UBENABVC2DVector@@@Z
#define FABLE_RETAIL_C2DVECTOR
#include "rebuild_abi.h"
#include "engine/NFractal.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
template<class T,class L,class U> inline void FABLE_FASTCALL GFLimit(T& value,const L& lower,const U& upper){if(value<lower)value=lower;else if(value>upper)value=upper;}
inline double FABLE_FASTCALL GFCos(float angle){return cos(angle*(double)6.2831855f);}
double NFractal::CHybridMultifractal::GetFractalHeight(const C2DVector& map_point)const
{
 C2DVector point=map_point;
 const NFractal::CNoise& noise=Noise;
 double offset=0.7f;
 double gain=1.0;
 double value=noise.PerlinNoise(point);
 value+=1.0;value/=2.0;
 value=1.0-GFCos((float)(value*0.25));
 value*=2.0;value-=1.0;value+=offset;
 double weight=gain*value;
 point.X*=Lacunarity;point.Y*=Lacunarity;
 long i;
 for(i=1;weight>0.001&&i<Octaves;++i){
  weight=weight<1.0?weight:1.0;
  double signal=noise.PerlinNoise(point);
  signal+=offset;signal*=ExponentArray[i];
  value+=weight*signal;
  weight*=gain*signal;
  point.X*=Lacunarity;point.Y*=Lacunarity;
 }
 double remainder=Octaves-(long)Octaves;
 if(remainder!=0.0)value+=remainder*noise.PerlinNoise(point)*ExponentArray[i];
 value+=0.3f;value/=3.2f;
 GFLimit(value,0.0,1.0);
 return value;
}
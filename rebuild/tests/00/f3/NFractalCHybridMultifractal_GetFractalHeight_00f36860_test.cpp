#include <math.h>
#include <assert.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C2DVector {public:float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C2DVector& p):X(p.X),Y(p.Y){}};
static float samples[128];static unsigned int cursor;static float px,py;static double lacunarity;
namespace NFractal {
class CNoise {public:float PerlinNoise(const C2DVector& p)const{assert(cursor<128);assert(fabs(p.X-px)<0.0001f&&fabs(p.Y-py)<0.0001f);px=(float)(px*lacunarity);py=(float)(py*lacunarity);return samples[cursor++];}};
class CFractal {protected:double ExponentArray[128];double Lacunarity,Octaves;CNoise& Noise;public:CFractal(CNoise& n):Noise(n){} virtual double GetFractalHeight(const C2DVector&)const=0;void configure(double o,double l){Octaves=o;Lacunarity=l;for(int i=0;i<128;++i)ExponentArray[i]=1.0/(i+1);}};
class CHybridMultifractal:public CFractal{public:CHybridMultifractal(CNoise& n):CFractal(n){}virtual double GetFractalHeight(const C2DVector&)const;};
}
template<class T,class L,class U> inline void __fastcall GFLimit(T& value,const L& lower,const U& upper){if(value<lower)value=lower;else if(value>upper)value=upper;}
inline double __fastcall GFCos(float angle){return cos(angle*(double)6.2831855f);}
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

static double oracle(double oct,unsigned int& count){double value=samples[0];value=(value+1)/2;value=1-cos((float)(value*0.25)*(double)6.2831855f);value=value*2-1+(double)0.7f;double weight=value;long i=1;count=1;while(weight>0.001&&i<oct){if(!(weight<1))weight=1;double signal=(samples[count++]+(double)0.7f)/(i+1);value+=weight*signal;weight*=signal;++i;}double rem=oct-(long)oct;if(rem!=0)value+=rem*samples[count++]/(i+1);value=(value+(double)0.3f)/(double)3.2f;return value<0?0:value>1?1:value;}
int main(){NFractal::CNoise n;NFractal::CHybridMultifractal f(n);C2DVector point(0.125f,-0.25f);unsigned int seed=47;
for(int o=4;o<=28;++o)for(int k=0;k<32;++k){for(int i=0;i<128;++i){seed=1664525u*seed+1013904223u;samples[i]=(float)((seed>>8)%2001)/1000-1;}
double oct=o*0.25;f.configure(oct,1.125);cursor=0;px=point.X;py=point.Y;lacunarity=1.125;unsigned int count;double expected=oracle(oct,count);double actual=f.GetFractalHeight(point);assert(cursor==count);assert(fabs(actual-expected)<0.000002);}
puts("00f36860_TEST PASS");return 0;}
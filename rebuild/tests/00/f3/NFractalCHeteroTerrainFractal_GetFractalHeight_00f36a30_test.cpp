#include <math.h>
#include <assert.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C2DVector {public:float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C2DVector& p):X(p.X),Y(p.Y){}};
namespace NFractal {
class CFractalFixture;
class CNoise {
 friend class CFractalFixture;
private:
 long P[512];C2DVector G2[512];unsigned long Seed;
 void InitPerlinNoise();float SCurve(float)const;float Lerp(float,float,float)const;float At2(const C2DVector&,float,float)const;
public:
 CNoise(){}CNoise(long);CNoise(const CNoise&);~CNoise(){}void SetSeed(long);float PerlinNoise(const C2DVector&)const;
};
class CFractal {
 friend class CFractalFixture;
protected:
 double ExponentArray[128];double Lacunarity;double Octaves;CNoise& Noise;
public:
 CFractal(CNoise&,double,double,double);CFractal(CNoise& n):Noise(n){}CFractal(const CFractal&);
 void NewFractalData(double,double,double);
 virtual double GetFractalHeight(const C2DVector&)const=0;
};
class CHybridMultifractal:public CFractal {public:CHybridMultifractal(CNoise&);CHybridMultifractal(const CHybridMultifractal&);virtual double GetFractalHeight(const C2DVector&)const;};
class CHeteroTerrainFractal:public CFractal {public:CHeteroTerrainFractal(CNoise& n):CFractal(n){}CHeteroTerrainFractal(const CHeteroTerrainFractal&);virtual double GetFractalHeight(const C2DVector&)const;};
}
namespace NFractal {class CFractalFixture {public:
 static void fill(CNoise& n){for(int i=0;i<512;++i){n.P[i]=i&255;n.G2[i].X=(float)((i%256)%3-1)*0.5f;n.G2[i].Y=(float)((i%256)%5-2)*0.25f;}n.Seed=0;}
 static void setup(CFractal& f,double o,double l){f.Octaves=o;f.Lacunarity=l;for(int i=0;i<128;++i)f.ExponentArray[i]=1.0/(i+1);}
};}
template<class T,class L,class U> inline void __fastcall GFLimit(T& value,const L& lower,const U& upper){if(value<lower)value=lower;else if(value>upper)value=upper;}
inline double __fastcall GFCos(float angle){return cos(angle*(double)6.2831855f);}

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

static double gradient(int index,double x,double y){index&=255;return ((index%3)-1)*0.5*x+((index%5)-2)*0.25*y;}
static double independentNoise(float x,float y){float tx=(float)(x+4096.0),ty=(float)(y+4096.0);int ix=(int)tx,iy=(int)ty;double rx=tx-ix,ry=ty-iy,sx=rx*rx*(3-2*rx),sy=ry*ry*(3-2*ry);double a=gradient((ix&255)+(iy&255),rx,ry),b=gradient(((ix+1)&255)+(iy&255),rx-1,ry),c=gradient((ix&255)+((iy+1)&255),rx,ry-1),e=gradient(((ix+1)&255)+((iy+1)&255),rx-1,ry-1);return (a+(b-a)*sx)+((c+(e-c)*sx)-(a+(b-a)*sx))*sy;}
static double oracle(C2DVector p,double oct,double lac){double value=independentNoise(p.X,p.Y);p.X=(float)(p.X*lac);p.Y=(float)(p.Y*lac);int i=1;for(;i<oct;++i){double inc=independentNoise(p.X,p.Y)/(i+1)*value;value+=inc;p.X=(float)(p.X*lac);p.Y=(float)(p.Y*lac);}double r=oct-(int)oct;if(r!=0)value+=r*(independentNoise(p.X,p.Y)/(i+1))*value;value=(value+0.48)/1.5;return value<0?0:value>1?1:value;}
int main(){NFractal::CNoise noise;NFractal::CFractalFixture::fill(noise);
for(int x=-16;x<=16;++x)for(int y=-16;y<=16;++y){C2DVector p(x*0.137f,y*0.183f);assert(fabs(noise.PerlinNoise(p)-independentNoise(p.X,p.Y))<0.000002);}
NFractal::CHeteroTerrainFractal fractal(noise);
for(int o=4;o<=28;++o)for(int j=0;j<32;++j){double oct=o*0.25,lac=1.125;C2DVector p((j-16)*0.067f,(j%7-3)*0.137f);NFractal::CFractalFixture::setup(fractal,oct,lac);assert(fabs(fractal.GetFractalHeight(p)-oracle(p,oct,lac))<0.000002);}
puts("00f36a30_TEST PASS");return 0;}
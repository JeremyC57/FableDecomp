#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#define CHECK(x) do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
// 006b3030 CEnvironment::UpdateWind
// Leaf: ?UpdateWind@CEnvironment@@AAEXXZ
// Flags:
extern unsigned long GFastOneOverSqrtTable[128];
float __fastcall GFFloatRandom(float,unsigned long*);
template<class T,class L,class H>inline void __fastcall GFLimit(T&value,const L&lo,const H&hi){if(value<lo)value=lo;else if(value>hi)value=hi;}
__forceinline float __fastcall GFFastOneOverSqrt(float value){unsigned int bits;memcpy(&bits,&value,4);unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r;memcpy(&r,&estimate,4);return (3.0f-r*r*value)*r*0.5f;}
class C3DVector {public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    const C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    void Rotate(float zAngle, float xAngle);      // 0x00A14260
    __forceinline float GetSquaredMagnitude()const{const float x=X;const float y=Y;const float z=Z;float result=z*z;result+=y*y;result+=x*x;return result;}
    __forceinline const C3DVector&operator*=(float value){X*=value;Y*=value;Z*=value;return *this;}
    __forceinline C3DVector&Normalise(){float r=GFFastOneOverSqrt(GetSquaredMagnitude());X*=r;Y*=r;Z*=r;return *this;}
};


// Layout: supplied environment GameTickState at54; nested WindSpeed0xbbc, vectors0xbc0/0xbd0, flag0xc0c.
class CEnvironment{public:
 class CEnvironmentGameTickState{unsigned char _pad_0[0xbbc];public:float WindSpeed;C3DVector WindDirection;float ThemeWindSpeed;C3DVector ThemeWindDirection;unsigned char _pad_0xbdc[0xc0c-0xbdc];bool OverrideWindDirection;};
 unsigned char test_prefix[0x54]; CEnvironmentGameTickState GameTickState;
 void UpdateWind();
};
void CEnvironment::UpdateWind(){
 static unsigned long seed;
 CEnvironmentGameTickState&state=reinterpret_cast<CEnvironmentGameTickState&>(GameTickState);
 if(state.OverrideWindDirection)return;
 float angle=(GFFloatRandom(195.0f,&seed)-97.5f)*0.0027777778f;
 state.WindDirection.Rotate(angle,0.0f);
 state.WindDirection.Normalise();
 state.WindSpeed=state.WindSpeed-0.225f+GFFloatRandom(0.45f,&seed);
 GFLimit(state.WindSpeed,0.04f,0.2f);
 float fraction=(state.WindSpeed-0.04f)*6.25f;
 float blend=fraction*0.09f+0.01f;
 state.ThemeWindDirection+=(state.WindDirection-state.ThemeWindDirection)*blend;
 state.ThemeWindDirection.Normalise();
 state.ThemeWindSpeed+=(state.WindSpeed-state.ThemeWindSpeed)*blend;
}

unsigned long GFastOneOverSqrtTable[128]={106,103,100,98,95,92,90,87,85,83,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,51,49,47,46,44,42,41,39,38,36,35,33,32,31,29,28,26,25,24,22,21,20,19,17,16,15,14,13,12,10,9,8,7,6,5,4,3,2,1,255,252,248,244,240,237,233,230,226,223,220,217,214,211,208,205,202,199,196,193,191,188,185,183,180,178,176,173,171,168,166,164,162,160,158,155,153,151,149,147,145,143,142,140,138,136,134,133,131,129,127,126,124,122,121,119,118,116,115,113,112,110,109,107};

static int calls,rotations,scenario;static float second_random,first_random;static unsigned long*saved_seed;
float __fastcall GFFloatRandom(float maximum,unsigned long*seed){CHECK(seed!=0);if(!saved_seed)saved_seed=seed;CHECK(seed==saved_seed);++calls;if(calls==1){CHECK(maximum==195.0f);return first_random;}CHECK(calls==2&&maximum==0.45f);return second_random;}
void C3DVector::Rotate(float angle,float other){++rotations;CHECK(other==0.0f);float expected=(first_random-97.5f)*0.0027777778f;CHECK(fabs(angle-expected)<0.000001);float old=X;if(angle>0){X=-Y;Y=old;}else if(angle<0){X=Y;Y=-old;}}
static void Norm(double*x){double n=sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);for(int k=0;k<3;++k)x[k]/=n;}
static bool Near(double a,double b){return fabs(a-b)<0.0001;}
#ifdef _MSC_VER
typedef char test_state_offset[offsetof(CEnvironment,GameTickState)==0x54?1:-1];
typedef char test_wind_offset[offsetof(CEnvironment::CEnvironmentGameTickState,WindDirection)==0xbc0?1:-1];
typedef char test_theme_offset[offsetof(CEnvironment::CEnvironmentGameTickState,ThemeWindSpeed)==0xbcc?1:-1];
#endif
int main(){int cases=0;for(int blocked=0;blocked<2;++blocked)for(int axis=0;axis<6;++axis)for(int theme=0;theme<6;++theme)for(int speed=0;speed<5;++speed)for(int rng=0;rng<4;++rng){
 CEnvironment environment;CEnvironment::CEnvironmentGameTickState&state=environment.GameTickState;
 double wind[3]={0,0,0},th[3]={0,0,0};wind[axis/2]=(axis%2)?-1.0:1.0;th[theme/2]=(theme%2)?-1.0:1.0;
 state.WindDirection=C3DVector(float(wind[0]),float(wind[1]),float(wind[2]));state.ThemeWindDirection=C3DVector(float(th[0]),float(th[1]),float(th[2]));state.WindSpeed=float(speed)*0.0625f;state.ThemeWindSpeed=0.125f;state.OverrideWindDirection=blocked!=0;
 double original_speed=state.WindSpeed;calls=rotations=0;first_random=(rng==0)?0.0f:(rng==1)?97.5f:(rng==2)?130.0f:195.0f;second_random=float(rng)*0.125f;
 environment.UpdateWind();
 if(blocked){CHECK(calls==0&&rotations==0);CHECK(state.WindSpeed==original_speed&&state.ThemeWindSpeed==0.125f);}
 else{CHECK(calls==2&&rotations==1);double old=wind[0];if(first_random>97.5f){wind[0]=-wind[1];wind[1]=old;}else if(first_random<97.5f){wind[0]=wind[1];wind[1]=-old;}Norm(wind);double ws=original_speed-double(0.225f)+second_random;if(ws<double(0.04f))ws=double(0.04f);else if(ws>double(0.2f))ws=double(0.2f);double blend=(ws-double(0.04f))*6.25*double(0.09f)+double(0.01f);for(int k=0;k<3;++k)th[k]+=(wind[k]-th[k])*blend;Norm(th);CHECK(Near(state.WindSpeed,ws));CHECK(Near(state.ThemeWindSpeed,0.125+(ws-0.125)*blend));}
 CHECK(Near(state.WindDirection.X,wind[0])&&Near(state.WindDirection.Y,wind[1])&&Near(state.WindDirection.Z,wind[2]));CHECK(Near(state.ThemeWindDirection.X,th[0])&&Near(state.ThemeWindDirection.Y,th[1])&&Near(state.ThemeWindDirection.Z,th[2]));++cases;
 }CHECK(cases==1440);puts("006b3030_TEST PASS");return 0;}
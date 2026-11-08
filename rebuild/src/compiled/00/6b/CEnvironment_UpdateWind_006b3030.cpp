// 006b3030 CEnvironment::UpdateWind
// Leaf: ?UpdateWind@CEnvironment@@AAEXXZ
// Flags:
#include "fable_compiler.h"
#define CEnvironment CEnvironmentFields
#include "engine/CEnvironment.h"
#undef CEnvironment
extern unsigned long GFastOneOverSqrtTable[128];
float __fastcall GFFloatRandom(float,unsigned long*);
template<class T,class L,class H>inline void __fastcall GFLimit(T&value,const L&lo,const H&hi){if(value<lo)value=lo;else if(value>hi)value=hi;}
FABLE_FORCEINLINE float __fastcall GFFastOneOverSqrt(float value){unsigned long bits=*reinterpret_cast<unsigned long*>(&value);unsigned long estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r=*reinterpret_cast<float*>(&estimate);return (3.0f-r*r*value)*r*0.5f;}
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
    FABLE_FORCEINLINE float GetSquaredMagnitude()const{const float x=X;const float y=Y;const float z=Z;float result=z*z;result+=y*y;result+=x*x;return result;}
    FABLE_FORCEINLINE const C3DVector&operator*=(float value){X*=value;Y*=value;Z*=value;return *this;}
    FABLE_FORCEINLINE C3DVector&Normalise(){float r=GFFastOneOverSqrt(GetSquaredMagnitude());X*=r;Y*=r;Z*=r;return *this;}
};


// Layout: supplied environment GameTickState at54; nested WindSpeed0xbbc, vectors0xbc0/0xbd0, flag0xc0c.
class CEnvironment:public CEnvironmentFields{
 class CEnvironmentGameTickState{unsigned char _pad_0[0xbbc];public:float WindSpeed;C3DVector WindDirection;float ThemeWindSpeed;C3DVector ThemeWindDirection;unsigned char _pad_0xbdc[0xc0c-0xbdc];bool OverrideWindDirection;};
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
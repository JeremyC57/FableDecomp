#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// CEnvironmentGameTickState::operator= -- copies six blended themes and wind/light state.
// Retail006b63c0 length317; manifest CEnvironmentThemeDef is wrong.
// PDB CEnvironmentGameTickState layout establishes all field names; donor blended theme524
// shrinks to500 natively because its six vector descriptors use12 vs16bytes.
// PDB explicitly puts OldWindDirection at unaligned+0xc9d (native+0xc0d), so vector
// alignment1 is real layout evidence, not a hidden offset adjustment.
#pragma pack(push,1)
struct CThemeVector {float X,Y,Z;};
#pragma pack(pop)
struct CThemeColour {float R,G,B,A;};
template<class T> struct CThemeArray {T* Begin;T* End;T* Capacity;};
struct CThemeTextureSet {CThemeArray<int> Textures;CThemeArray<float> Blends;};
struct CBlendedEnvironmentTheme {
 float TimeOfDay;CThemeColour Colours[19];float Scalers[30];CThemeTextureSet TextureSets[3];
 CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme&); //006b5df0
};
struct CEnvironmentGameTickState {
 CBlendedEnvironmentTheme BlendedTheme[6];float Time,WindSpeed;CThemeVector WindDirection;
 float ThemeWindSpeed;CThemeVector ThemeWindDirection,SunVector,MoonVector,DiffuseVector,ShadowVector;
 bool OverrideWindDirection;CThemeVector OldWindDirection;float OldWindSpeed;
};
void AssignEnvironmentState(CEnvironmentGameTickState& a,const CEnvironmentGameTickState& b){a=b;}



#include <stdio.h>
#include <stddef.h>
#ifdef _MSC_VER
typedef char ThemeSize[(sizeof(CBlendedEnvironmentTheme)==500)?1:-1];
typedef char StateSize[(sizeof(CEnvironmentGameTickState)==0xc20)?1:-1];
typedef char OldWindOffset[(offsetof(CEnvironmentGameTickState,OldWindDirection)==0xc0d)?1:-1];
#endif
static int bad,calls;static CEnvironmentGameTickState*dst;static const CEnvironmentGameTickState*src;
static void Check(bool b){if(!b)bad=1;}
template<class T>static void Copy(CThemeArray<T>&a,const CThemeArray<T>&b){if(&a==&b)return;int n=b.End?int(b.End-b.Begin):0;T*p=n?new T[n]:0;for(int i=0;i<n;++i)p[i]=b.Begin[i];delete[]a.Begin;a.Begin=p;a.End=a.Capacity=p?p+n:0;}
CBlendedEnvironmentTheme& CBlendedEnvironmentTheme::operator=(const CBlendedEnvironmentTheme&b){
 Check(calls<6&&this==&dst->BlendedTheme[calls]&&&b==&src->BlendedTheme[calls]);++calls;
 TimeOfDay=b.TimeOfDay;for(int i=0;i<19;++i)Colours[i]=b.Colours[i];for(int j=0;j<30;++j)Scalers[j]=b.Scalers[j];
 for(int k=0;k<3;++k){Copy(TextureSets[k].Textures,b.TextureSets[k].Textures);Copy(TextureSets[k].Blends,b.TextureSets[k].Blends);}return *this;
}
static void Fill(CThemeVector&v,float n){v.X=n;v.Y=n+1;v.Z=n+2;}
static bool Equal(const CThemeVector&a,const CThemeVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){CEnvironmentGameTickState a={},b={};
 for(int i=0;i<6;++i){CBlendedEnvironmentTheme&t=b.BlendedTheme[i];t.TimeOfDay=float(i);for(int j=0;j<19;++j){t.Colours[j].R=float(i+j);t.Colours[j].G=float(j+1);t.Colours[j].B=float(j+2);t.Colours[j].A=float(j+3);}for(int j=0;j<30;++j)t.Scalers[j]=float(i*30+j);
 for(int k=0;k<3;++k){CThemeTextureSet&q=t.TextureSets[k];q.Textures.Begin=new int[2];q.Textures.End=q.Textures.Capacity=q.Textures.Begin+2;q.Textures.Begin[0]=i;q.Textures.Begin[1]=k;q.Blends.Begin=new float[1];q.Blends.End=q.Blends.Capacity=q.Blends.Begin+1;q.Blends.Begin[0]=.25f+i;}}
 b.Time=12;b.WindSpeed=13;b.ThemeWindSpeed=14;b.OldWindSpeed=15;b.OverrideWindDirection=true;
Fill(b.WindDirection,0);
Fill(b.ThemeWindDirection,3);
Fill(b.SunVector,6);
Fill(b.MoonVector,9);
Fill(b.DiffuseVector,12);
Fill(b.ShadowVector,15);
Fill(b.OldWindDirection,18);
dst=&a;src=&b;AssignEnvironmentState(a,b);Check(calls==6);Check(a.Time==12&&a.WindSpeed==13&&a.ThemeWindSpeed==14&&a.OldWindSpeed==15&&a.OverrideWindDirection);
Check(Equal(a.WindDirection,b.WindDirection));
Check(Equal(a.ThemeWindDirection,b.ThemeWindDirection));
Check(Equal(a.SunVector,b.SunVector));
Check(Equal(a.MoonVector,b.MoonVector));
Check(Equal(a.DiffuseVector,b.DiffuseVector));
Check(Equal(a.ShadowVector,b.ShadowVector));
Check(Equal(a.OldWindDirection,b.OldWindDirection));
for(int i=0;i<6;++i){Check(a.BlendedTheme[i].TimeOfDay==b.BlendedTheme[i].TimeOfDay);for(int j=0;j<19;++j)Check(a.BlendedTheme[i].Colours[j].B==b.BlendedTheme[i].Colours[j].B);for(int j=0;j<30;++j)Check(a.BlendedTheme[i].Scalers[j]==b.BlendedTheme[i].Scalers[j]);for(int k=0;k<3;++k){Check(a.BlendedTheme[i].TextureSets[k].Textures.Begin!=b.BlendedTheme[i].TextureSets[k].Textures.Begin);Check(a.BlendedTheme[i].TextureSets[k].Textures.Begin[1]==k);}}
 calls=0;src=&a;AssignEnvironmentState(a,a);Check(calls==6);
 for(int i=0;i<6;++i)for(int k=0;k<3;++k){delete[]a.BlendedTheme[i].TextureSets[k].Textures.Begin;delete[]a.BlendedTheme[i].TextureSets[k].Blends.Begin;delete[]b.BlendedTheme[i].TextureSets[k].Textures.Begin;delete[]b.BlendedTheme[i].TextureSets[k].Blends.Begin;}
 printf("006b63c0_TEST %s\n",bad?"FAIL":"PASS");return bad;}
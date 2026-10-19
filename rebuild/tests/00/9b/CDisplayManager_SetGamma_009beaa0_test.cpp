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
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif
// CDisplayManager::SetGamma(float, bool) -- clamp gamma and submit an identical RGB ramp to D3D9
// Retail009BEAA0,266bytes. Gamma+2C4 PD3DDevice+58. D3D9 SetGammaRamp slot21 (+54), stdcall member ABI.
// Constants1238170=.3,125A2A0=6,122DEDC=0,129B3F0=65280 independently read from native PE.
// Leaf: SetGamma
// Flags: /GS
#include <stddef.h>
#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif

struct GammaRamp { unsigned short Red[256],Green[256],Blue[256]; };
struct NativeD3DDevice {
 void* VTable;
 typedef void (__stdcall NativeD3DDevice::*GammaMethod)(unsigned int,unsigned int,const GammaRamp*);
 void SetGammaRamp(unsigned int chain,unsigned int flags,const GammaRamp* ramp) {
  (this->*static_cast<GammaMethod*>(VTable)[21])(chain,flags,ramp);
 }
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NativeD3DDevice::GammaMethod)==4);
#endif
extern const float g_RampZero; //0122DEDC
extern const float g_RampMaximum; //0129B3F0
struct CDisplayManager {
 unsigned char DisplayState[0x58];NativeD3DDevice* PD3DDevice;
 unsigned char DisplayStateTail[0x2c4-0x5c];float Gamma;
 void SetGamma(float gamma,bool calibrate);
};
void CDisplayManager::SetGamma(float gamma,bool calibrate) {
 if(gamma<0.3f)gamma=0.3f;else if(gamma>6.0f)gamma=6.0f;
 Gamma=gamma;GammaRamp ramp;
 for(int i=0;i<=255;++i){
  float value=(i*256)*gamma;
  value=value<g_RampZero?g_RampZero:(value>g_RampMaximum?g_RampMaximum:value);
  unsigned short v=(unsigned short)value;
  ramp.Red[i]=v;ramp.Green[i]=v;ramp.Blue[i]=v;
 }
 PD3DDevice->SetGammaRamp(0,calibrate?1:0,&ramp);
}

const float g_RampZero=0.0f;
const float g_RampMaximum=65280.0f;
static int bad,calls;
static CDisplayManager display;
static float expectedGamma;
static unsigned int expectedFlags;
static void Check(bool b){if(!b)++bad;}
struct FixtureDevice : NativeD3DDevice {
 void __stdcall Capture(unsigned int chain,unsigned int flags,const GammaRamp* p) {
  ++calls;Check(chain==0&&flags==expectedFlags&&display.Gamma==expectedGamma);
  for(int i=0;i!=256;++i){
   double x=(double)(i*256)*expectedGamma;if(x>65280)x=65280;
   unsigned short v=(unsigned short)x;
   Check(p->Red[i]==v&&p->Green[i]==v&&p->Blue[i]==v);
  }
  Check(p->Red[0]==0);
 }
};
static void Run(float value,float clamped,bool calibrate) {
 expectedGamma=clamped;expectedFlags=calibrate?1:0;int before=calls;
 display.SetGamma(value,calibrate);Check(calls==before+1&&display.Gamma==clamped);
}
int main(){
 NativeD3DDevice::GammaMethod table[22];for(int i=0;i<22;++i)table[i]=0;
 table[21]=static_cast<NativeD3DDevice::GammaMethod>(&FixtureDevice::Capture);
 FixtureDevice device;device.VTable=table;display.PD3DDevice=&device;
 Run(-1,0.3f,false);Run(0.3f,0.3f,true);Run(1,1,false);Run(6,6,true);Run(9,6,false);
 unsigned int infbits=0x7f800000;float inf;memcpy(&inf,&infbits,4);
 Run(inf,6,true);Run(-inf,0.3f,false);
 printf("009beaa0_TEST %s\n",bad?"FAIL":"PASS");return bad?1:0;
}
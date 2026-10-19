// CDisplayManager::SetGamma(float, bool) -- clamp gamma and submit an identical RGB ramp to D3D9
// Retail009BEAA0,266bytes. Gamma+2C4 PD3DDevice+58. D3D9 SetGammaRamp slot21 (+54), stdcall member ABI.
// Constants1238170=.3,125A2A0=6,122DEDC=0,129B3F0=65280 independently read from native PE.
// Leaf: SetGamma
// Flags: /GS
#include <stddef.h>
#include "rebuild_abi.h"
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
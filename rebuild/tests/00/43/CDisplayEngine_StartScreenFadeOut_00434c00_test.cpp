#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// CDisplayEngine::StartScreenFadeOut: clear initial fade and open the unlocked screen fade.
// Grade: pending owner naming preflight; historical exact grade2 preserved
// Leaf: ?StartScreenFadeOut@CDisplayEngine@@QAEXMMVCRGBColour@@M@Z
// Flags:
#define CDisplayEngine CDisplayEngineFields
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
#define CFadeInFadeOutBase CFadeInFadeOutBaseFields
#include "engine/CFadeInFadeOutBase.h"
#undef CFadeInFadeOutBase
class CRGBColour { public:
 union { struct { unsigned char B,G,R,A; } Components; };
 CRGBColour(){}
 CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){Initialise(r,g,b,a);}
 CRGBColour(const CRGBColour&c){Initialise(c.Components.R,c.Components.G,c.Components.B,c.Components.A);}
 __forceinline void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
 {Components.R=r;Components.G=g;Components.B=b;Components.A=a;}
};
class CBaseClass{public:virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
class CFadeInFadeOutBase {public:
 bool Active;float FadeInTime,FadeOutTime;bool Closing,Opening;
 float OpenTimer,CloseTimer;CRGBColour ToColour;
 void Open(float fadeInTime,float fadeOutTime,CRGBColour colour){
  FadeInTime=fadeInTime;FadeOutTime=fadeOutTime;Active=true;Closing=false;Opening=true;OpenTimer=0.0f;CloseTimer=fadeOutTime;
  ToColour=colour;
 }
};
typedef char CFadeLayoutCheck[sizeof(CFadeInFadeOutBase)==sizeof(CFadeInFadeOutBaseFields)?1:-1];
extern bool CUnprovenGlobal_01375cdc;
class CDisplayEngine:public CBaseClassNonCopyable {public:
 void StartScreenFadeOut(float,float,CRGBColour,float);
};
void CDisplayEngine::StartScreenFadeOut(float fadeInTime,float fadeOutTime,CRGBColour colour,float duration)
{
 CDisplayEngineFields& fields=*reinterpret_cast<CDisplayEngineFields*>(this);
 fields.InitialFadeActive=false;CUnprovenGlobal_01375cdc=false;
 if(fields.ScreenFadeOutLocked){fields.InitialFadeReadynessCount=0;return;}
 fields.ScreenFadeDuration=duration;fields.TimeSinceFadeStarted=0.0f;
 reinterpret_cast<CFadeInFadeOutBase*>(fields.ScreenFadeOutInfo)->Open(fadeInTime,fadeOutTime,colour);
}

CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
bool CUnprovenGlobal_01375cdc=true;
class TestDisplay:public CDisplayEngine {public:unsigned char storage[sizeof(CDisplayEngineFields)-4];};
int main(){TestDisplay display;
 CDisplayEngineFields& f=*reinterpret_cast<CDisplayEngineFields*>(&display);
 std::memset(reinterpret_cast<unsigned char*>(&display)+4,0,sizeof(display)-4);
 CFadeInFadeOutBaseFields& fade=*reinterpret_cast<CFadeInFadeOutBaseFields*>(f.ScreenFadeOutInfo);
 CRGBColour c(0x12,0x34,0x56,0x78);
 f.ScreenFadeOutLocked=true;f.InitialFadeActive=true;f.InitialFadeReadynessCount=9;
 f.ScreenFadeDuration=99;fade.FadeInTime=77;
 display.StartScreenFadeOut(1.25f,2.5f,c,8);
 if(f.InitialFadeActive||CUnprovenGlobal_01375cdc||f.InitialFadeReadynessCount||f.ScreenFadeDuration!=99||fade.FadeInTime!=77)return 1;
 f.ScreenFadeOutLocked=false;f.InitialFadeActive=true;CUnprovenGlobal_01375cdc=true;f.InitialFadeReadynessCount=4;
 display.StartScreenFadeOut(1.25f,2.5f,c,8);
 if(f.InitialFadeActive||CUnprovenGlobal_01375cdc||f.InitialFadeReadynessCount!=4||f.ScreenFadeDuration!=8||f.TimeSinceFadeStarted!=0)return 2;
 if(!fade.Active||fade.Closing||!fade.Opening||fade.FadeInTime!=1.25f||fade.FadeOutTime!=2.5f||fade.CloseTimer!=2.5f||fade.OpenTimer!=0)return 3;
 if(fade.ToColour[0]!=0x56||fade.ToColour[1]!=0x34||fade.ToColour[2]!=0x12||fade.ToColour[3]!=0x78)return 4;
 std::puts("00434c00_TEST PASS");return 0;}
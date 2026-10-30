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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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
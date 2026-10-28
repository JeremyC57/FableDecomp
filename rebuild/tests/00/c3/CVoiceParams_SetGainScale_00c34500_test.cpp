#include <stdio.h>
#include <string.h>
// CVoiceParams::SetGainScale(float value) -- Change a stored parameter and return an integer changed flag.
// Leaf: ?SetGainScale@CVoiceParams@@QAEHM@Z
// Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20c80 and neighboring GetGain00c34440 multiply field48 with Gain44. Manifest CGuiInputProcess::AddToInterface is wrong.
// Layout: no active shared header; untouched prefix48, own float field at48. Return is full-width0/1 as in retail.
class CVoiceParams{public:unsigned char UnmodeledPrefix[48];float GainScale;int SetGainScale(float);};
int CVoiceParams::SetGainScale(float value){if(GainScale==value)return 0;GainScale=value;return 1;}
int main(){CVoiceParams params;memset(params.UnmodeledPrefix,0x5a,sizeof(params.UnmodeledPrefix));params.GainScale=1.5f;bool ok=params.SetGainScale(1.5f)==0&&params.GainScale==1.5f;ok=ok&&params.SetGainScale(-2.0f)==1&&params.GainScale==-2.0f;ok=ok&&params.SetGainScale(0.0f)==1&&params.SetGainScale(-0.0f)==0;for(unsigned int i=0;i<sizeof(params.UnmodeledPrefix);++i)ok=ok&&params.UnmodeledPrefix[i]==0x5a;puts(ok?"00c34500_TEST PASS":"00c34500_TEST FAIL");return ok?0:1;}
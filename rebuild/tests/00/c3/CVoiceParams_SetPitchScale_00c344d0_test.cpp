#include <stdio.h>
#include <string.h>
// CVoiceParams::SetPitchScale(float value) -- Change a stored parameter and return an integer changed flag.
// Leaf: ?SetPitchScale@CVoiceParams@@QAEHM@Z
// Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20c50 and neighboring GetPitch00c34410 multiply field56 with Pitch52. Manifest CGuiInputProcess::AddToInterface is wrong.
// Layout: no active shared header; untouched prefix56, own float field at56. Return is full-width0/1 as in retail.
class CVoiceParams{public:unsigned char UnmodeledPrefix[56];float PitchScale;int SetPitchScale(float);};
int CVoiceParams::SetPitchScale(float value){if(PitchScale==value)return 0;PitchScale=value;return 1;}
int main(){CVoiceParams params;memset(params.UnmodeledPrefix,0x5a,sizeof(params.UnmodeledPrefix));params.PitchScale=1.5f;bool ok=params.SetPitchScale(1.5f)==0&&params.PitchScale==1.5f;ok=ok&&params.SetPitchScale(-2.0f)==1&&params.PitchScale==-2.0f;ok=ok&&params.SetPitchScale(0.0f)==1&&params.SetPitchScale(-0.0f)==0;for(unsigned int i=0;i<sizeof(params.UnmodeledPrefix);++i)ok=ok&&params.UnmodeledPrefix[i]==0x5a;puts(ok?"00c344d0_TEST PASS":"00c344d0_TEST FAIL");return ok?0:1;}
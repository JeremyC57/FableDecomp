#include <stdio.h>
#include <string.h>
// CVoiceParams::SetUnprovenParameter28(float value) -- Change a stored parameter and return an integer changed flag.
// Leaf: ?SetUnprovenParameter28@CVoiceParams@@QAEHM@Z
// Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20eb0 passes the float to field40; original parameter role not established. Manifest CGuiInputProcess::AddToInterface is wrong.
// Layout: no active shared header; untouched prefix40, own float field at40. Return is full-width0/1 as in retail.
class CVoiceParams{public:unsigned char UnmodeledPrefix[40];float UnprovenParameter28;int SetUnprovenParameter28(float);};
int CVoiceParams::SetUnprovenParameter28(float value){if(UnprovenParameter28==value)return 0;UnprovenParameter28=value;return 1;}
int main(){CVoiceParams params;memset(params.UnmodeledPrefix,0x5a,sizeof(params.UnmodeledPrefix));params.UnprovenParameter28=1.5f;bool ok=params.SetUnprovenParameter28(1.5f)==0&&params.UnprovenParameter28==1.5f;ok=ok&&params.SetUnprovenParameter28(-2.0f)==1&&params.UnprovenParameter28==-2.0f;ok=ok&&params.SetUnprovenParameter28(0.0f)==1&&params.SetUnprovenParameter28(-0.0f)==0;for(unsigned int i=0;i<sizeof(params.UnmodeledPrefix);++i)ok=ok&&params.UnmodeledPrefix[i]==0x5a;puts(ok?"00c34580_TEST PASS":"00c34580_TEST FAIL");return ok?0:1;}
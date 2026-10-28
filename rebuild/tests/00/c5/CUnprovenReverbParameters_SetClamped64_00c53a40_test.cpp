#include <stdio.h>
#include <string.h>
// CUnprovenReverbParameters::SetClamped64(float value) -- Clamp an audio parameter and mark its dirty bit.
// Leaf: ?SetClamped64@CUnprovenReverbParameters@@QAEXM@Z
// Identity: Own callers 00c53ef3 / 00c5437d / 00c5465a; values clamped to[0.1f,100.0f] and corresponding dirty mask 0x1. Descriptive parameter identity only.
// Layout: no active shared header. Caller passes its subobject+0x290 to all three methods; common flags at0, float parameters40/48/100. Remaining members are not modeled. Libpk has no original PDB class name evidence here.
class CUnprovenReverbParameters{public:unsigned long DirtyFlags;unsigned char Unmodeled04[36];float Float28;unsigned char Unmodeled2C[4];float Float30;unsigned char Unmodeled34[48];float Float64;void SetClamped28(float);void SetClamped30(float);void SetClamped64(float);};
void CUnprovenReverbParameters::SetClamped64(float value){Float64=value<0.1f?0.1f:(value>100.0f?100.0f:value);DirtyFlags|=0x1;}
static float FromBits(unsigned int bits){float result;memcpy(&result,&bits,4);return result;}
int main(){CUnprovenReverbParameters p;memset(&p,0,sizeof(p));float inputs[]={-2.0f,0.0f,0.0625f,0.25f,0.5f,16.0f,100.0f,128.0f};float low=FromBits(0x3dcccccd),high=FromBits(0x42c80000);bool ok=true;for(int i=0;i<8;++i){p.DirtyFlags=0x40000000;p.SetClamped64(inputs[i]);float expected=inputs[i];if(expected<low)expected=low;else if(expected>high)expected=high;ok=ok&&p.Float64==expected&&p.DirtyFlags==(0x40000000|0x1);}p.DirtyFlags=0;p.SetClamped64(p.Float64);ok=ok&&p.DirtyFlags==0x1;puts(ok?"00c53a40_TEST PASS":"00c53a40_TEST FAIL");return ok?0:1;}
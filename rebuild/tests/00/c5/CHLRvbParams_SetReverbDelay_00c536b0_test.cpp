#include <stdio.h>
#include <string.h>
// CUnprovenReverbParameters::SetClamped30(float value) -- Clamp an audio parameter and mark its dirty bit.
// Leaf: ?SetClamped30@CUnprovenReverbParameters@@QAEXM@Z
// Identity: Own callers 00c53e33 / 00c5434d / 00c5464e; values clamped to[0.0f,0.1f] and corresponding dirty mask 0x10000. Descriptive parameter identity only.
// Layout: no active shared header. Caller passes its subobject+0x290 to all three methods; common flags at0, float parameters40/48/100. Remaining members are not modeled. Libpk has no original PDB class name evidence here.
class CUnprovenReverbParameters{public:unsigned long DirtyFlags;unsigned char Unmodeled04[36];float Float28;unsigned char Unmodeled2C[4];float Float30;unsigned char Unmodeled34[48];float Float64;void SetClamped28(float);void SetClamped30(float);void SetClamped64(float);};
void CUnprovenReverbParameters::SetClamped30(float value){Float30=value<0.0f?0.0f:(value>0.1f?0.1f:value);DirtyFlags|=0x10000;}
static float FromBits(unsigned int bits){float result;memcpy(&result,&bits,4);return result;}
int main(){CUnprovenReverbParameters p;memset(&p,0,sizeof(p));float inputs[]={-2.0f,0.0f,0.0625f,0.25f,0.5f,16.0f,100.0f,128.0f};float low=FromBits(0),high=FromBits(0x3dcccccd);bool ok=true;for(int i=0;i<8;++i){p.DirtyFlags=0x40000000;p.SetClamped30(inputs[i]);float expected=inputs[i];if(expected<low)expected=low;else if(expected>high)expected=high;ok=ok&&p.Float30==expected&&p.DirtyFlags==(0x40000000|0x10000);}p.DirtyFlags=0;p.SetClamped30(p.Float30);ok=ok&&p.DirtyFlags==0x10000;puts(ok?"00c536b0_TEST PASS":"00c536b0_TEST FAIL");return ok?0:1;}
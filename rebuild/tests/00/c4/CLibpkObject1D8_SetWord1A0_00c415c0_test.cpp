#include <stdio.h>
#include <string.h>
// CLibpkObject1D8::SetWord1A0(long value) -- Store a changed parameter and raise its dirty bit.
// Leaf: ?SetWord1A0@CLibpkObject1D8@@QAEXJ@Z
// Identity: Own wrapper00c34229 and call00c4ee75 on libpk source object; neighboring float setters share dirty472. Parameter role unknown. All owner/function names descriptive, no original PDB recovery claimed.
// Layout: no active shared header. Actual vptr0, parameter416, dirty472. Untouched prefix and intervening members not modeled.
class CLibpkObject1D8{public:virtual void UnusedSlot0(){}virtual bool Accepts(){return true;}unsigned char Unmodeled04[412];long Parameter;unsigned char UnmodeledGap[52];unsigned long DirtyFlags;void SetWord1A0(long);};
void CLibpkObject1D8::SetWord1A0(long value){if(Parameter!=value){Parameter=value;DirtyFlags|=0x400000;}}
int main(){CLibpkObject1D8 p;memset(p.Unmodeled04,0x5a,sizeof(p.Unmodeled04));memset(p.UnmodeledGap,0xa5,sizeof(p.UnmodeledGap));p.Parameter=7;p.DirtyFlags=0x20;p.SetWord1A0(7);bool ok=p.Parameter==7&&p.DirtyFlags==0x20;p.SetWord1A0(-3);ok=ok&&p.Parameter==-3&&p.DirtyFlags==(0x20|0x400000);p.DirtyFlags=0;p.SetWord1A0(-3);ok=ok&&!p.DirtyFlags;p.SetWord1A0(0);ok=ok&&p.Parameter==0&&p.DirtyFlags==0x400000;for(unsigned int i=0;i<sizeof(p.Unmodeled04);++i)ok=ok&&p.Unmodeled04[i]==0x5a;for(unsigned int j=0;j<sizeof(p.UnmodeledGap);++j)ok=ok&&p.UnmodeledGap[j]==0xa5;puts(ok?"00c415c0_TEST PASS":"00c415c0_TEST FAIL");return ok?0:1;}
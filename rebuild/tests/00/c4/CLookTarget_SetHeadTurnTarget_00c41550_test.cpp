#include <stdio.h>
#include <string.h>
// CLookTarget::SetHeadTurnTarget(long value) -- Store a changed parameter and raise its dirty bit.
// Leaf: ?SetHeadTurnTarget@CLookTarget@@QAEXJ@Z
// Identity: Own wrapper00c34180 virtual slot1, existing exact callee binding ?SetHeadTurnTarget@CLookTarget@@QAEXJ@Z. Head-turn role is unsupported libpk labeling; spelling retained only for binding consistency. All owner/function names descriptive, no original PDB recovery claimed.
// Layout: no active shared header. Actual vptr0, parameter404, dirty472. Untouched prefix and intervening members not modeled.
struct CLookTarget{public:virtual void UnusedSlot0(){}virtual bool Accepts(){return true;}unsigned char Unmodeled04[400];long Parameter;unsigned char UnmodeledGap[64];unsigned long DirtyFlags;void SetHeadTurnTarget(long);};
void CLookTarget::SetHeadTurnTarget(long value){if(Parameter!=value){Parameter=value;DirtyFlags|=0x100000;}}
int main(){CLookTarget p;memset(p.Unmodeled04,0x5a,sizeof(p.Unmodeled04));memset(p.UnmodeledGap,0xa5,sizeof(p.UnmodeledGap));p.Parameter=7;p.DirtyFlags=0x20;p.SetHeadTurnTarget(7);bool ok=p.Parameter==7&&p.DirtyFlags==0x20;p.SetHeadTurnTarget(-3);ok=ok&&p.Parameter==-3&&p.DirtyFlags==(0x20|0x100000);p.DirtyFlags=0;p.SetHeadTurnTarget(-3);ok=ok&&!p.DirtyFlags;p.SetHeadTurnTarget(0);ok=ok&&p.Parameter==0&&p.DirtyFlags==0x100000;for(unsigned int i=0;i<sizeof(p.Unmodeled04);++i)ok=ok&&p.Unmodeled04[i]==0x5a;for(unsigned int j=0;j<sizeof(p.UnmodeledGap);++j)ok=ok&&p.UnmodeledGap[j]==0xa5;puts(ok?"00c41550_TEST PASS":"00c41550_TEST FAIL");return ok?0:1;}
#include <stdio.h>
#include <string.h>
// CVtable_012B5870::SetWord88(long value) -- Store a changed parameter and raise its dirty bit.
// Leaf: ?SetWord88@CVtable_012B5870@@QAEXJ@Z
// Identity: Own global singleton wrapper00c335f0 loads receiver0143bb58; existing SetSign8C00c3d310 and GetPair8000c3d2d0 prove same table012b5870. All owner/function names descriptive, no original PDB recovery claimed.
// Model amendment: Actual table012b5870 first entry00c3cbb0 is deleting destructor invoking00c3cb30; plain destructor stores012b5870 then destroys four20-byte members and finally stores original base012b5868. Original virtual-object base and virtual destructor slot0 are modeled; inactive opaque field regions remain explicit projections.
// Layout: no active shared header. Actual vptr0, parameter136, dirty164. Untouched prefix and intervening members not modeled.
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
struct CVtable_012B5870:public CUnprovenLibraryObject_012B5868{public:virtual ~CVtable_012B5870();unsigned char Unmodeled04[132];long Parameter;unsigned char UnmodeledGap[24];unsigned long DirtyFlags;void SetWord88(long);};
void CVtable_012B5870::SetWord88(long value){if(Parameter!=value){Parameter=value;DirtyFlags|=0x400000;}}
int main(){CVtable_012B5870 p;memset(p.Unmodeled04,0x5a,sizeof(p.Unmodeled04));memset(p.UnmodeledGap,0xa5,sizeof(p.UnmodeledGap));p.Parameter=7;p.DirtyFlags=0x20;p.SetWord88(7);bool ok=p.Parameter==7&&p.DirtyFlags==0x20;p.SetWord88(-3);ok=ok&&p.Parameter==-3&&p.DirtyFlags==(0x20|0x400000);p.DirtyFlags=0;p.SetWord88(-3);ok=ok&&!p.DirtyFlags;p.SetWord88(0);ok=ok&&p.Parameter==0&&p.DirtyFlags==0x400000;for(unsigned int i=0;i<sizeof(p.Unmodeled04);++i)ok=ok&&p.Unmodeled04[i]==0x5a;for(unsigned int j=0;j<sizeof(p.UnmodeledGap);++j)ok=ok&&p.UnmodeledGap[j]==0xa5;puts(ok?"00c3d2f0_TEST PASS":"00c3d2f0_TEST FAIL");return ok?0:1;}

CUnprovenLibraryObject_012B5868::~CUnprovenLibraryObject_012B5868(){}CVtable_012B5870::~CVtable_012B5870(){}
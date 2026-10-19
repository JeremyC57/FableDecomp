// CAnimFilterOf3Groups::Accepts(const CAnimationEntry*) -- accept an animation in any of three named groups.
// Retail: 0x0073E1D0, 278 bytes; vtable 0x01267480 slot 0.
// Leaf: Accepts
// No shared CAnimFilterOf3Groups, CAnimationEntry, CCharString or CDefString header exists.
// Filter layout agrees with landed Clone 0x0073E2F0: strings +4/+8/+c, CRCs +10/+14/+18.
// Entry group ID +8 agrees with sibling filter 0x0073E940; string data text pointer +0.
// Callee: CDefStringTable::GetStringAsCharArray 0x009D49F0; table 0x013CA828.
#include <string.h>
extern const char g_EmptyString[]; // 0x0129AAF4
struct CCharStringData { const char* Text; };
struct CCharString {
    CCharStringData* PStringData;
    const char* c_str() const { if (!PStringData) return g_EmptyString; return PStringData->Text; }
};
struct CDefString { long TablePos; };
struct CAnimationEntry { long BankIndex; CDefString AnimName; CDefString GroupName; void* Components; };
struct CDefStringTable { const char* GetStringAsCharArray(long); }; // 0x009D49F0
extern CDefStringTable g_DefStringTable; // 0x013CA828
struct CAnimFilterOf3Groups {
    virtual long Accepts(const CAnimationEntry* anim);
    CCharString Group1, Group2, Group3;
    unsigned long Group1CRC, Group2CRC, Group3CRC;
};
long CAnimFilterOf3Groups::Accepts(const CAnimationEntry* anim)
{
    if (!strcmp(g_DefStringTable.GetStringAsCharArray(anim->GroupName.TablePos), Group1.c_str()) ||
        !strcmp(g_DefStringTable.GetStringAsCharArray(anim->GroupName.TablePos), Group2.c_str()) ||
        !strcmp(g_DefStringTable.GetStringAsCharArray(anim->GroupName.TablePos), Group3.c_str()))
        return 1;
    return 0;
}

#include <stdio.h>
const char g_EmptyString[] = "";
CDefStringTable g_DefStringTable;
static const char* fixtureName;
static long fixtureId;
static int calls, badCall;
const char* CDefStringTable::GetStringAsCharArray(long id) {
    ++calls;
    if (this != &g_DefStringTable || id != fixtureId) ++badCall;
    return fixtureName;
}
static int run(CAnimFilterOf3Groups& f, CAnimationEntry& a, const char* text, long result, int count) {
    fixtureName=text; fixtureId=a.GroupName.TablePos; calls=badCall=0;
    long got=f.Accepts(&a);
    return got==result && calls==count && badCall==0;
}
int main() {
    CCharStringData d1={"alpha"}, d2={"bravo"}, d3={"charlie"}, empty={""};
    CAnimFilterOf3Groups f;
    f.Group1.PStringData=&d1; f.Group2.PStringData=&d2; f.Group3.PStringData=&d3;
    f.Group1CRC=1; f.Group2CRC=2; f.Group3CRC=3;
    CAnimationEntry a; a.BankIndex=49; a.AnimName.TablePos=999; a.GroupName.TablePos=73; a.Components=0;
    int ok=run(f,a,"alpha",1,1) && run(f,a,"bravo",1,2) && run(f,a,"charlie",1,3)
        && run(f,a,"delta",0,3) && run(f,a,"Alpha",0,3) && run(f,a,"alph",0,3)
        && run(f,a,"alpha-long",0,3) && run(f,a,"",0,3);
    f.Group1.PStringData=0;
    ok=ok && run(f,a,"",1,1) && run(f,a,"bravo",1,2);
    f.Group1.PStringData=&d1; f.Group2.PStringData=0;
    ok=ok && run(f,a,"",1,2) && run(f,a,"charlie",1,3);
    f.Group2.PStringData=&d2; f.Group3.PStringData=0;
    a.GroupName.TablePos=-1;
    ok=ok && run(f,a,"",1,3) && run(f,a,"other",0,3);
    f.Group1.PStringData=&empty;
    ok=ok && run(f,a,"",1,1);
    ok=ok && f.Group1CRC==1 && f.Group2CRC==2 && f.Group3CRC==3
        && a.BankIndex==49 && a.AnimName.TablePos==999 && a.GroupName.TablePos==-1;
    printf("0073e1d0_TEST %s\n",ok?"PASS":"FAIL"); return ok?0:1;
}
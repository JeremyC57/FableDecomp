#include <cstdio>
struct CCharString { void* PStringData; CCharString() : PStringData(0) {} };
static int g_sets = 0;
struct CAnimFilterOf2Groups {
    virtual long Accepts(const void*) { return 0; }
    virtual CAnimFilterOf2Groups* Clone();
    CCharString Group1; CCharString Group2; unsigned long Group1CRC; unsigned long Group2CRC;
    CAnimFilterOf2Groups(const CCharString& a, const CCharString& b) { SetGroups(a, b); }
    void SetGroups(const CCharString& a, const CCharString& b) { Group1.PStringData = a.PStringData; Group2.PStringData = b.PStringData; Group1CRC = 1; Group2CRC = 2; ++g_sets; }
};
CAnimFilterOf2Groups* CAnimFilterOf2Groups::Clone() { return new CAnimFilterOf2Groups(Group1, Group2); }
int main()
{
    CCharString a, b; a.PStringData = (void*)0x10; b.PStringData = (void*)0x20;
    CAnimFilterOf2Groups f(a, b);
    CAnimFilterOf2Groups* c = f.Clone();
    if (!c || c == &f || g_sets != 2 || c->Group1.PStringData != (void*)0x10 || c->Group2.PStringData != (void*)0x20 || c->Group2CRC != 2) { std::printf("0073ea70_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("0073ea70_TEST PASS\n");
    return 0;
}
#include <cstdio>
struct CCharString { void* PStringData; };
struct CAnimFilterOfGroup {
    virtual long Accepts(const void*) { return 0; }
    virtual CAnimFilterOfGroup* Clone();
    CCharString Group; unsigned long GroupCRC;
    CAnimFilterOfGroup(const CCharString& g) : Group(g), GroupCRC(9) {}
};
CAnimFilterOfGroup* CAnimFilterOfGroup::Clone() { return new CAnimFilterOfGroup(Group); }
int main()
{
    CCharString a = { (void*)5 };
    CAnimFilterOfGroup f(a);
    CAnimFilterOfGroup* k = f.Clone();
    if (!k || k == &f || k->Group.PStringData != (void*)5 || k->GroupCRC != 9) { std::printf("0073e9a0_TEST FAIL\n"); return 1; }
    delete k;
    std::printf("0073e9a0_TEST PASS\n");
    return 0;
}
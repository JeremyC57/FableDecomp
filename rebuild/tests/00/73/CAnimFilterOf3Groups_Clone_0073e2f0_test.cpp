#include <cstdio>
class CCharString { public: void* PStringData; };
struct CAnimFilterOf3Groups {
    virtual long Accepts(const void*) { return 0; }
    virtual CAnimFilterOf3Groups* Clone();
    CCharString Group1, Group2, Group3; unsigned long Group1CRC, Group2CRC, Group3CRC;
    CAnimFilterOf3Groups(const CCharString& a, const CCharString& b, const CCharString& c) : Group1(a), Group2(b), Group3(c), Group1CRC(1), Group2CRC(2), Group3CRC(3) {}
};
CAnimFilterOf3Groups* CAnimFilterOf3Groups::Clone() { return new CAnimFilterOf3Groups(Group1, Group2, Group3); }
int main()
{
    CCharString a = { (void*)1 }, b = { (void*)2 }, c = { (void*)3 };
    CAnimFilterOf3Groups f(a, b, c);
    CAnimFilterOf3Groups* k = f.Clone();
    if (!k || k == &f || k->Group3.PStringData != (void*)3 || k->Group1CRC != 1) { std::printf("0073e2f0_TEST FAIL\n"); return 1; }
    delete k;
    std::printf("0073e2f0_TEST PASS\n");
    return 0;
}
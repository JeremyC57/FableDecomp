#include <cstdio>
struct CDefStringTable { unsigned long GetStringCRC(long i) { return (unsigned long)i * 7u; } };
CDefStringTable g_defStringTable;
struct CCharString { void* PStringData; };
struct CAnimInfo { unsigned char _pad[8]; long GroupName; };
struct CAnimFilterOf2Groups {
    virtual long Accepts(const CAnimInfo& anim);
    CCharString Group1; CCharString Group2; unsigned long Group1CRC; unsigned long Group2CRC;
};
long CAnimFilterOf2Groups::Accepts(const CAnimInfo& anim)
{
    if (g_defStringTable.GetStringCRC(anim.GroupName) == Group1CRC ||
        g_defStringTable.GetStringCRC(anim.GroupName) == Group2CRC)
        return 1;
    return 0;
}
int main()
{
    CAnimFilterOf2Groups f; f.Group1CRC = 21; f.Group2CRC = 70;
    CAnimInfo a; a.GroupName = 3;
    if (f.Accepts(a) != 1) { std::printf("0073ea00_TEST FAIL\n"); return 1; }
    a.GroupName = 10;
    if (f.Accepts(a) != 1) { std::printf("0073ea00_TEST FAIL\n"); return 1; }
    a.GroupName = 5;
    if (f.Accepts(a) != 0) { std::printf("0073ea00_TEST FAIL\n"); return 1; }
    std::printf("0073ea00_TEST PASS\n");
    return 0;
}
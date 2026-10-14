#include <cstdio>
struct CDefStringTable { unsigned long GetStringCRC(long i) { return (unsigned long)i * 7u; } };
CDefStringTable g_defStringTable;
struct CCharString { void* PStringData; };
struct CAnimInfo { unsigned char _pad[8]; long GroupName; };
struct CAnimFilterOfGroup { virtual long Accepts(const CAnimInfo& anim); CCharString Group; unsigned long GroupCRC; };
long CAnimFilterOfGroup::Accepts(const CAnimInfo& anim) { return g_defStringTable.GetStringCRC(anim.GroupName) == GroupCRC; }
int main()
{
    CAnimFilterOfGroup f; f.GroupCRC = 21;
    CAnimInfo a; a.GroupName = 3;
    if (f.Accepts(a) != 1) { std::printf("0073e940_TEST FAIL\n"); return 1; }
    a.GroupName = 4;
    if (f.Accepts(a) != 0) { std::printf("0073e940_TEST FAIL\n"); return 1; }
    std::printf("0073e940_TEST PASS\n");
    return 0;
}
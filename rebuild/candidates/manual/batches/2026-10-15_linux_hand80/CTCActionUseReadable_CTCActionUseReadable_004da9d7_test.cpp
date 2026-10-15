#include <cstdio>
#include <cstring>

struct CThing { int id; };
static int g_baseCtors = 0, g_stringCtors = 0;
struct CWideString { void* m_pData; CWideString() : m_pData(0) { ++g_stringCtors; } };
struct CDefPointer { void* m_pDef; CDefPointer() : m_pDef(0) { } };
struct CTCActionUseBase { CThing* ParentThing; bool Usable; CTCActionUseBase(CThing& owner) : ParentThing(&owner), Usable(true) { ++g_baseCtors; } };
struct CTCActionUseReadable : CTCActionUseBase {
    CDefPointer PDef; CWideString TextTag;
    CTCActionUseReadable(CThing& owner) : CTCActionUseBase(owner) { }
};
int main()
{
    CThing thing; thing.id = 2;
    CTCActionUseReadable r(thing);
    if (r.ParentThing != &thing || r.PDef.m_pDef != 0 || r.TextTag.m_pData != 0 || g_baseCtors != 1 || g_stringCtors != 1) { std::printf("004da9d7_TEST FAIL\n"); return 1; }
    std::printf("004da9d7_TEST PASS\n");
    return 0;
}

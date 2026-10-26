#include <cstdio>
#include <cstring>

class CThing { public: int id; };
static int g_baseCtors = 0, g_stringCtors = 0;
class CWideString { public: void* m_pData; CWideString() : m_pData(0) { ++g_stringCtors; } };
struct CDefPointer { void* m_pDef; CDefPointer() : m_pDef(0) { } };
class CTCActionUseBase { public: CThing* ParentThing; bool Usable; CTCActionUseBase(CThing& owner) : ParentThing(&owner), Usable(true) { ++g_baseCtors; } };
class CTCActionUseReadable : public CTCActionUseBase { public:
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
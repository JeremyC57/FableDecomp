// Behaviour test for the implicit CHeroSoulsCreatureDef::~CHeroSoulsCreatureDef 0x00F270D0: Location, then CreatureType, then the
// CDefClassBase / CDefPointeeBase / CResource chain
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CCharString {
public:
    CCharString() : Id(0) {}
    ~CCharString() { g_order[g_count++] = Id; }
    int Id;
};
class CResource {
public:
    virtual ~CResource() { g_order[g_count++] = 9; }
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefClassBase : public CDefPointeeBase {
public:
    virtual ~CDefClassBase() {}
    void* PDefManager;
    unsigned long GlobalIndex;
};
class CHeroSoulsCreatureDef : public CDefClassBase {
public:
    CCharString CreatureType;
    CCharString Location;
    long        NumCreatures;
};
int main()
{
    {
        CHeroSoulsCreatureDef def;
        def.CreatureType.Id = 1;
        def.Location.Id = 2;
        def.NumCreatures = 4;
    }
    if (g_count != 3 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 9) { std::printf("00f270d0_TEST FAIL\n"); return 1; }
    CResource* p = new CHeroSoulsCreatureDef;
    delete p;
    if (g_count != 6 || g_order[5] != 9) { std::printf("00f270d0_TEST FAIL\n"); return 1; }
    std::printf("00f270d0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
struct CCharString { void* PStringData; CCharString& operator=(const CCharString& o) { PStringData = o.PStringData; return *this; } };
static int g_base = 0;
struct CDefBase { unsigned char _body[0x28]; CDefBase& Copy(const CDefBase& o) { std::memcpy(_body, o._body, 0x28); ++g_base; return *this; } };
struct CArenaCreatureDef : CDefBase {
    CCharString CreatureType; long NumCreatures; CCharString HUDType; long DeathScore;
    CArenaCreatureDef& Copy(const CArenaCreatureDef& o) { CDefBase::Copy(o); CreatureType = o.CreatureType; NumCreatures = o.NumCreatures; HUDType = o.HUDType; DeathScore = o.DeathScore; return *this; }
};
int main()
{
    CArenaCreatureDef a, b; std::memset(&a, 0, sizeof a); std::memset(&b, 0x11, sizeof b);
    b.CreatureType.PStringData = (void*)1; b.NumCreatures = 5; b.HUDType.PStringData = (void*)2; b.DeathScore = 9;
    CArenaCreatureDef& r = a.Copy(b);
    if (&r != &a || g_base != 1 || a._body[3] != 0x11 || a.CreatureType.PStringData != (void*)1 || a.NumCreatures != 5 || a.HUDType.PStringData != (void*)2 || a.DeathScore != 9) { std::printf("00f25f10_TEST FAIL\n"); return 1; }
    std::printf("00f25f10_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; };
static CThingCreatureBase* g_owner; static long g_a, g_b, g_s0, g_s1;
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };

class CCreatureActionBase { public: virtual void v00() {} unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long a, long b) { g_owner = owner; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; g_a = a; g_b = b; std::memset(_body, 0, sizeof(_body)); } };
class CCreatureAction_SpiritReturnToHero : public CCreatureActionBase { public: virtual void v00() {} CCreatureAction_SpiritReturnToHero(CThingCreatureBase* owner); };
CCreatureAction_SpiritReturnToHero::CCreatureAction_SpiritReturnToHero(CThingCreatureBase* owner) : CCreatureActionBase(owner, CSoundPair(), 0, 0) { }
int main()
{
    CThingCreatureBase owner; CCreatureAction_SpiritReturnToHero x(&owner);
    if (g_owner != &owner || g_a != 0 || g_b != 0 || g_s0 != 0 || g_s1 != 0 || sizeof(x) != 0xa8) { std::printf("00854660_TEST FAIL\n"); return 1; }
    std::printf("00854660_TEST PASS\n");
    return 0;
}
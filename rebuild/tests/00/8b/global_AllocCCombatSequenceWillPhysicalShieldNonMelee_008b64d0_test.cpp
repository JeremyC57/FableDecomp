#include <cstdio>
#include <cstdlib>
#include <cstring>
class CThingCreatureBase { public: int x; }; static CThingCreatureBase* g_first = 0; static CThingCreatureBase* g_second = 0;
class CCombatSequenceBase { public: virtual void v00() { } unsigned char _body[0x28 - 4]; CCombatSequenceBase(CThingCreatureBase* f, CThingCreatureBase* s) { g_first = f; g_second = s; } };
struct CCombatSequence_WillPhysicalShieldNonMelee : CCombatSequenceBase { virtual void v00() { } CCombatSequence_WillPhysicalShieldNonMelee(CThingCreatureBase* f, CThingCreatureBase* s) : CCombatSequenceBase(f, s) { } };
static CCombatSequence_WillPhysicalShieldNonMelee* Alloc_CCombatSequence_WillPhysicalShieldNonMelee(CThingCreatureBase* f, CThingCreatureBase* s) { return new CCombatSequence_WillPhysicalShieldNonMelee(f, s); }
int main() {
    CThingCreatureBase a, b; CCombatSequence_WillPhysicalShieldNonMelee* s = Alloc_CCombatSequence_WillPhysicalShieldNonMelee(&a, &b); if (!s || g_first != &a || g_second != &b) { std::printf("008b64d0_TEST FAIL\n"); return 1; } delete s;
    std::printf("008b64d0_TEST PASS\n"); return 0;
}
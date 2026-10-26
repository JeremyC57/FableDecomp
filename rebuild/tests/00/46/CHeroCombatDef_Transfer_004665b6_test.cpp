// behaviour test for CHeroCombatDef::Transfer (0x004665B6): 44 typed transfers; HeavyWeapons goes between the two vectors
#include <cstdio>
class CFloatRange { public: float Min, Max; };
class CHeroAbilityStatWeight { public: float W[3]; };
class CHeroAbilityStatPackage { public: unsigned char b[0x10]; };
struct CLongVector { long* a[3]; };
struct CFloatVector { float* a[3]; };
static const void* g_log[64]; static int g_n = 0;
class CPersistContext { public:
    void Transfer(CFloatRange& v) { g_log[g_n++] = &v; }
    void Transfer(CHeroAbilityStatWeight& v) { g_log[g_n++] = &v; }
    void Transfer(float& v) { g_log[g_n++] = &v; }
    void Transfer(bool& v) { g_log[g_n++] = &v; }
    void Transfer(long& v) { g_log[g_n++] = &v; }
    void Transfer(CLongVector& v) { g_log[g_n++] = &v; }
    void Transfer(CHeroAbilityStatPackage& v) { g_log[g_n++] = &v; }
    void Transfer(CFloatVector& v) { g_log[g_n++] = &v; }
};
class CHeroCombatDef { public:
    CFloatRange r0, r1, r2; CHeroAbilityStatWeight w3; CFloatRange r4; CHeroAbilityStatWeight w5; float f6; CHeroAbilityStatWeight w7;
    float f8, f9, f10, f11, f12, f13; CHeroAbilityStatWeight w14; CFloatRange r15; CHeroAbilityStatWeight w16; CFloatRange r17;
    CHeroAbilityStatWeight w18; bool b19; long l20; bool b21; float f22; bool b23, b24, b25; float f26, f27; long l28;
    CFloatRange r29; CHeroAbilityStatWeight w30; float f31, f32, f33, f34, f35, f36; CLongVector DefaultCombatAbilities;
    CFloatVector FOV; bool b39, b40, b41; float f42; CHeroAbilityStatPackage HeavyWeapons;
    void Transfer(CPersistContext& c)
    {
        c.Transfer(r0); c.Transfer(r1); c.Transfer(r2); c.Transfer(w3); c.Transfer(r4); c.Transfer(w5); c.Transfer(f6); c.Transfer(w7);
        c.Transfer(f8); c.Transfer(f9); c.Transfer(f10); c.Transfer(f11); c.Transfer(f12); c.Transfer(f13); c.Transfer(w14); c.Transfer(r15);
        c.Transfer(w16); c.Transfer(r17); c.Transfer(w18); c.Transfer(b19); c.Transfer(l20); c.Transfer(b21); c.Transfer(f22); c.Transfer(b23);
        c.Transfer(b24); c.Transfer(b25); c.Transfer(f26); c.Transfer(f27); c.Transfer(l28); c.Transfer(r29); c.Transfer(w30); c.Transfer(f31);
        c.Transfer(f32); c.Transfer(f33); c.Transfer(f34); c.Transfer(f35); c.Transfer(f36); c.Transfer(DefaultCombatAbilities);
        c.Transfer(HeavyWeapons); c.Transfer(FOV); c.Transfer(b39); c.Transfer(b40); c.Transfer(b41); c.Transfer(f42);
    }
};
int main()
{
    static CHeroCombatDef d; CPersistContext c; d.Transfer(c);
    if (g_n != 44) { std::printf("004665b6_TEST FAIL\n"); return 1; }
    if (g_log[37] != &d.DefaultCombatAbilities || g_log[38] != &d.HeavyWeapons || g_log[39] != &d.FOV || g_log[43] != &d.f42) { std::printf("004665b6_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 38; ++i)
        if ((const char*)g_log[i] <= (const char*)g_log[i - 1]) { std::printf("004665b6_TEST FAIL\n"); return 1; }
    std::printf("004665b6_TEST PASS\n");
    return 0;
}
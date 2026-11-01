// Behaviour test for CWillResponseDef::Transfer (0x004DF562): the eight flags go to Transfer<bool> in declaration order.
#include <cstdio>
static const bool* g_seen[16];
static int g_count = 0;
class CPersistContext {
public:
    template <class T> void Transfer(T& v) { g_seen[g_count++] = &v; v = !v; }
};
class CWillResponseDef {
public:
    bool IsForcePushable, IsLifeDrainable, IsLightningable, IsNotBloodyWillable;
    bool IsAffectedBySlowTime, IsAbleToBeLifted, IsEpicPowerExplodable, IsEpicSpellable;
    void Transfer(CPersistContext& context)
    {
        context.Transfer(IsForcePushable);
        context.Transfer(IsLifeDrainable);
        context.Transfer(IsLightningable);
        context.Transfer(IsNotBloodyWillable);
        context.Transfer(IsAffectedBySlowTime);
        context.Transfer(IsAbleToBeLifted);
        context.Transfer(IsEpicPowerExplodable);
        context.Transfer(IsEpicSpellable);
    }
};
int main()
{
    CWillResponseDef def;
    def.IsForcePushable = def.IsLifeDrainable = def.IsLightningable = def.IsNotBloodyWillable = false;
    def.IsAffectedBySlowTime = def.IsAbleToBeLifted = def.IsEpicPowerExplodable = def.IsEpicSpellable = false;
    CPersistContext ctx;
    def.Transfer(ctx);
    const bool* want[8] = { &def.IsForcePushable, &def.IsLifeDrainable, &def.IsLightningable, &def.IsNotBloodyWillable,
                            &def.IsAffectedBySlowTime, &def.IsAbleToBeLifted, &def.IsEpicPowerExplodable, &def.IsEpicSpellable };
    if (g_count != 8) { std::printf("004df562_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        if (g_seen[i] != want[i] || !*want[i]) { std::printf("004df562_TEST FAIL\n"); return 1; }
    std::printf("004df562_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>

static int g_baseSerialises = 0; static bool g_lastLoading = false;
static const char* g_names[2]; static bool* g_values[2]; static bool g_defaults[2]; static int g_count = 0;
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& def) { g_names[g_count] = name; g_values[g_count] = &value; g_defaults[g_count] = def; ++g_count; }
};
struct CTCActivationReceptorBase { void OnSerialise(CPersistContext&, bool loading) { ++g_baseSerialises; g_lastLoading = loading; } };
struct CTCActivationReceptorCreatureGenerator : CTCActivationReceptorBase {
    bool TriggerOnActivate, ActivateOnActivate;
    void OnSerialise(CPersistContext& context, bool loading)
    {
        CTCActivationReceptorBase::OnSerialise(context, loading);
        context.Transfer("ActivateOnActivate", ActivateOnActivate, false);
        context.Transfer("TriggerOnActivate", TriggerOnActivate, false);
    }
};
int main()
{
    CTCActivationReceptorCreatureGenerator g; g.TriggerOnActivate = true; g.ActivateOnActivate = false;
    CPersistContext ctx; g.OnSerialise(ctx, true);
    if (g_baseSerialises != 1 || !g_lastLoading || g_count != 2) { std::printf("007f7600_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_names[0], "ActivateOnActivate") != 0 || g_values[0] != &g.ActivateOnActivate || g_defaults[0]) { std::printf("007f7600_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_names[1], "TriggerOnActivate") != 0 || g_values[1] != &g.TriggerOnActivate || g_defaults[1]) { std::printf("007f7600_TEST FAIL\n"); return 1; }
    std::printf("007f7600_TEST PASS\n");
    return 0;
}
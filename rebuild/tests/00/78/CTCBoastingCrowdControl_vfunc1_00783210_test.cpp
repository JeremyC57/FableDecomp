// Behaviour test for 0x00783210 CTCBoastingCrowdControl::vfunc_1: when asked, CrowdSizesSuppressed is
// transferred under its own name with a false default; otherwise nothing is transferred.
#include <cstdio>
#include <cstring>

static int g_transfers;
static const char* g_name;
static bool* g_value;
static bool g_default;
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue) { ++g_transfers; g_name = name; g_value = &value; g_default = defaultValue; }
};
struct CTCBoastingCrowdControl {
    bool CrowdSizesSuppressed;
    void vfunc_1(CPersistContext& context, bool transfer)
    {
        if (transfer)
            context.Transfer("CrowdSizesSuppressed", CrowdSizesSuppressed, false);
    }
};

int main()
{
    CPersistContext context;
    CTCBoastingCrowdControl c; c.CrowdSizesSuppressed = true;
    g_transfers = 0;
    c.vfunc_1(context, false);
    if (g_transfers != 0) { std::printf("00783210_TEST FAIL\n"); return 1; }
    g_default = true;
    c.vfunc_1(context, true);
    if (g_transfers != 1 || std::strcmp(g_name, "CrowdSizesSuppressed") != 0 || g_value != &c.CrowdSizesSuppressed || g_default) { std::printf("00783210_TEST FAIL\n"); return 1; }
    std::printf("00783210_TEST PASS\n");
    return 0;
}
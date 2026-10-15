#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
static const char* g_name = 0;
static bool* g_value = 0;
static bool g_default = false;
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue) { g_name = name; g_value = &value; g_default = defaultValue; note("T"); }
};
static bool g_baseFlag = true;
struct CTCContainer_Calls {
    virtual void v00() { }
    virtual void Persist(CPersistContext* context, bool flag) { g_baseFlag = flag; note("C"); }
    unsigned char _pad04[0x14];
};
struct CTCContainerRewardHero : CTCContainer_Calls {
    bool Enabled; bool CreateObjectsThatTimeOut;
    virtual void Persist(CPersistContext* context, bool flag)
    {
        if (flag)
            context->Transfer("RewardEnabled", Enabled, true);
        CTCContainer_Calls::Persist(context, flag);
    }
};
int main()
{
    CTCContainerRewardHero r; CPersistContext ctx;
    CTCContainer_Calls* base = &r;
    base->Persist(&ctx, false);
    if (std::strcmp(g_seq, "C") || g_baseFlag || g_name != 0) { std::printf("0074e240_TEST FAIL\n"); return 1; }
    base->Persist(&ctx, true);
    if (std::strcmp(g_seq, "CTC") || !g_baseFlag || g_name == 0 || std::strcmp(g_name, "RewardEnabled") || g_value != &r.Enabled || !g_default)
    { std::printf("0074e240_TEST FAIL\n"); return 1; }
    std::printf("0074e240_TEST PASS\n");
    return 0;
}
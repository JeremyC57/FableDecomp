#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
static const char* g_name = 0;
static bool* g_value = 0;
static bool g_default = true;
class CPersistContext { public:
    void Transfer(const char* name, bool& value, const bool& defaultValue) { g_name = name; g_value = &value; g_default = defaultValue; note("T"); }
};
static bool g_baseFlag = false;
struct CTCContainer_Calls {
    virtual void v00() { }
    virtual void Persist(CPersistContext* context, bool flag) { g_baseFlag = flag; note("C"); }
    unsigned char _pad04[0x14];
};
struct CTCContainerRewardHero_Calls : CTCContainer_Calls { bool Enabled; bool CreateObjectsThatTimeOut; unsigned char _pad1a[2]; };
class CTCChest : public CTCContainerRewardHero_Calls { public:
    unsigned char ChestOpener[0x14]; bool ChestOpen;
    virtual void Persist(CPersistContext* context, bool flag)
    {
        CTCContainer_Calls::Persist(context, flag);
        context->Transfer("ChestOpen", ChestOpen, false);
    }
};
int main()
{
    CTCChest chest; chest.ChestOpen = true;
    CPersistContext ctx;
    CTCContainer_Calls* base = &chest;
    base->Persist(&ctx, true);
    if (std::strcmp(g_seq, "CT") || !g_baseFlag || g_name == 0 || std::strcmp(g_name, "ChestOpen") || g_value != &chest.ChestOpen || g_default)
    { std::printf("00750010_TEST FAIL\n"); return 1; }
    std::printf("00750010_TEST PASS\n");
    return 0;
}
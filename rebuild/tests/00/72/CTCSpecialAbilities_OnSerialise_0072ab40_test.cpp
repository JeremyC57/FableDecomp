// Behaviour test for CTCSpecialAbilities::OnSerialise (0x0072AB40): the static def number is transferred under its key
// with a zero default.
#include <cstdio>
#include <cstring>

static const char* g_name = 0;
static long* g_value = 0;
static long g_default = -5;

struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue) { g_name = name; g_value = &value; g_default = defaultValue; value = 42; }
};

struct CTCSpecialAbilities {
    static long CurrentCreatureGlobalDefNum;
    virtual ~CTCSpecialAbilities() { }
    virtual void OnSerialise(CPersistContext* context, long unused);
};

long CTCSpecialAbilities::CurrentCreatureGlobalDefNum = -1;

void CTCSpecialAbilities::OnSerialise(CPersistContext* context, long)
{
    long defaultValue = 0;
    context->Transfer("CurrentCreatureGlobalDefNum", CurrentCreatureGlobalDefNum, defaultValue);
}

int main()
{
    CPersistContext context;
    CTCSpecialAbilities s;
    s.OnSerialise(&context, 7);
    if (!g_name || std::strcmp(g_name, "CurrentCreatureGlobalDefNum") != 0 || g_value != &CTCSpecialAbilities::CurrentCreatureGlobalDefNum
        || g_default != 0 || CTCSpecialAbilities::CurrentCreatureGlobalDefNum != 42) {
        std::printf("0072ab40_TEST FAIL\n"); return 1;
    }
    std::printf("0072ab40_TEST PASS\n");
    return 0;
}
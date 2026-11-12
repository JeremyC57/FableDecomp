#include <cstdio>
#include <cstring>

static const char* g_name = 0; static long g_default = -1; static int g_derived = 0;

class CPersistContext { public:
    long m_loaded;
    void Transfer(const char* name, long& value, const long& defaultValue) { g_name = name; g_default = defaultValue; value = m_loaded; }
};

class CTCGatherPointBase { public:
    long MaxOccupation;
    virtual ~CTCGatherPointBase() { }
    virtual void TransferDerived(CPersistContext*) { ++g_derived; }
    void vfunc_1(CPersistContext* context, long)
    {
        long defaultValue = 0;
        context->Transfer("MaxOccupation", MaxOccupation, defaultValue);
        TransferDerived(context);
    }
};

int main()
{
    CPersistContext ctx; ctx.m_loaded = 5;
    CTCGatherPointBase g; g.MaxOccupation = 1;
    g.vfunc_1(&ctx, 99);
    if (g.MaxOccupation != 5 || g_default != 0 || !g_name || std::strcmp(g_name, "MaxOccupation") != 0 || g_derived != 1)
        { std::printf("007ffe40_TEST FAIL\n"); return 1; }
    std::printf("007ffe40_TEST PASS\n");
    return 0;
}
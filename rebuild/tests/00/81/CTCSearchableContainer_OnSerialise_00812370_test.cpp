#include <cstdio>
#include <cstring>
class CPersistContext { public:
    const char* m_lastName; long m_loadValue; bool m_loading; int m_baseCalls;
    void Transfer(const char* name, long& value, const long& defaultValue)
    {
        m_lastName = name;
        if (m_loading) value = m_loadValue; else if (value == defaultValue) m_lastName = "default";
    }
};
class CTCContainerRewardHero { public:
    virtual ~CTCContainerRewardHero() { }
    virtual void OnSerialise(CPersistContext* context, bool loading) { ++context->m_baseCalls; }
};
class CTCSearchableContainer : public CTCContainerRewardHero { public:
    long NumberOfSearchesRemaining;
    virtual void OnSerialise(CPersistContext* context, bool loading)
    {
        CTCContainerRewardHero::OnSerialise(context, loading);
        long defaultValue = 0;
        context->Transfer("NumberOfTimesToSearch", NumberOfSearchesRemaining, defaultValue);
    }
};
int main()
{
    CPersistContext ctx; ctx.m_loading = true; ctx.m_loadValue = 4; ctx.m_baseCalls = 0; ctx.m_lastName = 0;
    CTCSearchableContainer c; c.NumberOfSearchesRemaining = 0;
    CTCContainerRewardHero* base = &c;
    base->OnSerialise(&ctx, true);
    if (ctx.m_baseCalls != 1 || c.NumberOfSearchesRemaining != 4 || std::strcmp(ctx.m_lastName, "NumberOfTimesToSearch") != 0) { std::printf("00812370_TEST FAIL\n"); return 1; }
    std::printf("00812370_TEST PASS\n");
    return 0;
}
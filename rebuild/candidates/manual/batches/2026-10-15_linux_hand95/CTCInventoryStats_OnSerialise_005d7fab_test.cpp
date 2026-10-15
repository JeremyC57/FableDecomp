#include <cstdio>
#include <cstring>
// Behaviour of CTCInventoryStats::OnSerialise (0x005D7FAB): base first, then "CreaturesKilled2" -> CreaturesKilled.
struct CCreatureKillMap { long m_entries; };
struct CPersistContext {
    long m_calls; const char* m_lastName; void* m_lastValue; bool m_baseSeenFirst;
    template <class T> void Transfer(const char* name, T* value) { ++m_calls; m_lastName = name; m_lastValue = value; }
};
struct CTCInventoryBase_Retail {
    long m_baseCalls; bool m_lastLoading;
    void OnSerialise(CPersistContext& context, bool loading) { ++m_baseCalls; m_lastLoading = loading; context.m_baseSeenFirst = (context.m_calls == 0); }
};
struct CBase { virtual long vfunc_0() { return 0; } };
struct CTCInventoryStats : CTCInventoryBase_Retail, CBase {
    CCreatureKillMap CreaturesKilled;
    void OnSerialise(CPersistContext& context, bool loading)
    {
        CTCInventoryBase_Retail::OnSerialise(context, loading);
        context.Transfer("CreaturesKilled2", &CreaturesKilled);
    }
};
int main()
{
    CPersistContext ctx; ctx.m_calls = 0; ctx.m_lastName = 0; ctx.m_lastValue = 0; ctx.m_baseSeenFirst = false;
    CTCInventoryStats s; s.m_baseCalls = 0; s.m_lastLoading = false;
    s.OnSerialise(ctx, true);
    if (s.m_baseCalls != 1 || !s.m_lastLoading || !ctx.m_baseSeenFirst || ctx.m_calls != 1 || ctx.m_lastValue != &s.CreaturesKilled
        || std::strcmp(ctx.m_lastName, "CreaturesKilled2") != 0) { std::printf("005d7fab_TEST FAIL\n"); return 1; }
    std::printf("005d7fab_TEST PASS\n");
    return 0;
}

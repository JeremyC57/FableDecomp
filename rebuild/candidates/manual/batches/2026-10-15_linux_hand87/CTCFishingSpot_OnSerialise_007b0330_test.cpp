#include <cstdio>
#include <cstring>
struct CPersistContext {
    const char* m_name; bool m_loaded; int m_calls; bool m_default;
    void Transfer(const char* name, bool& value, const bool& defaultValue) { m_name = name; m_default = defaultValue; value = m_loaded; ++m_calls; }
};
struct CTCFishingSpot {
    void* vptrSlot; void* ParentThing; bool Enabled;
    void OnSerialise(CPersistContext& context, bool transfer) { if (transfer) context.Transfer("Enabled", Enabled, true); }
};
int main()
{
    CPersistContext c; c.m_name = 0; c.m_loaded = false; c.m_calls = 0; c.m_default = false;
    CTCFishingSpot s; s.Enabled = true;
    s.OnSerialise(c, false); bool skipped = c.m_calls == 0 && s.Enabled;
    s.OnSerialise(c, true);
    if (!skipped || c.m_calls != 1 || std::strcmp(c.m_name, "Enabled") != 0 || !c.m_default || s.Enabled) { std::printf("007b0330_TEST FAIL\n"); return 1; }
    std::printf("007b0330_TEST PASS\n");
    return 0;
}

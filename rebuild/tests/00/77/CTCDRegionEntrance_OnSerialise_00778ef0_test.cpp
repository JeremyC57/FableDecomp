// Behaviour test for CTCDRegionEntrance::OnSerialise(CPersistContext&, bool) @ 0x00778EF0: one "Active" transfer with a
// true default, for both transfer kinds.
#include <cstdio>
#include <cstring>
static int g_transfers = 0;
static const char* g_name = 0;
static bool g_default = false;
static bool* g_value = 0;
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue)
    { ++g_transfers; g_name = name; g_default = defaultValue; g_value = &value; }
};
struct CTCDRegionEntrance {
    virtual void v00() { }
    unsigned char _base[0x8];
    bool          Active;
    void OnSerialise(CPersistContext& context, bool fullState);
};
void CTCDRegionEntrance::OnSerialise(CPersistContext& context, bool fullState)
{
    context.Transfer("Active", Active, true);
}
int main()
{
    CTCDRegionEntrance r; CPersistContext c;
    r.OnSerialise(c, false);
    if (g_transfers != 1 || std::strcmp(g_name, "Active") != 0 || g_default != true || g_value != &r.Active) { std::printf("00778ef0_TEST FAIL\n"); return 1; }
    r.OnSerialise(c, true);
    if (g_transfers != 2) { std::printf("00778ef0_TEST FAIL\n"); return 1; }
    std::printf("00778ef0_TEST PASS\n");
    return 0;
}
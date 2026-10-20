// Behaviour test for CTCInfoDisplay::OnSerialise (0x007E1B10): four transfers in order with the right names,
// members and defaults (empty strings, 0.0f), every string temporary destroyed.
#include <cstdio>
#include <cstring>

static int g_live = 0;
struct CCharString {
    const char* Text;
    CCharString() : Text("") { ++g_live; }
    CCharString(const CCharString& other) : Text(other.Text) { ++g_live; }
    ~CCharString() { --g_live; }
};
inline CCharString EmptyString() { CCharString empty; return empty; }

static const char* g_names[8]; static const void* g_members[8]; static int g_count = 0; static bool g_defaultsOk = true;
struct CPersistContext {
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue)
    { g_names[g_count] = name; g_members[g_count++] = &value; if (std::strcmp(defaultValue.Text, "") != 0) g_defaultsOk = false; value.Text = "loaded"; }
    void Transfer(const char* name, float& value, const float& defaultValue)
    { g_names[g_count] = name; g_members[g_count++] = &value; if (defaultValue != 0.0f) g_defaultsOk = false; value = 2.5f; }
};
struct CTCInfoDisplay {
    bool Forward, Backward;
    CCharString TextTag, TextTagBack;
    float Radius, DisplayTime;
    void OnSerialise(CPersistContext& context, bool);
};
void CTCInfoDisplay::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("TextTag", TextTag, EmptyString());
    context.Transfer("TextTagBack", TextTagBack, EmptyString());
    context.Transfer("Radius", Radius, 0.0f);
    context.Transfer("DisplayTime", DisplayTime, 0.0f);
}
int main()
{
    CTCInfoDisplay d; d.Radius = 1.0f; d.DisplayTime = 1.0f;
    int before = g_live;
    CPersistContext ctx;
    d.OnSerialise(ctx, true);
    if (g_count != 4 || g_live != before || !g_defaultsOk ||
        std::strcmp(g_names[0], "TextTag") || std::strcmp(g_names[1], "TextTagBack") ||
        std::strcmp(g_names[2], "Radius") || std::strcmp(g_names[3], "DisplayTime") ||
        g_members[0] != &d.TextTag || g_members[1] != &d.TextTagBack || g_members[2] != &d.Radius || g_members[3] != &d.DisplayTime ||
        d.Radius != 2.5f || std::strcmp(d.TextTagBack.Text, "loaded")) { std::printf("007e1b10_TEST FAIL\n"); return 1; }
    std::printf("007e1b10_TEST PASS\n");
    return 0;
}
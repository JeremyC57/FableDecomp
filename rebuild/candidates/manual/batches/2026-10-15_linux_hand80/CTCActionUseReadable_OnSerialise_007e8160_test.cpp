#include <cstdio>
#include <cstring>

static int g_live = 0, g_made = 0;
struct CWideString {
    const char* m_text;
    CWideString() : m_text("") { ++g_live; ++g_made; }
    CWideString(const CWideString& o) : m_text(o.m_text) { ++g_live; ++g_made; }
    ~CWideString() { --g_live; }
};
static const char* g_name = 0; static CWideString* g_value = 0; static int g_defaultEmpty = 0, g_liveAtCall = 0;
struct CPersistContext {
    void Transfer(const char* name, CWideString& value, const CWideString& def) { g_name = name; g_value = &value; g_defaultEmpty = (def.m_text[0] == 0); g_liveAtCall = g_live; }
};
inline CWideString EmptyWideString() { CWideString empty; return empty; }
struct CTCActionUseReadable {
    CWideString TextTag;
    void OnSerialise(CPersistContext& context, bool) { context.Transfer("GameTextDefName", TextTag, EmptyWideString()); }
};
int main()
{
    CTCActionUseReadable r; r.TextTag.m_text = "TEXT_SIGN";
    int liveBefore = g_live;
    CPersistContext ctx; r.OnSerialise(ctx, true);
    if (std::strcmp(g_name, "GameTextDefName") != 0 || g_value != &r.TextTag || !g_defaultEmpty) { std::printf("007e8160_TEST FAIL\n"); return 1; }
    if (g_live != liveBefore || g_liveAtCall < liveBefore + 1 || std::strcmp(r.TextTag.m_text, "TEXT_SIGN") != 0) { std::printf("007e8160_TEST FAIL\n"); return 1; }
    std::printf("007e8160_TEST PASS\n");
    return 0;
}

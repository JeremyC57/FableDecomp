#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void OnReplace() { Log("replace"); }
    virtual bool IsRunnable() { return false; }
    virtual bool IsRunnableLeaf() { return m_leaf; }
    bool m_leaf;
};
struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    bool m_seat;
    bool GetSeat() { Log("seat"); return m_seat; }
    virtual bool IsRunnable() { if (IsRunnableLeaf() && GetSeat()) { OnReplace(); return true; } return false; }
};
int main()
{
    CAIStateGroup_EatAtTable g;
    g.m_leaf = false; g.m_seat = true; if (g.IsRunnable()) return Check("0096d450", "x");
    g.m_leaf = true; g.m_seat = false; if (g.IsRunnable()) return Check("0096d450", "x");
    g.m_seat = true; if (!g.IsRunnable()) return Check("0096d450", "x");
    return Check("0096d450", "seat seat replace ");
}

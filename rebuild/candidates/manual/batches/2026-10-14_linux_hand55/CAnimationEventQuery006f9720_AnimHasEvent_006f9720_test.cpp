#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
static bool g_table = false, g_embedded = false; static long g_anim = -1, g_event = -1;
struct CQuery {
    unsigned char _pad[0x3d]; bool m_useEmbeddedEvents;
    bool AnimHasEventInTable(long a, long e) { note("T"); g_anim = a; g_event = e; return g_table; }
    bool AnimHasEmbeddedEvent(long a, long e) { note("E"); g_anim = a; g_event = e; return g_embedded; }
    bool AnimHasEvent(long animation, long event) { bool found = AnimHasEventInTable(animation, event); if (!found && m_useEmbeddedEvents) found = AnimHasEmbeddedEvent(animation, event); return found; }
};

int main()
{
    CQuery q; q.m_useEmbeddedEvents = false; g_table = true; g_embedded = false; g_seq[0] = 0;
    if (!q.AnimHasEvent(1, 2) || std::strcmp(g_seq, "T")) { std::printf("006f9720_TEST FAIL\n"); return 1; }
    g_table = false; g_seq[0] = 0; if (q.AnimHasEvent(1, 2) || std::strcmp(g_seq, "T")) { std::printf("006f9720_TEST FAIL\n"); return 1; }
    q.m_useEmbeddedEvents = true; g_seq[0] = 0; if (q.AnimHasEvent(1, 2) || std::strcmp(g_seq, "TE") || g_anim != 1 || g_event != 2) { std::printf("006f9720_TEST FAIL\n"); return 1; }
    g_embedded = true; g_seq[0] = 0; if (!q.AnimHasEvent(3, 4) || std::strcmp(g_seq, "TE") || g_anim != 3 || g_event != 4) { std::printf("006f9720_TEST FAIL\n"); return 1; }
    std::printf("006f9720_TEST PASS\n");
    return 0;
}

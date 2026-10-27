// Behaviour test for 0x00C1DD70 PKCOneShotQueue<SFileReply>::Pop (hand the single reply out, clear the filled flag)
#include <cstdio>

struct Reply { bool ok; long value; };
static int g_traces = 0;
struct OneShot {
    Reply item; long filled;
    Reply* Pop(Reply* out)
    {
        if (filled == 0) ++g_traces;               // retail traces "Nothing to return!" (line 28) and breaks
        filled = 0;
        *out = item;
        return out;
    }
};

int main()
{
    OneShot q; q.item.ok = false; q.item.value = 1234; q.filled = 1;
    Reply r; r.ok = true; r.value = 0;
    if (q.Pop(&r) != &r || r.ok || r.value != 1234 || q.filled != 0 || g_traces) { std::printf("00c1dd70_TEST FAIL\n"); return 1; }
    q.Pop(&r);
    if (g_traces != 1) { std::printf("00c1dd70_TEST FAIL\n"); return 1; }
    std::printf("00c1dd70_TEST PASS\n");
    return 0;
}
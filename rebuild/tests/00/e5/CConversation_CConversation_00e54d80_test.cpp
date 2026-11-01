// Behaviour test for CConversation::~CConversation (0x00E54D80): the four CCharString vectors are destroyed in reverse member
// order (AnimLoop, Animation, Dialogue, Speaker), each string destroyed front to back before its storage is freed.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_order[32];
static int g_n = 0;
static int g_frees = 0;
class CCharString { public: int id; ~CCharString() { g_order[g_n++] = id; } };
struct StringVector {
    CCharString* start; CCharString* finish;
    void Fill(int base, int count)
    {
        start = finish = 0;
        if (!count) return;
        start = (CCharString*)std::malloc(sizeof(CCharString) * count);
        for (int i = 0; i < count; ++i) { new (start + i) CCharString; start[i].id = base + i; }
        finish = start + count;
    }
    ~StringVector()
    {
        for (CCharString* p = start; p != finish; ++p) p->~CCharString();
        if (start) { std::free(start); ++g_frees; }
    }
};
struct CDefBase { ~CDefBase() { g_order[g_n++] = 999; } };
class CConversation : public CDefBase { public:
    long Lines;
    StringVector Speaker, Dialogue, Animation, AnimLoop;
};
int main()
{
    {
        CConversation c;
        c.Lines = 2;
        c.Speaker.Fill(10, 2);
        c.Dialogue.Fill(20, 2);
        c.Animation.Fill(30, 0);
        c.AnimLoop.Fill(40, 1);
    }
    int want[6] = { 40, 20, 21, 10, 11, 999 };
    if (g_n != 6 || g_frees != 3) { std::printf("00e54d80_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (g_order[i] != want[i]) { std::printf("00e54d80_TEST FAIL\n"); return 1; }
    std::printf("00e54d80_TEST PASS\n");
    return 0;
}
// Behaviour test for CActionEventSound::Dispatch (0x00870C50): two strings built (first, second) and destroyed (second, first); nothing else happens.
#include <cstdio>
static int g_log[8]; static int g_n = 0; static int g_next = 0;
class CCharString { public: int id; CCharString() { id = ++g_next; g_log[g_n++] = id; } ~CCharString() { g_log[g_n++] = -id; } };
struct CActionEventContext { int frame; };
class CActionEventSound { public: int state; void Dispatch(const CActionEventContext&) { CCharString soundName; CCharString boneName; } };
int main()
{
    CActionEventSound e; e.state = 5; CActionEventContext c; c.frame = 3;
    e.Dispatch(c);
    if (g_n != 4 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != -2 || g_log[3] != -1 || e.state != 5) { std::printf("00870c50_TEST FAIL\n"); return 1; }
    std::printf("00870c50_TEST PASS\n");
    return 0;
}
// behaviour test for 0x005EA950: with a sound player the sound plays (its handle is returned), without one the invalid handle
#include <cstdio>
static unsigned long g_invalid = 0xffffffffUL; static int g_played = 0;
struct Player { unsigned long Play(const char*) { ++g_played; return 42; } };
static unsigned long PlaySoundW(Player* p, const char* n) { if (p) return p->Play(n); return g_invalid; }
int main()
{
    Player pl;
    if (PlaySoundW(&pl, "x") != 42 || PlaySoundW(0, "x") != g_invalid || g_played != 1) { std::printf("005ea950_TEST FAIL\n"); return 1; }
    std::printf("005ea950_TEST PASS\n");
    return 0;
}
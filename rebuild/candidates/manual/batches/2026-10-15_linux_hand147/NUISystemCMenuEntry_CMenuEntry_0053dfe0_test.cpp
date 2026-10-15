#include <cstdio>
struct List;
static int g_base = 0, g_rumble = 0; static long g_id = 0;
struct Clickable { virtual ~Clickable() {} Clickable(long id) { ++g_base; g_id = id; } };
struct Entry : Clickable {
    bool IsAvailable; List* lists[8];
    Entry(long id) : Clickable(id), IsAvailable(true) { for (int i = 0; i < 8; ++i) lists[i] = 0; InitializeRumbleActions(); }
    void InitializeRumbleActions() { ++g_rumble; }
};
int main()
{
    Entry e(77);
    if (g_base != 1 || g_id != 77 || g_rumble != 1 || !e.IsAvailable) { std::printf("0053dfe0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) if (e.lists[i] != 0) { std::printf("0053dfe0_TEST FAIL\n"); return 1; }
    std::printf("0053dfe0_TEST PASS\n");
    return 0;
}

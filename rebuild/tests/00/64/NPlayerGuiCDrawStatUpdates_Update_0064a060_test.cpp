// Behaviour test for 0x0064A060 NPlayerGui::CDrawStatUpdates::Update (see src/0064a060.cpp): for the three stat categories
// and the spendable experience, a positive change against the previous frame's value queues a stat update once initialised;
// -1 marks "no previous value"; the first call only records.
#include <cstdio>
struct Update { int type; int change; };
static Update g_updates[16]; static int g_count;
static void AddStatUpdate(int type, int change) { g_updates[g_count].type = type; g_updates[g_count].change = change; ++g_count; }
struct Drawer {
    bool Initialised; int Previous[4];
    void Run(bool alive, const int stats[3], int spendable)
    {
        if (!alive) return;
        for (int i = 0; i < 4; ++i) {
            int value = i < 3 ? stats[i] : spendable;
            if (Initialised) {
                int previous = Previous[i];
                if (previous != -1) { int change = value - previous; if (change > 0) AddStatUpdate(i, change); }
            }
            Previous[i] = value;
        }
        Initialised = true;
    }
};
int main()
{
    Drawer d = { false, { -1, -1, -1, -1 } };
    int s1[3] = { 10, 20, 30 }; d.Run(true, s1, 5);
    if (g_count != 0 || !d.Initialised) { std::printf("0064a060_TEST FAIL\n"); return 1; }
    int s2[3] = { 15, 20, 25 }; d.Run(true, s2, 9);
    if (g_count != 2 || g_updates[0].type != 0 || g_updates[0].change != 5 || g_updates[1].type != 3 || g_updates[1].change != 4) { std::printf("0064a060_TEST FAIL\n"); return 1; }
    int s3[3] = { 99, 99, 99 }; d.Run(false, s3, 99);
    if (g_count != 2 || d.Previous[0] != 15) { std::printf("0064a060_TEST FAIL\n"); return 1; }
    std::printf("0064a060_TEST PASS\n");
    return 0;
}
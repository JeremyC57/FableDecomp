// behaviour test for 0x00587787: assign wins over remove, remove clears slot 1, otherwise the base handler runs
#include <cstdio>
struct Inv {
    long q[4]; bool assign, remove; int assigned, base;
    void Run() { if (assign) { assigned = 1; assign = false; } else if (remove) { q[1] = 0; remove = false; } else ++base; }
};
int main()
{
    Inv a = { { 5, 6, 7, 8 }, true, true, -1, 0 }; a.Run();
    Inv b = { { 5, 6, 7, 8 }, false, true, -1, 0 }; b.Run();
    Inv c = { { 5, 6, 7, 8 }, false, false, -1, 0 }; c.Run();
    if (a.assigned != 1 || !a.remove || b.q[1] != 0 || b.remove || c.base != 1) { std::printf("00587787_TEST FAIL\n"); return 1; }
    std::printf("00587787_TEST PASS\n");
    return 0;
}
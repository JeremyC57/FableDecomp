#include <cstdio>
// Behaviour model of CTCInventory::OpenPCScoreboard (0x00587B93): unless already on screen 8, deletes the menu
// components, switches to screen 8 and shows the hero's scoreboard component when it has one.
struct Inv { long screen; int deletes; bool hasBoard; int shown; };
static void Open(Inv& i)
{
    if (i.screen != 8) {
        ++i.deletes; i.screen = 8;
        if (i.hasBoard) ++i.shown;
    }
}
int main()
{
    Inv i = { 2, 0, true, 0 };
    Open(i);
    if (i.screen != 8 || i.deletes != 1 || i.shown != 1) { std::printf("00587b93_TEST FAIL\n"); return 1; }
    Open(i);
    if (i.deletes != 1 || i.shown != 1) { std::printf("00587b93_TEST FAIL\n"); return 1; }
    std::printf("00587b93_TEST PASS\n");
    return 0;
}

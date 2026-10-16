#include <cstdio>
// Behaviour model of CTCInventory::IsQuitMenuReceivingInput (0x005836D2): each screen lists its
// "PC_UI_MENU_ENTRY_LOAD_CONFIRM" entries; true as soon as a non-null entry is in state 2.
struct Entry { long state; };
struct Screen { Entry* entries[3]; unsigned int n; };
static bool IsQuit(const Screen* s, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
        for (unsigned int j = 0; j < s[i].n; ++j) {
            Entry* e = s[i].entries[j];
            if (e && e->state == 2) return true;
        }
    return false;
}
int main()
{
    Entry e1 = { 1 }, e2 = { 2 };
    Screen s[2] = { { { 0, &e1, 0 }, 2 }, { { &e1, &e2, 0 }, 2 } };
    if (!IsQuit(s, 2) || IsQuit(s, 1)) { std::printf("005836d2_TEST FAIL\n"); return 1; }
    s[1].n = 1;
    if (IsQuit(s, 2)) { std::printf("005836d2_TEST FAIL\n"); return 1; }
    std::printf("005836d2_TEST PASS\n");
    return 0;
}
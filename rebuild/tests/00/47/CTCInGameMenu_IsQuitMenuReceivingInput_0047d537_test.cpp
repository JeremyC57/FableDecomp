#include <cstdio>
// Behaviour model of CTCInGameMenu::IsQuitMenuReceivingInput (0x0047D537): true when some component's "UI_QUIT_LIST"
// child exists and is in input state 5 or 3.
struct Child { long state; };
struct Comp { Child* quitList; };
static bool IsQuit(const Comp* c, unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i) {
        Child* q = c[i].quitList;
        if (q && (q->state == 5 || q->state == 3)) return true;
    }
    return false;
}
int main()
{
    Child s1 = { 1 }, s3 = { 3 }, s5 = { 5 };
    Comp a[3] = { { 0 }, { &s1 }, { &s3 } };
    if (!IsQuit(a, 3) || IsQuit(a, 2)) { std::printf("0047d537_TEST FAIL\n"); return 1; }
    a[1].quitList = &s5;
    if (!IsQuit(a, 2)) { std::printf("0047d537_TEST FAIL\n"); return 1; }
    std::printf("0047d537_TEST PASS\n");
    return 0;
}
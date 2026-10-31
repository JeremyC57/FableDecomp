// behaviour test for NUISystem::CYesNo::ProcessEvent 0x0041CB10: events 4/5 reset the state to 0 and run the yes/no callback when set
#include <cstdio>
static int g_state = -1, g_yes = 0, g_no = 0;
static void Yes() { ++g_yes; }
static void No() { ++g_no; }
typedef void (*TCallback)();
struct CYesNo {
    TCallback YesCallback, NoCallback;
    void ChangeState(unsigned long s) { g_state = (int)s; }
    void ProcessEvent(int event)
    {
        if (event == 4 || event == 5)
            ChangeState(0);
        if (event == 4 && YesCallback)
            YesCallback();
        if (event == 5 && NoCallback)
            NoCallback();
    }
};
int main()
{
    CYesNo d; d.YesCallback = Yes; d.NoCallback = No;
    d.ProcessEvent(3);
    if (g_state != -1 || g_yes || g_no) { std::printf("0041cb10_TEST FAIL\n"); return 1; }
    d.ProcessEvent(4);
    if (g_state != 0 || g_yes != 1 || g_no) { std::printf("0041cb10_TEST FAIL\n"); return 1; }
    g_state = 7; d.ProcessEvent(5);
    if (g_state != 0 || g_yes != 1 || g_no != 1) { std::printf("0041cb10_TEST FAIL\n"); return 1; }
    d.NoCallback = 0; g_state = 7; d.ProcessEvent(5);
    if (g_state != 0 || g_no != 1) { std::printf("0041cb10_TEST FAIL\n"); return 1; }
    std::printf("0041cb10_TEST PASS\n");
    return 0;
}
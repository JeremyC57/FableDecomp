// Behaviour test for CTCHeroOnlineScoreboard::GlobalUpdate (00564570): each state steps one task; success advances, the failure code aborts with a message, anything else waits
#include <cstdio>

enum { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_WAIT = 1002 };
struct Board { int State; int Aborted; int Clans; int Selected; int Displayed; };

static void Step(Board& b, int result)
{
    switch (b.State) {
    case 1: case 2: case 4: case 6: case 7: case 8:
        if (result == NETWORK_BUSY) { b.Aborted = 1; return; }
        if (result == NETWORK_SUCCESS) b.State = b.State == 4 ? 5 : b.State + 1;
        return;
    case 3:
        if (result == NETWORK_BUSY) { b.Aborted = 1; return; }
        if (result == NETWORK_SUCCESS) {
            if (b.Clans == 0) { b.Aborted = 2; return; }
            b.State = 4;
        }
        return;
    case 5:
        if (b.Selected) b.State = 6;
        return;
    case 9:
        ++b.Displayed;
        return;
    }
}

int main()
{
    Board b = { 1, 0, 2, 1, 0 };
    for (int i = 0; i < 8; ++i) Step(b, NETWORK_SUCCESS);
    if (b.State != 9 || b.Aborted) { std::printf("00564570_TEST FAIL\n"); return 1; }
    Step(b, NETWORK_SUCCESS);
    if (b.Displayed != 1) { std::printf("00564570_TEST FAIL\n"); return 1; }
    Board w = { 2, 0, 0, 0, 0 };
    Step(w, NETWORK_WAIT);
    if (w.State != 2 || w.Aborted) { std::printf("00564570_TEST FAIL\n"); return 1; }
    Step(w, NETWORK_BUSY);
    if (w.Aborted != 1) { std::printf("00564570_TEST FAIL\n"); return 1; }
    Board n = { 3, 0, 0, 0, 0 };
    Step(n, NETWORK_SUCCESS);
    if (n.Aborted != 2 || n.State != 3) { std::printf("00564570_TEST FAIL\n"); return 1; }
    Board s = { 5, 0, 1, 0, 0 };
    Step(s, NETWORK_SUCCESS);
    if (s.State != 5) { std::printf("00564570_TEST FAIL\n"); return 1; }
    std::printf("00564570_TEST PASS\n");
    return 0;
}
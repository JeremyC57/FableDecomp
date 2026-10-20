#include <cstdio>
struct Wife { bool married; bool attracted; };
struct Thing { long mapIndex; Wife* wife; };
static int g_count[2] = { 0, 0 };
static long Region(long mapIndex) { return mapIndex / 10; }
static void Initialise(Thing** things, int n, long current, bool loaded)
{
    if (!loaded) return;
    for (int i = 0; i < n; ++i) {
        Thing* t = things[i];
        if (current == Region(t->mapIndex) && t->wife) {
            if (t->wife->married) ++g_count[0];
            else if (t->wife->attracted) ++g_count[1];
        }
    }
}
int main()
{
    Wife m = { true, true }, a = { false, true }, n = { false, false };
    Thing t1 = { 21, &m }, t2 = { 22, &a }, t3 = { 23, &n }, t4 = { 35, &m }, t5 = { 24, 0 };
    Thing* list[] = { &t1, &t2, &t3, &t4, &t5 };
    Initialise(list, 5, 2, true);
    if (g_count[0] != 1 || g_count[1] != 1) { std::printf("0082b520_TEST FAIL\n"); return 1; }
    std::printf("0082b520_TEST PASS\n");
    return 0;
}

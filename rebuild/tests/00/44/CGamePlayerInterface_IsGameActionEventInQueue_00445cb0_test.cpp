// Behaviour test for 00445cb0 CGamePlayerInterface::IsGameActionEventInQueue: equal range of the action map, then the per-action button list.
#include <cstdio>
struct Input { bool queued; bool InQueue(int, long, float& v) const { if (queued) v = 1.0f; return queued; } };
struct Entry { int action; Input* input; };
static bool InQueue(const Entry* map, int n, Input** quick, int nq, Input** ctx, int nc, Input** special, int ns, int action, float& v)
{
    const Entry* first = map; while (first != map + n && first->action < action) ++first;
    const Entry* last = first; while (last != map + n && last->action == action) ++last;
    for (; first != last; ++first) if (first->input->InQueue(0, 0, v)) return true;
    if (action == 0x37) { for (int i = 0; i < nq; ++i) if (quick[i]->InQueue(0, 0, v)) return true; }
    else if (action == 0x38) { for (int i = 0; i < nc; ++i) if (ctx[i]->InQueue(0, 0, v)) return true; }
    else if (action == 0x36) { for (int i = 0; i < ns; ++i) if (special[i]->InQueue(0, 0, v)) return true; }
    return false;
}
int main()
{
    Input no = { false }, yes = { true };
    Entry map[3] = { { 1, &no }, { 0x37, &no }, { 0x40, &yes } };
    Input* quick[2] = { &no, &yes }; Input* none[1] = { &no };
    float v = 0;
    bool a = InQueue(map, 3, quick, 2, none, 1, none, 1, 0x37, v);
    bool b = InQueue(map, 3, quick, 2, none, 1, none, 1, 0x38, v);
    bool c = InQueue(map, 3, quick, 2, none, 1, none, 1, 0x40, v);
    bool d = InQueue(map, 3, quick, 2, none, 1, none, 1, 1, v);
    if (!a || b || !c || d || v != 1.0f) { std::printf("00445cb0_TEST FAIL\n"); return 1; }
    std::printf("00445cb0_TEST PASS\n");
    return 0;
}
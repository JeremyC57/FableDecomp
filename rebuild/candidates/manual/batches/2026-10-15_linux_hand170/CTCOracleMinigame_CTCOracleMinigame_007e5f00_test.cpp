#include <cstdio>
#include <cstdlib>
// Behaviour model of CTCOracleMinigame::CTCOracleMinigame (0x007E5F00): level 1, stage 0, not won, a self-linked sentinel for the
// pending-input list, a null def that GetSubDef then fills.
struct Node { Node* next; Node* prev; long value; };
struct Oracle { long level, stage; bool success; Node* inputs; void* def; };
static void Construct(Oracle& o)
{
    o.level = 1; o.stage = 0; o.success = false;
    o.inputs = 0;
    Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; o.inputs = s;
    o.def = 0;
}
int main()
{
    Oracle o; o.success = true; o.level = 9;
    Construct(o);
    bool ok = o.level == 1 && o.stage == 0 && !o.success && o.inputs && o.inputs->next == o.inputs && o.inputs->prev == o.inputs && !o.def;
    std::free(o.inputs);
    if (!ok) { std::printf("007e5f00_TEST FAIL\n"); return 1; }
    std::printf("007e5f00_TEST PASS\n");
    return 0;
}

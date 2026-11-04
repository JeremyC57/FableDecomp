// Behaviour test for NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry (0x00881690): a model of the row.
#include <stdio.h>

struct Entry { long frames; };
struct Stack { Entry e[4][4]; int n[4]; Entry* keys[8]; int nkeys; };
static Entry* Insert(Stack& s, float time, int priority, long fps)
{
    Entry& e = s.e[priority][s.n[priority]++]; e.frames = (long)(time * fps);
    s.keys[s.nkeys++] = &e; return &e;
}
static int run()
{
    Stack s = { 0 }; Entry* a = Insert(s, 1.5f, 2, 30); Entry* b = Insert(s, 0.1f, 2, 30);
    return a->frames == 45 && b->frames == 3 && s.n[2] == 2 && s.nkeys == 2 && s.keys[1] == b;
}
int main()
{
    int ok = run();
    printf(ok ? "00881690_TEST PASS\n" : "00881690_TEST FAIL\n");
    return ok ? 0 : 1;
}

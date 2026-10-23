// Behaviour test for NInventory::CTCInventoryTrade::AddWantedItemToList @ 0x0059CB90: five actions with the right types and params
#include <cstdio>
struct Action { int type; int params[4]; int count; int targets; };
struct Entry { int selected; int focus; int unfocus; Entry() : selected(0), focus(0), unfocus(0) {} };
static int Wire(int defIndex, Entry& entry, Action* actions)
{
    int n = 0;
    Action* a = &actions[n++]; a->type = 0x7d0; a->count = 0; a->targets = 0;
    a->params[a->count++] = defIndex; a->targets++; a->params[a->count++] = -1; entry.selected++;
    a = &actions[n++]; a->type = 0x7df; a->count = 0; a->targets = 1; entry.focus++;
    a = &actions[n++]; a->type = 0x7db; a->count = 0; a->params[a->count++] = defIndex; a->targets = 2; entry.selected++;
    a = &actions[n++]; a->type = 0x7e7; a->count = 0; a->targets = 1; entry.focus++;
    a = &actions[n++]; a->type = 0x7d6; a->count = 0; a->params[a->count++] = defIndex; a->targets = 1;
    a->params[a->count++] = 1; entry.unfocus++;
    return n;
}
int main()
{
    int ok = 1;
    Entry e; Action acts[5];
    int n = Wire(42, e, acts);
    if (n != 5 || e.selected != 2 || e.focus != 2 || e.unfocus != 1) ok = 0;
    if (acts[0].type != 2000 || acts[0].params[0] != 42 || acts[0].count != 2) ok = 0;
    if (acts[2].type != 2011 || acts[2].targets != 2) ok = 0;
    if (acts[4].type != 2006 || acts[4].params[1] != 1 || acts[4].count != 2) ok = 0;
    if (!ok) { std::printf("0059cb90_TEST FAIL\n"); return 1; }
    std::printf("0059cb90_TEST PASS\n");
    return 0;
}
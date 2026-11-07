// Behaviour test for 0x0070C4E0 CAnimateThing::GetFirstCombinationAnimFromType: a standalone model of the parked source
// (the residue is one epilogue scheduling line). Two intrusive lists of anim sets (the current animation's combination
// anims, then the delayed animation's); the first set whose type matches wins, else null. Prints exactly one line.
#include <cstdio>

struct AnimSet { AnimSet* next; long type; const char* name; };
struct AnimList { AnimSet head; AnimList() { head.next = &head; head.type = -1; head.name = "end"; } };

static void Append(AnimList& list, AnimSet& set)
{
    AnimSet* last = &list.head;
    while (last->next != &list.head) last = last->next;
    last->next = &set; set.next = &list.head;
}

// The parked shape: find in the current list, early return; find in the delayed list, early return; else 0.
static AnimSet* GetFirstCombinationAnimFromType(AnimList& current, AnimList& delayed, long type)
{
    AnimSet* end = &current.head;
    AnimSet* it = current.head.next;
    for (; it != end; it = it->next) if (it->type == type) break;
    if (it != end) return it;
    end = &delayed.head;
    it = delayed.head.next;
    for (; it != end; it = it->next) if (it->type == type) break;
    if (it != end) return it;
    return 0;
}

int main()
{
    int fails = 0;
    AnimList current, delayed;
    AnimSet a = { 0, 3, "cur3a" }, b = { 0, 5, "cur5" }, c = { 0, 3, "cur3b" }, d = { 0, 7, "del7" }, e = { 0, 3, "del3" };
    Append(current, a); Append(current, b); Append(current, c); Append(delayed, d); Append(delayed, e);
    if (GetFirstCombinationAnimFromType(current, delayed, 3) != &a) ++fails;   // first match of the current list
    if (GetFirstCombinationAnimFromType(current, delayed, 5) != &b) ++fails;
    if (GetFirstCombinationAnimFromType(current, delayed, 7) != &d) ++fails;   // only in the delayed list
    if (GetFirstCombinationAnimFromType(current, delayed, 9) != 0) ++fails;    // in neither
    AnimList empty1, empty2;
    if (GetFirstCombinationAnimFromType(empty1, empty2, 3) != 0) ++fails;      // both empty
    if (GetFirstCombinationAnimFromType(empty1, delayed, 3) != &e) ++fails;    // empty current list
    std::printf(fails == 0 ? "0070c4e0_TEST PASS\n" : "0070c4e0_TEST FAIL\n");
    return fails == 0 ? 0 : 1;
}

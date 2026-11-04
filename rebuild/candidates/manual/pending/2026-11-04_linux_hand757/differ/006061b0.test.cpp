// Behaviour test for 0x006061B0 CTCInventoryMap::FrameUpdate (quest-drop tail): a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <list>
struct Card { bool dead; long def; bool hasQuest; bool failed, deactivated; };
static long DropQuest(std::list<Card*>& cards, long toDrop)
{
    if (toDrop > 0) {
        for (std::list<Card*>::iterator it = cards.begin(); it != cards.end(); ++it) {
            Card* c = *it;
            if (c && !c->dead && c->def == toDrop) { if (c->hasQuest) { c->failed = true; c->deactivated = true; } break; }
        }
        toDrop = 0;
    }
    return toDrop;
}
int main() {
    Card a = { false, 3, true, false, false }, b = { true, 5, true, false, false }, c = { false, 5, true, false, false };
    std::list<Card*> l; l.push_back(&a); l.push_back(0); l.push_back(&b); l.push_back(&c);
    int ok = DropQuest(l, 5) == 0 && !b.failed && c.failed && c.deactivated && !a.failed;
    std::printf(ok ? "006061b0_TEST PASS\n" : "006061b0_TEST FAIL\n");
    return ok ? 0 : 1;
}

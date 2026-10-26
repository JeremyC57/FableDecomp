// Behaviour test for 0x008FD1B0: DestroyItem kills the carried item of the named definition and clears the slot.
#include <cstdio>
#include <cstring>
class CThing { public: bool killed; void Kill(bool) { killed = true; } };
class CTCCarrying { public:
    CThing* slot[4]; bool removed[4];
    CThing* GetThingInCarrySlot(long i) { return slot[i]; }
    CThing* RemoveThingInCarrySlot(long i, bool) { removed[i] = true; CThing* t = slot[i]; slot[i] = 0; return t; }
};
struct CCreature { bool hasCarrying; CTCCarrying* carrying;
    bool GetInterface(CTCCarrying** out, long type) { if (type == 0x46 && hasCarrying) { *out = carrying; return true; } return false; } };
static long DefIndexFromName(const char* name) { return std::strcmp(name, "AXE") == 0 ? 2 : 1; }
static void DestroyItem(CCreature* creature, const char* itemName)
{
    CTCCarrying* carrying;
    if (creature->GetInterface(&carrying, 0x46)) {
        long defIndex = DefIndexFromName(itemName);
        CThing* item = carrying->GetThingInCarrySlot(defIndex);
        if (item)
            item->Kill(true);
        carrying->RemoveThingInCarrySlot(defIndex, false);
    }
}
int main()
{
    CThing axe = { false };
    CTCCarrying c; std::memset(&c, 0, sizeof c); c.slot[2] = &axe;
    CCreature cr = { true, &c };
    DestroyItem(&cr, "AXE");
    int bad = (!axe.killed || c.slot[2] != 0 || !c.removed[2]);
    DestroyItem(&cr, "BUCKET");          // empty slot: no kill, slot still cleared
    if (!c.removed[1]) bad = 1;
    cr.hasCarrying = false; c.removed[2] = false;
    DestroyItem(&cr, "AXE");
    if (c.removed[2]) bad = 1;
    if (bad) { std::printf("008fd1b0_TEST FAIL\n"); return 1; }
    std::printf("008fd1b0_TEST PASS\n");
    return 0;
}
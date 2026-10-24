// Behaviour test for 0x0056FBED YesCloseInventory: with the main player's character alive, its interface 0x16
// (the experience inventory) gets CloseInventoryInternal(true); a dead or missing character closes nothing.
#include <cstdio>

static int g_closed = 0, g_lastUndo = -1;
struct CTCInventoryExperience {
    virtual void CloseInventoryInternal(bool undo) { ++g_closed; g_lastUndo = undo ? 1 : 0; }
};
struct SEntry { int type; void* component; };
struct CThing {
    unsigned int present;          // interface presence bits (bit 0x16 tested)
    SEntry entries[2]; int count;
    unsigned char DeadFlags;
    bool test(int bit) const { return (present >> bit) & 1; }
    template<class T> bool GetInterface(T** out, int wanted)
    {
        if (test(wanted)) {
            SEntry* last = entries + count;
            SEntry* it = entries;
            while (it != last && it->type < wanted) ++it;   // lower_bound
            if (it == last || wanted < it->type) it = last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
static CThing* g_hero = 0;
template<class T> static bool IS_THING_ALIVE(const T& thing) { return thing != 0 && (thing->DeadFlags & 1) == 0; }

static void YesCloseInventory()
{
    CThing* hero = g_hero;
    if (IS_THING_ALIVE(hero)) {
        CTCInventoryExperience* inventory = 0;
        hero->GetInterface(&inventory, 0x16);
        inventory->CloseInventoryInternal(true);
    }
}

int main()
{
    CTCInventoryExperience screen;
    CThing hero;
    hero.present = 1u << 0x16;
    hero.entries[0].type = 4; hero.entries[0].component = 0;
    hero.entries[1].type = 0x16; hero.entries[1].component = &screen;
    hero.count = 2; hero.DeadFlags = 0;
    g_hero = &hero;
    YesCloseInventory();
    bool ok = g_closed == 1 && g_lastUndo == 1;
    hero.DeadFlags = 1;
    YesCloseInventory();
    g_hero = 0;
    YesCloseInventory();
    ok = ok && g_closed == 1;
    std::printf(ok ? "0056fbed_TEST PASS\n" : "0056fbed_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for CThingPlayerCreature::LearnExpression (0x006AC430): a copy of the control flow over a small model --
// availability, index/type checks, the spawned object's components, inventory add, learned list, quick-access slot choice,
// tutorial, def release counts on every path.
#include <cstdio>
#include <cstring>

struct Def { long refs; long dummyObject; bool destroyed; };
struct Item { bool hasExpression; bool hasItemTC; long defGlobalIndex; bool killed; long category; };
struct World {
    bool available; long index; const char* type; Def def; bool hasInventory; bool hasAbilities; bool hasHero;
    bool spawnFails; Item item; long added; long learned; long nextFree; long slotSet; long slotThing; long tutorial;
};

static void Release(Def& d) { if (--d.refs == 0) d.destroyed = true; }

static bool LearnExpression(World& w, long quickAccessSlot, bool silent)
{
    if (w.available) return false;
    long expression = w.index;
    if (expression <= 0) return false;
    if (std::strcmp(w.type, "EXPRESSION") != 0) return false;
    Def* def = &w.def; ++def->refs;                       // GetDef hands out a counted handle
    Item* item = w.spawnFails ? 0 : &w.item;
    bool ok = false;
    if (item) {
        if (!item->hasExpression) item->killed = true;
        else if (w.hasInventory) {
            Item* itemTC = item->hasItemTC ? item : 0;
            if (itemTC) ++w.added;
            if (w.hasAbilities) w.learned = expression;
            if (quickAccessSlot != -1 || (quickAccessSlot = w.nextFree) != -1) { w.slotSet = quickAccessSlot; w.slotThing = item->defGlobalIndex; }
            if (!silent && w.hasHero && itemTC->category) w.tutorial = itemTC->category;
            ok = true;
        }
    }
    Release(*def);
    return ok;
}

static World Base()
{
    World w; std::memset(&w, 0, sizeof w);
    w.index = 7; w.type = "EXPRESSION"; w.def.refs = 1; w.def.dummyObject = 99;
    w.hasInventory = w.hasAbilities = w.hasHero = true; w.item.hasExpression = w.item.hasItemTC = true;
    w.item.defGlobalIndex = 99; w.item.category = 3; w.nextFree = 4; w.slotSet = -2; return w;
}

int main()
{
    int bad = 0;
    World w = Base();
    if (!LearnExpression(w, -1, false) || w.added != 1 || w.learned != 7 || w.slotSet != 4 || w.slotThing != 99 || w.tutorial != 3 || w.def.refs != 1) bad |= 1;
    w = Base();
    if (!LearnExpression(w, 2, true) || w.slotSet != 2 || w.tutorial != 0) bad |= 2;
    w = Base(); w.nextFree = -1;
    if (!LearnExpression(w, -1, false) || w.slotSet != -2) bad |= 4;
    w = Base(); w.available = true;
    if (LearnExpression(w, -1, false) || w.added) bad |= 8;
    w = Base(); w.index = 0;
    if (LearnExpression(w, -1, false)) bad |= 16;
    w = Base(); w.type = "WEAPON";
    if (LearnExpression(w, -1, false)) bad |= 32;
    w = Base(); w.item.hasExpression = false;
    if (LearnExpression(w, -1, false) || !w.item.killed || w.def.refs != 1) bad |= 64;
    w = Base(); w.hasInventory = false;
    if (LearnExpression(w, -1, false) || w.added || w.def.refs != 1) bad |= 128;
    w = Base(); w.spawnFails = true;
    if (LearnExpression(w, -1, false) || w.def.refs != 1) bad |= 256;
    if (bad) { std::printf("006ac430_TEST FAIL\n"); return 1; }
    std::printf("006ac430_TEST PASS\n");
    return 0;
}
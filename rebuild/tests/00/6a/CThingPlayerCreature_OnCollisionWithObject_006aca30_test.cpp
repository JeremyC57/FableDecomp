// Behaviour test for 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject(CThing*) (model copy; plain C++98, no engine headers).
// The model mirrors the row's decisions: the object's properties (0x400 gold, 0x2000000 pickable object, anything else ignored),
// the stock-for-sale veto, the quest-container branch (CTCActionUseActivateQuest class-name test, the container's item list:
// gold defs pay the hero, items flagged DoNotAutoPickUp or not yet picked up are created and shown, the rest are added to the
// inventory with an auto-pickup notice; the container is then killed without OnKill), the single-object branch
// (AutoPickUpAfterFirstPickUp + already picked up + inventory accepts it -> auto pickup, else shown), the gold branch (money,
// rent money and the online score when the gold belongs to a rented house) and the final pickup sound / kill.
#include <cstdio>
#include <cstring>
#include <vector>

struct ItemDef { long index; bool is_gold; long gold; bool has_item_def; bool do_not_auto; bool auto_after_first; };
struct ContainerItem { long index; const char* name; };

struct Hero {
    bool has_stats, has_receive, has_sound, has_scoreboard;
    long money, rent_money;
    std::vector<long> picked_up;
    std::vector<long> shown;          // GiveItem (created / touched objects)
    std::vector<long> inventory;      // CreateAndAddObjectToInventory / AddToInventory
    std::vector<long> auto_pickups;   // CPlayerGui::AddAutoPickup
    const char* last_sound;
    int sounds_played;
    float last_score; int scores;
    bool inventory_accepts;
    bool HasPickedUpItem(long i) const { for (size_t k = 0; k < picked_up.size(); ++k) if (picked_up[k] == i) return true; return false; }
    void SetItemAsPickedUp(long i) { if (!HasPickedUpItem(i)) picked_up.push_back(i); }
};

struct Thing {
    unsigned long properties;
    bool for_sale, has_stock;
    bool is_quest_container, has_container;
    std::vector<ContainerItem> items;
    long def_index;
    long gold; bool has_gold;
    bool owner_is_rented_house;
    bool killed; bool kill_arg;
};

static std::vector<ItemDef> g_defs;
static const ItemDef* FindDef(long index) { for (size_t i = 0; i < g_defs.size(); ++i) if (g_defs[i].index == index) return &g_defs[i]; return 0; }

static void OnCollisionWithObject(Hero& hero, Thing& thing)
{
    bool collected = false, kill_thing = false, call_on_kill = true;
    const char* sound = "";
    switch (thing.properties) {
    case 0x2000000:
        if (thing.has_stock && thing.for_sale)
            break;
        if (thing.is_quest_container) {
            if (!thing.has_container || thing.items.empty())
                break;
            for (unsigned long i = 0; i < thing.items.size(); ++i) {
                const ItemDef* def = FindDef(thing.items[i].index);
                if (!def)
                    continue;
                if (def->is_gold) {
                    if (hero.has_stats) { hero.money += def->gold; sound = "gold"; }
                } else {
                    bool give_item = def->has_item_def && def->do_not_auto;
                    if (!give_item && hero.has_receive && !hero.HasPickedUpItem(def->index)) {
                        hero.SetItemAsPickedUp(def->index);
                        give_item = true;
                    }
                    if (!give_item) {
                        hero.inventory.push_back(def->index);
                        hero.auto_pickups.push_back(def->index);
                        sound = "item";
                    } else {
                        hero.shown.push_back(def->index);
                    }
                }
            }
            kill_thing = true;
            call_on_kill = false;
            collected = true;
        } else {
            bool already_picked_up = false;
            const ItemDef* def = FindDef(thing.def_index);
            if (def && def->has_item_def && def->auto_after_first && hero.has_receive) {
                if (hero.HasPickedUpItem(thing.def_index)) already_picked_up = true;
                else hero.SetItemAsPickedUp(thing.def_index);
            }
            bool added = false;
            if (already_picked_up && hero.inventory_accepts) {
                hero.inventory.push_back(thing.def_index);
                hero.auto_pickups.push_back(thing.def_index);
                sound = "item";
                added = true;
            }
            if (!added)
                hero.shown.push_back(thing.def_index);
            collected = true;
        }
        break;
    case 0x400:
        if (!hero.has_stats)
            break;
        collected = true;
        kill_thing = true;
        if (thing.has_gold) {
            hero.money += thing.gold;
            sound = "gold";
            if (thing.owner_is_rented_house) {
                hero.rent_money += thing.gold;
                hero.last_score = (float)hero.rent_money; ++hero.scores;
            }
        }
        break;
    }
    if (collected) {
        if (hero.has_sound) { hero.last_sound = sound; ++hero.sounds_played; }
        if (kill_thing) { thing.killed = true; thing.kill_arg = call_on_kill; }
    }
}

static Hero NewHero() { Hero h; h.has_stats = h.has_receive = h.has_sound = h.has_scoreboard = true; h.money = 0; h.rent_money = 10;
    h.last_sound = 0; h.sounds_played = 0; h.last_score = 0; h.scores = 0; h.inventory_accepts = true; return h; }
static Thing NewThing(unsigned long p) { Thing t; t.properties = p; t.for_sale = t.has_stock = false; t.is_quest_container = false;
    t.has_container = true; t.def_index = 0; t.gold = 0; t.has_gold = false; t.owner_is_rented_house = false; t.killed = false;
    t.kill_arg = true; return t; }

int main()
{
    int fail = 0;
    ItemDef d1 = { 1, true, 50, false, false, false }; g_defs.push_back(d1);          // gold def
    ItemDef d2 = { 2, false, 0, true, true, false };  g_defs.push_back(d2);           // never auto-picked
    ItemDef d3 = { 3, false, 0, true, false, true };  g_defs.push_back(d3);           // auto after first pickup
    // gold with a rented-house owner
    { Hero h = NewHero(); Thing t = NewThing(0x400); t.has_gold = true; t.gold = 25; t.owner_is_rented_house = true;
      OnCollisionWithObject(h, t);
      if (h.money != 25 || h.rent_money != 35 || h.scores != 1 || h.last_score != 35.0f || !t.killed || !t.kill_arg ||
          std::strcmp(h.last_sound, "gold") != 0) fail |= 1; }
    // gold without hero stats: nothing at all
    { Hero h = NewHero(); h.has_stats = false; Thing t = NewThing(0x400); t.has_gold = true; t.gold = 5;
      OnCollisionWithObject(h, t);
      if (h.money != 0 || t.killed || h.sounds_played != 0) fail |= 2; }
    // object for sale: ignored
    { Hero h = NewHero(); Thing t = NewThing(0x2000000); t.has_stock = t.for_sale = true; t.def_index = 3;
      OnCollisionWithObject(h, t);
      if (!h.shown.empty() || h.sounds_played != 0) fail |= 4; }
    // quest container: gold paid, item 2 shown, item 3 shown first time then auto-picked; container killed without OnKill
    { Hero h = NewHero(); Thing t = NewThing(0x2000000); t.is_quest_container = true;
      ContainerItem a = { 1, "" }, b = { 2, "" }, c = { 3, "" }, d = { 3, "" };
      t.items.push_back(a); t.items.push_back(b); t.items.push_back(c); t.items.push_back(d);
      OnCollisionWithObject(h, t);
      if (h.money != 50 || h.shown.size() != 2 || h.shown[0] != 2 || h.shown[1] != 3 || h.inventory.size() != 1 ||
          h.auto_pickups.size() != 1 || !t.killed || t.kill_arg || std::strcmp(h.last_sound, "item") != 0) fail |= 8; }
    // empty quest container: nothing
    { Hero h = NewHero(); Thing t = NewThing(0x2000000); t.is_quest_container = true;
      OnCollisionWithObject(h, t);
      if (t.killed || h.sounds_played != 0) fail |= 16; }
    // single object: first touch shown and remembered, second touch auto-picked up; never killed
    { Hero h = NewHero(); Thing t = NewThing(0x2000000); t.def_index = 3;
      OnCollisionWithObject(h, t);
      if (h.shown.size() != 1 || !h.HasPickedUpItem(3) || t.killed || h.sounds_played != 1) fail |= 32;
      OnCollisionWithObject(h, t);
      if (h.shown.size() != 1 || h.inventory.size() != 1 || h.auto_pickups.size() != 1 || t.killed) fail |= 64;
      h.inventory_accepts = false; OnCollisionWithObject(h, t);
      if (h.shown.size() != 2) fail |= 128; }
    // other properties: ignored
    { Hero h = NewHero(); Thing t = NewThing(0x10); OnCollisionWithObject(h, t);
      if (h.sounds_played != 0 || t.killed) fail |= 256; }
    if (fail) { std::printf("006aca30_TEST FAIL\n"); return 1; }
    std::printf("006aca30_TEST PASS\n");
    return 0;
}
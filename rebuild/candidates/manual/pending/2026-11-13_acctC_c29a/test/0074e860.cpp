// Behaviour test for 0x0074E860 CTCContainerRewardHero::GetRewardItems(items, gold): gold starts at 0 and the item list is cleared; a gold
// component sets gold. Stored container items are handed over (potion dummy -> dynamic potion index if > 0; resurrection dummy -> dynamic
// resurrection index if > 0, else +20 gold; anything else copied as is) and the container is emptied. With no stored items the def's object
// family pick decides: a gold object is spawned (no item), a potion / resurrection dummy is resolved the same way (a resurrection pick of 0
// gives +20 gold, a negative one nothing), any other positive index becomes an item with an empty script name.
#include <cstdio>
#include <string>
#include <vector>
#include <utility>
typedef std::vector<std::pair<long, std::string> > Items;
enum { POTION = 100, RESURRECTION = 200 };
struct World {
    bool hasGoldTC; long goldTC;
    Items container;
    long dynPotion, dynRes;
    bool hasDef; std::vector<long> families; long familyPick;   // index into families
    bool familyDefOk; long familyObject; bool thingDefOk; bool isGold;
    int spawned; long spawnedIndex;
};
static void GetRewardItems(World& w, Items& items, long& gold)
{
    gold = 0;
    items.clear();
    if (w.hasGoldTC) gold = w.goldTC;
    if (!w.container.empty()) {
        for (unsigned long i = 0; i < w.container.size(); ++i) {
            if (w.container[i].first == POTION) {
                long index = w.dynPotion;
                if (index > 0) items.push_back(std::make_pair(index, w.container[i].second));
            } else if (w.container[i].first == RESURRECTION) {
                long index = w.dynRes;
                if (index > 0) items.push_back(std::make_pair(index, w.container[i].second));
                else gold += 20;
            } else items.push_back(w.container[i]);
        }
        w.container.clear();
        return;
    }
    if (w.hasDef && !w.families.empty()) {
        if (w.familyDefOk) {
            long index = w.familyObject;
            if (index > 0) {
                if (w.thingDefOk && w.isGold) { ++w.spawned; w.spawnedIndex = index; return; }
                if (index == POTION) index = w.dynPotion;
                else if (index == RESURRECTION) { index = w.dynRes; if (index == 0) gold += 20; }
                if (index > 0) items.push_back(std::make_pair(index, std::string("")));
            }
        }
    }
}
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { ++fails; std::printf("FAIL %s\n", what); } }
static World Base() { World w; w.hasGoldTC = false; w.goldTC = 0; w.familyPick = 0; w.familyObject = 0; w.isGold = false; w.spawned = 0; w.spawnedIndex = 0; w.dynPotion = 7; w.dynRes = 9; w.hasDef = true; w.families.push_back(1); w.familyDefOk = true; w.thingDefOk = true; return w; }
int main()
{
    Items items; long gold = -5;
    World w = Base(); items.push_back(std::make_pair(1L, std::string("old")));
    w.hasGoldTC = true; w.goldTC = 33;
    w.container.push_back(std::make_pair((long)POTION, std::string("p")));
    w.container.push_back(std::make_pair((long)RESURRECTION, std::string("r")));
    w.container.push_back(std::make_pair(55L, std::string("s")));
    GetRewardItems(w, items, gold);
    check(gold == 33 && items.size() == 3 && items[0].first == 7 && items[0].second == "p" && items[1].first == 9 && items[1].second == "r"
          && items[2].first == 55 && items[2].second == "s" && w.container.empty(), "stored items resolved");
    w = Base(); w.dynPotion = 0; w.dynRes = -1;
    w.container.push_back(std::make_pair((long)POTION, std::string("p")));
    w.container.push_back(std::make_pair((long)RESURRECTION, std::string("r")));
    GetRewardItems(w, items, gold);
    check(gold == 20 && items.empty() && w.container.empty(), "failed picks: potion nothing, resurrection +20");
    w = Base(); w.familyObject = 42; w.isGold = true;
    GetRewardItems(w, items, gold);
    check(gold == 0 && items.empty() && w.spawned == 1 && w.spawnedIndex == 42, "gold object spawned");
    w = Base(); w.familyObject = 42; w.thingDefOk = false;
    GetRewardItems(w, items, gold);
    check(items.size() == 1 && items[0].first == 42 && items[0].second.empty() && w.spawned == 0, "plain object item");
    w = Base(); w.familyObject = POTION;
    GetRewardItems(w, items, gold);
    check(items.size() == 1 && items[0].first == 7, "def potion dummy");
    w = Base(); w.familyObject = RESURRECTION; w.dynRes = 0;
    GetRewardItems(w, items, gold);
    check(items.empty() && gold == 20, "def resurrection 0 -> +20");
    w = Base(); w.familyObject = RESURRECTION; w.dynRes = -3;
    GetRewardItems(w, items, gold);
    check(items.empty() && gold == 0, "def resurrection negative -> nothing");
    w = Base(); w.familyObject = 0;
    GetRewardItems(w, items, gold);
    check(items.empty() && gold == 0, "no object");
    w = Base(); w.families.clear(); w.familyObject = 42;
    GetRewardItems(w, items, gold);
    check(items.empty(), "no families");
    w = Base(); w.familyDefOk = false; w.familyObject = 42;
    GetRewardItems(w, items, gold);
    check(items.empty(), "family def missing");
    if (fails == 0) std::printf("0074e860_TEST PASS\n");
    return fails;
}

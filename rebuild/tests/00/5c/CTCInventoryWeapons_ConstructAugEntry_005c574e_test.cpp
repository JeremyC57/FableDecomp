// Behaviour test of CTCInventoryWeapons::ConstructAugEntry (005c574e): the entry's title is "Augment x<sum of the counts>",
// it owns the sub list, the left description and a name text, and the sub list gets one action entry per owned augmentation ITEM
// (an augmentation with count n gives n entries), each with an add-child-augmentation action (23) carrying the augmentation's
// inventory graphic, a delete-all action (24) on the item entry and an augment action (2) carrying the def index and the entry.
// 
#include <cstdio>
#include <string>
#include <utility>
#include <vector>
struct CAction { long ActionType; std::vector<long> ThingParams; std::vector<std::string> UIParams; };
struct CComponent {
    std::string Name, Text;
    std::vector<CComponent*> Children, ScrollingChildren, PositionChildren;
    std::vector<CAction> Actions;
    long Graphic; unsigned long Offsets;
    CComponent(const std::string& n) : Name(n), Graphic(-1), Offsets(0) { }
};
static std::vector<CComponent*> g_All;
static CComponent* Create(const std::string& n) { CComponent* c = new CComponent(n); g_All.push_back(c); return c; }
static std::string IntToString(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static long InventoryGraphicOf(long defIndex) { return defIndex * 10; }
static CComponent* ConstructAugEntry(std::vector<std::pair<long, long> >& augmentations, CComponent* parent)
{
    long total = 0;
    for (unsigned long i = 0; i < augmentations.size(); ++i)
        total += augmentations[i].second;
    CComponent* entry = Create("entry");
    entry->Text = "Augment x" + IntToString(total);
    CComponent* subList = Create("sublist");
    CComponent* description = Create("left");
    entry->Children.push_back(subList);
    entry->Children.push_back(description);
    if (parent) parent->PositionChildren.push_back(description);
    CComponent* name = Create("name");
    name->Text = entry->Text;
    entry->Children.push_back(name);
    if (parent) parent->PositionChildren.push_back(name);
    subList->Offsets = total;
    for (unsigned long i = 0; i < augmentations.size(); ++i) {
        for (long j = 0; j < augmentations[i].second; ++j) {
            CComponent* item = Create("item");
            CComponent* sprite = Create("sprite");
            sprite->Graphic = InventoryGraphicOf(augmentations[i].first);
            item->Children.push_back(sprite);
            if (parent) parent->PositionChildren.push_back(sprite);
            CAction show; show.ActionType = 23; show.UIParams.push_back(IntToString(sprite->Graphic));
            if (parent) show.UIParams.push_back("parent");
            item->Actions.push_back(show);
            CAction del; del.ActionType = 24; del.UIParams.push_back("item");
            item->Actions.push_back(del);
            CAction aug; aug.ActionType = 2; aug.ThingParams.push_back(augmentations[i].first); aug.UIParams.push_back("entry");
            item->Actions.push_back(aug);
            subList->ScrollingChildren.push_back(item);
        }
    }
    return entry;
}
int main()
{
    std::vector<std::pair<long, long> > augs;
    augs.push_back(std::make_pair(7L, 2L));
    augs.push_back(std::make_pair(9L, 1L));
    augs.push_back(std::make_pair(4L, 0L));
    CComponent* parent = 0;
    CComponent* entry = ConstructAugEntry(augs, parent);
    CComponent* subList = entry->Children[0];
    int bad = entry->Text != "Augment x3" || subList->Offsets != 3 || subList->ScrollingChildren.size() != 3;
    for (unsigned i = 0; i < subList->ScrollingChildren.size(); ++i) {
        CComponent* item = subList->ScrollingChildren[i];
        long def = i < 2 ? 7 : 9;
        bad |= item->Actions.size() != 3 || item->Actions[0].ActionType != 23 || item->Actions[1].ActionType != 24 || item->Actions[2].ActionType != 2;
        bad |= item->Actions[2].ThingParams.size() != 1 || item->Actions[2].ThingParams[0] != def;
        bad |= item->Children[0]->Graphic != def * 10 || item->Actions[0].UIParams[0] != IntToString(def * 10);
        bad |= item->Actions[0].UIParams.size() != (parent ? 2u : 1u);
    }
    if (parent) bad |= parent->PositionChildren.size() != 2 + 3;
    for (unsigned i = 0; i < g_All.size(); ++i) delete g_All[i];
    if (bad) { std::printf("005c574e_TEST FAIL\n"); return 1; }
    std::printf("005c574e_TEST PASS\n");
    return 0;
}
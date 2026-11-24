// Behaviour test for 0x005907AB CTCInventory::ConstructItemCategoryListPC: caption total, the per-def merge of the item list,
// the empty-category path, the usable / can't-use actions list choice and the frame registrations.
#include <cstdio>
#include <cstring>

struct Item { long first; long second; };
struct MergedMap {
    long keys[8]; long values[8]; int n;
    int find(long key) const { for (int i = 0; i < n; ++i) if (keys[i] == key) return i; return -1; }
    void insert(long key, long value) { keys[n] = key; values[n] = value; ++n; }
};
struct Result {
    long captionTotal; bool hasCount; bool cleared; int frameMembers; int itemCount;
    const char* actionsName; bool notUsableCleared; MergedMap merged;
};
static Result Build(const Item* items, int count, bool templateNamed, bool checkUsable, bool hasCarrying, bool heldAlive)
{
    Result r; std::memset(&r, 0, sizeof(r));
    if (count != 0) {
        long total = 0;
        for (unsigned long i = 0; i < (unsigned long)count; ++i) total += items[i].second;
        r.captionTotal = total; r.hasCount = true;
    }
    ++r.frameMembers;                 // subtitle
    ++r.frameMembers;                 // description
    if (templateNamed) ++r.frameMembers;
    if (count == 0) {
        r.cleared = true;
        return r;
    }
    for (long i = 0; i < count; ++i) {
        const Item* item = &items[i];
        long key = item->first;
        int found = r.merged.find(key);
        if (found < 0) r.merged.insert(key, item->second);
        else r.merged.values[found] += item->second;
    }
    r.itemCount = r.merged.n;
    ++r.frameMembers;                 // sub-list title
    bool usable = true;
    if (checkUsable && hasCarrying && !heldAlive) usable = false;
    r.actionsName = usable ? "PC_UI_LIST_ITEMS_ACTIONS" : "PC_UI_LIST_ITEMS_ACTIONS_CANT_USE";
    if (!usable) r.notUsableCleared = true;
    ++r.frameMembers;                 // actions title
    return r;
}

int main()
{
    Item items[4] = { { 10, 2 }, { 11, 1 }, { 10, 3 }, { 12, 4 } };
    Result a = Build(items, 4, true, true, true, false);
    Result b = Build(items, 0, false, false, false, false);
    Result c = Build(items, 2, false, true, false, false);
    bool ok = a.hasCount && a.captionTotal == 10 && a.itemCount == 3 && a.merged.values[0] == 5 && a.merged.values[2] == 4
        && std::strcmp(a.actionsName, "PC_UI_LIST_ITEMS_ACTIONS_CANT_USE") == 0 && a.notUsableCleared && a.frameMembers == 5
        && !b.hasCount && b.cleared && b.frameMembers == 2 && b.actionsName == 0
        && std::strcmp(c.actionsName, "PC_UI_LIST_ITEMS_ACTIONS") == 0 && !c.notUsableCleared && c.itemCount == 2 && c.captionTotal == 3;
    if (!ok) { std::printf("005907ab_TEST FAIL\n"); return 1; }
    std::printf("005907ab_TEST PASS\n");
    return 0;
}
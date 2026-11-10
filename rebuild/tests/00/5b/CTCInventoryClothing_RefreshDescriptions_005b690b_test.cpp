// Behaviour test for 0x005B690B CTCInventoryClothing::RefreshDescriptions: nothing happens without a clothing list; otherwise every
// "CLOTHING TITLE" text gets the title string, then (the found list is appended to, not cleared) every component found so far plus
// every "Clothing_Description" one is looked up in the item map (absent keys insert 0) and those with a positive item index get
// that item's description; the title texts are walked again in the second pass and only re-texted when the map holds an index.
#include <cstdio>
#include <list>
#include <map>
#include <string>
#include <vector>

struct Comp { std::string name; std::string value; };
struct Clothing {
    std::vector<Comp*>* ClothingList;
    std::map<Comp*, long> ClothingItemMap;
    int titleCalls;
    void FindAllChildren(const std::string& name, std::list<Comp*>& out)
    {
        for (unsigned i = 0; i < ClothingList->size(); ++i)
            if ((*ClothingList)[i]->name == name) out.push_back((*ClothingList)[i]);
    }
    void ConstructTitleString(std::string& title) { ++titleCalls; title = "TITLE"; }
    void GetItemDescription(std::string& text, long itemDefIndex, bool) const
    {
        char buf[32]; std::sprintf(buf, "DESC%ld", itemDefIndex); text = buf;
    }
    void RefreshDescriptions()
    {
        if (ClothingList) {
            std::list<Comp*> components;
            std::list<Comp*>::iterator it, end;
            FindAllChildren("CLOTHING TITLE", components);
            for (it = components.begin(), end = components.end(); it != end; ++it) {
                std::string title; ConstructTitleString(title); (*it)->value = title;
            }
            FindAllChildren("Clothing_Description", components);
            for (it = components.begin(), end = components.end(); it != end; ++it) {
                Comp* component = *it;
                long itemDefIndex = ClothingItemMap[component];
                if (itemDefIndex > 0) {
                    std::string description; GetItemDescription(description, itemDefIndex, false); component->value = description;
                }
            }
        }
    }
};
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("FAIL %s\n", what); ++fails; } }
int main()
{
    {
        Clothing c; c.ClothingList = 0; c.titleCalls = 0;
        c.RefreshDescriptions();
        check(c.titleCalls == 0 && c.ClothingItemMap.empty(), "no list: no work");
    }
    {
        Comp t1 = { "CLOTHING TITLE", "" }, t2 = { "CLOTHING TITLE", "" }, d1 = { "Clothing_Description", "" },
             d2 = { "Clothing_Description", "old" }, d3 = { "Clothing_Description", "" }, o = { "OTHER", "x" };
        std::vector<Comp*> list; list.push_back(&t1); list.push_back(&d1); list.push_back(&o); list.push_back(&t2);
        list.push_back(&d2); list.push_back(&d3);
        Clothing c; c.ClothingList = &list; c.titleCalls = 0;
        c.ClothingItemMap[&d1] = 12; c.ClothingItemMap[&d2] = 0; c.ClothingItemMap[&d3] = -4; c.ClothingItemMap[&t2] = 7;
        c.RefreshDescriptions();
        check(c.titleCalls == 2, "two titles");
        check(t1.value == "TITLE", "title 1");
        check(t2.value == "DESC7", "title 2 also in the map: re-texted by the second pass");
        check(d1.value == "DESC12", "positive index described");
        check(d2.value == "old" && d3.value == "", "non-positive index untouched");
        check(o.value == "x", "other untouched");
        check(c.ClothingItemMap.count(&t1) == 1 && c.ClothingItemMap[&t1] == 0, "lookup inserts absent keys");
    }
    if (fails) return 1;
    std::printf("005b690b_TEST PASS\n");
    return 0;
}
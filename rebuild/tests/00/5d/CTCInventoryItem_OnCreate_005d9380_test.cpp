#include <cstdio>

class CInventoryItemDef { public: long id; };
struct CDef { CInventoryItemDef sub; int calls;
    void GetSubDef(CInventoryItemDef*& out) { ++calls; out = &sub; } };
class CThing { public: CDef* def; CDef* GetDef() { return def; } };
class CTCInventoryItem { public:
    CThing* slot; CInventoryItemDef* PDef;
    CThing* GetThing() { return slot; }
    void OnCreate() { GetThing()->GetDef()->GetSubDef(PDef); }
};
static bool Run()
{
    CDef d; d.sub.id = 77; d.calls = 0; CThing t; t.def = &d;
    CTCInventoryItem item; item.slot = &t; item.PDef = 0;
    item.OnCreate();
    return item.PDef == &d.sub && item.PDef->id == 77 && d.calls == 1;
}

int main()
{
    if (!Run()) { std::printf("005d9380_TEST FAIL\n"); return 1; }
    std::printf("005d9380_TEST PASS\n");
    return 0;
}
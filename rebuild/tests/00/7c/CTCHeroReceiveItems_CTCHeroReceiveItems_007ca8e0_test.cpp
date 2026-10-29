// Behaviour test for CTCHeroReceiveItems::~CTCHeroReceiveItems @ 0x007CA8E0: the component leaves the global TC list first,
// then the two primitive handles, the container, the current item, the picked-up vector and the item list go in reverse
// order, then the CTCBase part.
#include <stdio.h>
#include <string.h>
static char g_log[64];
static void Log(const char* s) { strcat(g_log, s); }
class CTCBase { public: bool IsInGlobalUpdateTCs; CTCBase() : IsInGlobalUpdateTCs(true) { }
    void RemoveFromGlobalTCs() { if (IsInGlobalUpdateTCs) Log("R"); IsInGlobalUpdateTCs = false; }
    virtual ~CTCBase() { Log("B"); } };
struct CMember { const char* Tag; CMember(const char* t) : Tag(t) { } ~CMember() { Log(Tag); } };
class CTCHeroReceiveItems : public CTCBase { public:
    CMember Items, PickedUpItems, PCurrentItem; bool SendMessageW; CMember PContainer, ItemPrimitiveHandle, BackgroundPrimitiveHandle;
    CTCHeroReceiveItems() : Items("l"), PickedUpItems("v"), PCurrentItem("c"), SendMessageW(false), PContainer("p"),
        ItemPrimitiveHandle("1"), BackgroundPrimitiveHandle("2") { }
    ~CTCHeroReceiveItems() { RemoveFromGlobalTCs(); }
};
int main()
{
    CTCHeroReceiveItems* r = new CTCHeroReceiveItems;
    delete r;
    if (strcmp(g_log, "R21pcvlB") != 0) { printf("007ca8e0_TEST FAIL\n"); return 1; }
    printf("007ca8e0_TEST PASS\n");
    return 0;
}
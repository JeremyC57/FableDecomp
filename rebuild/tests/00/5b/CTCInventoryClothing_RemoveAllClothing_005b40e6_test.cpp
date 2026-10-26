#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    char text[32];
    CCharString(const char* t, long length = -1) { std::strcpy(text, t); ++g_live; (void)length; }
    ~CCharString() { --g_live; }
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) { return std::strcmp(name.text, "HERO_SUIT_NAKED") == 0 ? 321 : -1; }
};
static CDefinitionManager g_dm;
class CThing { public: int id; };
struct Clothing {
    CThing* Owner; bool UseMannequin; long selected; CThing* dressed; bool flagWhileDressing; int liveWhileSelecting;
    CDefinitionManager* GetDefinitionManager() { return &g_dm; }
    void SelectItemWithDefIndex(long defIndex) { selected = defIndex; liveWhileSelecting = g_live; }
    void SetThingAsWearingSelectedClothes(CThing* thing) { dressed = thing; flagWhileDressing = UseMannequin; }
    void RemoveAllClothing()
    {
        UseMannequin = false;
        long suitIndex = GetDefinitionManager()->GetDefGlobalIndexFromName("HERO_SUIT_NAKED");
        SelectItemWithDefIndex(suitIndex);
        SetThingAsWearingSelectedClothes(Owner);
        UseMannequin = true;
    }
};
int main()
{
    CThing hero; Clothing c; c.Owner = &hero; c.UseMannequin = false; c.selected = 0; c.dressed = 0; c.flagWhileDressing = true; c.liveWhileSelecting = -1;
    c.RemoveAllClothing();
    if (c.selected != 321 || c.dressed != &hero || c.flagWhileDressing || !c.UseMannequin || c.liveWhileSelecting != 0 || g_live != 0)
    { std::printf("005b40e6_TEST FAIL\n"); return 1; }
    std::printf("005b40e6_TEST PASS\n");
    return 0;
}
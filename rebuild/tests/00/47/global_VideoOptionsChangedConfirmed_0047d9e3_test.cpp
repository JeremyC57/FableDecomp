// Behaviour test for 0x0047d9e3 VideoOptionsChangedConfirmed (model copy: player chain, alive test, interface 0xC6 lookup, menu call).
#include <cstdio>

struct CTCInGameMenuModel {
    int calls;
    void ConfirmedVideoOptionsChanged() { ++calls; }
};
struct SEntry { long type; CTCInGameMenuModel* component; };
struct CThingModel {
    bool alive;
    bool hasMenu;
    SEntry entries[3];
    long count;
    bool test(long kind) const { return kind == 0xC6 && hasMenu; }
    SEntry* LowerBound(long type)
    {
        SEntry* lo = entries;
        SEntry* hi = entries + count;
        while (lo < hi) { SEntry* mid = lo + (hi - lo) / 2; if (mid->type < type) lo = mid + 1; else hi = mid; }
        return lo;
    }
    bool GetInterface(CTCInGameMenuModel** ppOut, long interfaceType)
    {
        if (test(interfaceType)) {
            SEntry* last = entries + count;
            SEntry* it = LowerBound(interfaceType);
            *ppOut = ((it == last || interfaceType < it->type) ? last : it)->component;
            return true;
        }
        return false;
    }
};
static CThingModel* g_playerCharacter = 0;
static bool IS_THING_ALIVE(CThingModel* const& thing) { return thing != 0 && thing->alive; }

static void VideoOptionsChangedConfirmedModel()
{
    CThingModel* thing = g_playerCharacter;
    if (IS_THING_ALIVE(thing)) {
        CTCInGameMenuModel* menu = 0;
        thing->GetInterface(&menu, 0xC6);
        if (menu) menu->ConfirmedVideoOptionsChanged();        // retail calls through null when the menu is absent
    }
}

int main()
{
    CTCInGameMenuModel menu;
    menu.calls = 0;
    CThingModel t;
    t.alive = true;
    t.hasMenu = true;
    t.count = 3;
    t.entries[0].type = 2;    t.entries[0].component = 0;
    t.entries[1].type = 0xC6; t.entries[1].component = &menu;
    t.entries[2].type = 0x200; t.entries[2].component = 0;
    t.entries[0].component = 0;
    g_playerCharacter = &t;
    VideoOptionsChangedConfirmedModel();
    int ok = menu.calls == 1;
    t.alive = false;
    VideoOptionsChangedConfirmedModel();
    if (menu.calls != 1) ok = 0;
    g_playerCharacter = 0;
    VideoOptionsChangedConfirmedModel();
    if (menu.calls != 1) ok = 0;
    if (!ok) { std::printf("0047d9e3_TEST FAIL\n"); return 1; }
    std::printf("0047d9e3_TEST PASS\n");
    return 0;
}
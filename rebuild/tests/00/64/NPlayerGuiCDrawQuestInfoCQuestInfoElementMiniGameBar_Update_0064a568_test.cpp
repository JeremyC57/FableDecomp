// Behaviour test for CQuestInfoElementMiniGameBar::Update (0x0064A568): always true; only an updatable bar with a
// living hero that has an inventory looks at the button, asking about EXPRESSION_PICKPOCKET (type 3) or
// EXPRESSION_PICKLOCK (type 4), and stops updating once the button is no longer held; other types are ignored.
#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* Text;
    CCharString(const char* t, long = -1) : Text(t) {}
};
static const char* g_asked;
static bool g_held;
class CTCInventory { public:
    bool IsButtonHeldForExpression(const CCharString& e) { g_asked = e.Text; return g_held; }
};
class CThing { public:
    bool Dead; CTCInventory* Inventory;
    template<class T> bool GetInterface(T** out, long type) const
    {
        if (type == 0x11 && Inventory) { *out = (T*)Inventory; return true; }
        return false;
    }
};
template<class T> bool IS_THING_ALIVE(const T& thing) { return thing && !thing->Dead; }
static CThing* g_hero;
class CPlayer { public: CThing* GetHeroThing() const { return g_hero; } };
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() { return &g_player; } };
class CMainGameComponent { public: CPlayerManager* PlayerManager; };
static CPlayerManager g_pm;
static CMainGameComponent g_mgc = { &g_pm };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CQuestInfoElementMiniGameBar {
    long Type; bool Updatable;
    bool Update();
};
bool CQuestInfoElementMiniGameBar::Update()
{
    if (Updatable) {
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        CTCInventory* inventory;
        if (IS_THING_ALIVE<CThing*>(hero) && hero->GetInterface(&inventory, 0x11)) {
            switch (Type) {
            case 3:
                if (inventory->IsButtonHeldForExpression("EXPRESSION_PICKPOCKET") ? false : true)
                    Updatable = false;
                break;
            case 4:
                if (inventory->IsButtonHeldForExpression("EXPRESSION_PICKLOCK") ? false : true)
                    Updatable = false;
                break;
            }
        }
    }
    return true;
}
int main()
{
    CTCInventory inv;
    CThing hero = { false, &inv };
    CQuestInfoElementMiniGameBar bar = { 3, true };
    int bad = 0;
    g_hero = &hero; g_held = true;
    if (!bar.Update() || !bar.Updatable || std::strcmp(g_asked, "EXPRESSION_PICKPOCKET")) bad |= 1;
    g_held = false;
    if (!bar.Update() || bar.Updatable) bad |= 2;
    bar.Type = 4; bar.Updatable = true; g_asked = 0;
    if (!bar.Update() || bar.Updatable || std::strcmp(g_asked, "EXPRESSION_PICKLOCK")) bad |= 4;
    bar.Type = 5; bar.Updatable = true; g_asked = 0;
    if (!bar.Update() || !bar.Updatable || g_asked) bad |= 8;
    bar.Type = 3; hero.Dead = true;
    if (!bar.Update() || !bar.Updatable || g_asked) bad |= 16;
    hero.Dead = false; hero.Inventory = 0;
    if (!bar.Update() || !bar.Updatable || g_asked) bad |= 32;
    g_hero = 0;
    if (!bar.Update() || !bar.Updatable) bad |= 64;
    bar.Updatable = false; hero.Inventory = &inv; g_hero = &hero;
    if (!bar.Update() || bar.Updatable || g_asked) bad |= 128;
    if (bad) { std::printf("0064a568_TEST FAIL\n"); return 1; }
    std::printf("0064a568_TEST PASS\n");
    return 0;
}
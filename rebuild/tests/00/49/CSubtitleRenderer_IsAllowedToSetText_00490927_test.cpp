// CSubtitleRenderer::IsAllowedToSetText(bool force) -- subtitles may be set only while the game is running, not over a blocking front-end state (unless forced), outside the world's special mode, with no hero-information screen up, and while the living hero has neither of two components (0xFC, 0xC6) in their active state
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: IsAllowedToSetText
// Retail: Fable.exe (TLC, VC7.1) @ 0x00490927, 251 bytes, `ret 4`; `this` is never read.
// Callees: a CWorld mode predicate 0x0048DB13 (`mode != 0 && mode != 9` on +0x104; emitted in this TU; manifest label
//   CPersistContext::IsBinary is propagated), CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0,
//   CWorld::GetCurrentPlayer() const 0x0049D9D0, CPlayer::GetControlledCreature 0x00487DD0,
//   IS_THING_ALIVE<CThing*> 0x0043DBF0 (__fastcall), std::bitset::test 0x00410DE0,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0.
// Globals: g_pMainGameComponent 0x013B86A0 (+0x24 PWorld, +0x28 a front-end object with a blocking byte at +0xBC, a
//   running byte at +0x161E2), g_pPlayerGui 0x013B8790.
// Tried: flat early returns (pushes sunk into the branches, one `xor al,al` per depth), speed TU (byte loads into
//   registers), bool flags (zero register in bl); this do/while(0) + break form gives the single shared exit.
// Shape: a frame-pointer size TU (the CSubtitleRenderer siblings' `optimize("y", off)` + `("s", on)`); every failed
//   test jumps to the shared `xor al,al` exit, the fall-through is `mov al,1`. The alive test binds the hero through a
//   converted temporary homed in the dead `force` slot (the interface keys reuse it); each component lookup is the
//   inline presence test + the ternary entry pick of the landed 0x0064B5C1 idiom, the type kept in edi. The two
//   component checks are one `||` of a __forceinline IsComponentActive<T>(thing, type) helper followed by
//   `return true`: that lays `mov al,1 ; jmp` before the shared `xor al,al` (two inline ifs put the false exit first).
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
template <class T> __forceinline bool IsComponentActive(CThing* thing, long type)
{
    if (thing->HasInterface(type)) {
        T* component = thing->GetInterfaceUnchecked<T>(type);
        if (component && component->m_active != 0)
            return true;
    }
    return false;
}
struct CTCActiveStateA {                           // interface 0xFC
    unsigned char _pad[0xd];
    unsigned char m_active;                        // +0x0D
};
struct CTCActiveStateB {                           // interface 0xC6
    unsigned char _pad[0xc];
    unsigned char m_active;                        // +0x0C
};
struct CThingCreatureBase : CThing {
};
struct CPlayer {
    CThingCreatureBase* GetControlledCreature();   // 0x00487DD0
};
struct CWorld {
    bool IsInSpecialMode();                        // 0x0048DB13
    CPlayer* GetCurrentPlayer() const;             // 0x0049D9D0
};
struct CFrontEnd {
    unsigned char _pad[0xbc];
    unsigned char m_blocking;                      // +0xBC
};
struct CMainGameComponent {
    unsigned char _pad0[0x24];
    CWorld*    PWorld;                             // +0x24
    CFrontEnd* m_pFrontEnd;                        // +0x28
    unsigned char _pad2[0x161e2 - 0x2c];
    unsigned char m_running;                       // +0x161E2
};
struct CHeroInfoScreensManager {
    unsigned char _pad[0x34];
    unsigned char m_open;                          // +0x34
};
struct CPlayerGui {
    CHeroInfoScreensManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790

class CSubtitleRenderer {
public:
    bool IsAllowedToSetText(bool force);
};

bool CSubtitleRenderer::IsAllowedToSetText(bool force)
{
    do {
        CMainGameComponent* game = g_pMainGameComponent;
        if (game->m_running == 0)
            break;
        if (game->m_pFrontEnd->m_blocking != 0 && !force)
            break;
        CWorld* world = game->PWorld;
        if (world->IsInSpecialMode())
            break;
        if (g_pPlayerGui->GetPHeroInfoScreensManager()->m_open != 0)
            break;
        CThingCreatureBase* hero = world->GetCurrentPlayer()->GetControlledCreature();
        if (!IS_THING_ALIVE<CThing*>(hero))
            break;
        if (IsComponentActive<CTCActiveStateA>(hero, 0xfc) || IsComponentActive<CTCActiveStateB>(hero, 0xc6))
            break;
        return true;
    } while (0);
    return false;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool g_present[0x200];
bool CPresenceBits::test(long bit) { return g_present[bit]; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{ SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
static bool g_alive = true;
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing != 0 && g_alive; }
static CThingCreatureBase* g_hero = 0;
CThingCreatureBase* CPlayer::GetControlledCreature() { return g_hero; }
static bool g_special = false; static CPlayer g_player;
bool CWorld::IsInSpecialMode() { return g_special; }
CPlayer* CWorld::GetCurrentPlayer() const { return &g_player; }
static CHeroInfoScreensManager g_screens;
CHeroInfoScreensManager* CPlayerGui::GetPHeroInfoScreensManager() const { return &g_screens; }
CMainGameComponent* g_pMainGameComponent = 0;
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;

int main()
{
    static CMainGameComponent game; static CFrontEnd fe; static CWorld world; static CThingCreatureBase hero;
    static CTCActiveStateA compA; static CTCActiveStateB compB;
    memset(&game, 0, sizeof(game)); memset(&fe, 0, sizeof(fe)); memset(&hero, 0, sizeof(hero));
    memset(&compA, 0, sizeof(compA)); memset(&compB, 0, sizeof(compB)); memset(&g_screens, 0, sizeof(g_screens));
    static CInterfaceMap::SEntry entries[2] = { { 0xc6, (CTCBase*)&compB }, { 0xfc, (CTCBase*)&compA } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    game.PWorld = &world; game.m_pFrontEnd = &fe; g_pMainGameComponent = &game; g_hero = &hero;
    CSubtitleRenderer r;
    CHECK(!r.IsAllowedToSetText(false));                  // not running
    game.m_running = 1;
    CHECK(r.IsAllowedToSetText(false));
    fe.m_blocking = 1;
    CHECK(!r.IsAllowedToSetText(false));
    CHECK(r.IsAllowedToSetText(true));
    fe.m_blocking = 0; g_special = true;
    CHECK(!r.IsAllowedToSetText(true));
    g_special = false; g_screens.m_open = 1;
    CHECK(!r.IsAllowedToSetText(false));
    g_screens.m_open = 0; g_alive = false;
    CHECK(!r.IsAllowedToSetText(false));
    g_alive = true; g_hero = 0;
    CHECK(!r.IsAllowedToSetText(false));
    g_hero = &hero;
    compA.m_active = 1;                                    // not present yet: ignored
    CHECK(r.IsAllowedToSetText(false));
    g_present[0xfc] = true;
    CHECK(!r.IsAllowedToSetText(false));
    compA.m_active = 0;
    CHECK(r.IsAllowedToSetText(false));
    g_present[0xc6] = true; compB.m_active = 2;
    CHECK(!r.IsAllowedToSetText(false));
    compB.m_active = 0;
    CHECK(r.IsAllowedToSetText(false));
    hero.m_interfaces.last = entries + 1;                  // 0xfc lookup misses: entry pick = last (null component)
    entries[1].component = 0;
    CHECK(r.IsAllowedToSetText(false));
    if (fails == 0) printf("00490927_TEST PASS\n");
    return fails;
}
// 00490a22 behaviour test: CSubtitleRenderer::IsInInteractiveState (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
struct CTCBase;
struct CPresenceBits {
    unsigned long Bits[9];
    bool test(long bit) { return (Bits[bit >> 5] >> (bit & 31)) & 1; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
    {
        SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
    SEntry* LowerBound(const long* type) { return ((const CInterfaceMap*)this)->LowerBound(type); }
};
struct CThing {
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    bool Alive;
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> T* GetInterfaceConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && thing->Alive; }
struct CTCA { unsigned char m_c; unsigned char m_d; unsigned char m_1c; unsigned char m_c0; };
struct CTCB { unsigned char m_c; };
struct CThingCreatureBase : CThing {};
static CThingCreatureBase* g_controlled;
static CThingCreatureBase* g_hero;
struct CPlayer {
    CThingCreatureBase* GetControlledCreature() { return g_controlled; }
    CThingCreatureBase* GetHeroCreature() { return g_hero; }
};
static CPlayer g_player;
struct CPlayerManager { CPlayer* GetMainPlayer() { return &g_player; } };
static bool g_frontEndActive;
struct CFrontEnd { bool IsActive() { return g_frontEndActive; } };
struct CMainGameComponent { CPlayerManager* PPlayerManager; CFrontEnd* m_pFrontEnd; };
struct CHeroInfoScreensManager { unsigned char m_open; };
static CHeroInfoScreensManager g_screens;
struct CPlayerGui { CHeroInfoScreensManager* GetPHeroInfoScreensManager() const { return &g_screens; } };
static CPlayerManager g_pm; static CFrontEnd g_fe;
static CMainGameComponent g_mgc = { &g_pm, &g_fe };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
static bool g_fullyShown, g_waiting;
class CSubtitleRenderer {
public:
    unsigned long DisplayFlags;
    bool AllCharactersDisplayed;
    bool InCutscene;
    bool DisplayingText;
    bool IsWaitingForClickPast() const { return g_waiting; }
    bool IsTextFullyShown() const { return g_fullyShown; }
    bool IsInInteractiveState();
};
bool CSubtitleRenderer::IsInInteractiveState()
{
    unsigned long flags = DisplayFlags;
    if (!(flags & 0x4c)) {
        if (InCutscene)
            return false;
        if (DisplayingText) {
            if (!((flags & 0x20) && (flags & 2) && IsTextFullyShown() && AllCharactersDisplayed)) {
                if (!(IsWaitingForClickPast() && IsTextFullyShown() && AllCharactersDisplayed))
                    return false;
            }
        } else {
            CThingCreatureBase* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetControlledCreature();
            bool busy = false;
            if (IS_THING_ALIVE<CThing*>(hero) && hero->HasInterface(0xfc)) {
                CTCA* c = hero->GetInterfaceConst<CTCA>(0xfc);
                if (c->m_c && !c->m_1c && !c->m_c0)
                    busy = true;
            }
            if (!busy) {
                CTCA* a = 0;
                CThingCreatureBase* h2 = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetHeroCreature();
                if (h2->HasInterface(0xfc))
                    a = h2->GetTC<CTCA>(0xfc);
                CTCB* b = 0;
                CThingCreatureBase* h3 = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetHeroCreature();
                if (h3->HasInterface(0xc6))
                    b = h3->GetTC<CTCB>(0xc6);
                return g_pMainGameComponent->m_pFrontEnd->IsActive() || (a && a->m_d) || (b && b->m_c)
                    || g_pPlayerGui->GetPHeroInfoScreensManager()->m_open;
            }
        }
    }
    return true;
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }
static void SetTC(CThing& t, CInterfaceMap::SEntry* e, int n)
{
    memset(&t.m_present, 0, sizeof(t.m_present));
    for (int i = 0; i < n; ++i) t.m_present.Bits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t.m_interfaces.first = e; t.m_interfaces.last = e + n;
}
int main()
{
    CTCA ctrlA = { 0, 0, 0, 0 }, heroA = { 0, 0, 0, 0 };
    CTCB heroB = { 0 };
    CThingCreatureBase ctrl, hero;
    CInterfaceMap::SEntry ce[1] = { { 0xfc, (CTCBase*)&ctrlA } };
    CInterfaceMap::SEntry he[2] = { { 0xc6, (CTCBase*)&heroB }, { 0xfc, (CTCBase*)&heroA } };
    SetTC(ctrl, ce, 1); SetTC(hero, he, 2);
    ctrl.Alive = true; hero.Alive = true;
    g_controlled = &ctrl; g_hero = &hero;
    CSubtitleRenderer r; r.DisplayFlags = 0; r.AllCharactersDisplayed = false; r.InCutscene = false; r.DisplayingText = false;

    Check("idle", !r.IsInInteractiveState());
    r.DisplayFlags = 0x40; Check("flag 0x40", r.IsInInteractiveState());
    r.DisplayFlags = 0x04; Check("flag 0x04", r.IsInInteractiveState());
    r.DisplayFlags = 0; r.InCutscene = true; Check("cutscene", !r.IsInInteractiveState());
    r.InCutscene = false;
    // text path
    r.DisplayingText = true;
    Check("text not shown", !r.IsInInteractiveState());
    r.DisplayFlags = 0x22; g_fullyShown = true; r.AllCharactersDisplayed = true;
    Check("text click flags", r.IsInInteractiveState());
    r.DisplayFlags = 0; Check("text no flags", !r.IsInInteractiveState());
    g_waiting = true; Check("text waiting", r.IsInInteractiveState());
    r.AllCharactersDisplayed = false; Check("text waiting partial", !r.IsInInteractiveState());
    g_waiting = false; g_fullyShown = false; r.DisplayingText = false;
    // busy controlled creature component
    ctrlA.m_c = 1; Check("busy", r.IsInInteractiveState());
    ctrlA.m_1c = 1; Check("busy masked 1c", !r.IsInInteractiveState());
    ctrlA.m_1c = 0; ctrlA.m_c0 = 1; Check("busy masked c0", !r.IsInInteractiveState());
    ctrlA.m_c0 = 0; ctrl.Alive = false; Check("dead not busy", !r.IsInInteractiveState());
    ctrl.Alive = true; ctrlA.m_c = 0;
    // lookups
    heroA.m_d = 1; Check("hero a", r.IsInInteractiveState()); heroA.m_d = 0;
    heroB.m_c = 1; Check("hero b", r.IsInInteractiveState()); heroB.m_c = 0;
    g_frontEndActive = true; Check("front end", r.IsInInteractiveState()); g_frontEndActive = false;
    g_screens.m_open = 1; Check("screens", r.IsInInteractiveState()); g_screens.m_open = 0;
    // hero lacking the interfaces
    heroA.m_d = 1; heroB.m_c = 1; SetTC(hero, he, 0);
    Check("no hero tcs", !r.IsInInteractiveState());
    if (g_fail) return 1;
    printf("00490a22_TEST PASS\n");
    return 0;
}
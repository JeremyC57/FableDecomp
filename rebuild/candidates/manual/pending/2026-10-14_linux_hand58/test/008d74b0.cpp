#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_base = 0, g_ctor = 0, g_dtor = 0;
struct CThing { int x; };
static CThing g_hero;
struct CBaseIP { virtual ~CBaseIP() { ++g_dtor; } void* item; CBaseIP() : item(0) { ++g_ctor; } void* GetPItem() const { return item; } void SetPItem(void* p) { item = p; } };
template<class T> struct CIP : CBaseIP { CIP(T* p) : CBaseIP() { SetPItem(p); } CIP& operator=(const CIP& o) { SetPItem(o.GetPItem()); return *this; } };
struct CPlayer { CThing* GetHeroThing() { return &g_hero; } };
struct CPlayerManager { CPlayer p; CPlayer* GetPlayer() { return &p; } };
struct CGame { unsigned char pad[0x1c]; CPlayerManager* pm; };
static CGame* g_pGame;
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CModeStandBase : CModeBase { void OnActivate() { ++g_base; } };
struct CMode : CModeStandBase { CIP<CThing> target; CMode() : target(0) { } virtual void OnActivate() { CModeStandBase::OnActivate(); target = g_pGame->pm->GetPlayer()->GetHeroThing(); } };
int main()
{
    CPlayerManager pm; CGame game; game.pm = &pm; g_pGame = &game;
    CMode m; g_ctor = 0; g_dtor = 0; m.OnActivate();
    if (g_base != 1 || m.target.GetPItem() != &g_hero || g_ctor != 1 || g_dtor != 1) { std::printf("008d74b0_TEST FAIL\n"); return 1; }
    std::printf("008d74b0_TEST PASS\n");
    return 0;
}

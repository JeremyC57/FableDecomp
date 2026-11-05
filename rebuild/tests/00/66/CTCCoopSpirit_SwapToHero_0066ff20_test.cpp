// Behaviour test for 0x0066FF20 CTCCoopSpirit::SwapToHero (plain C++, links the row object).
#include <stdio.h>
#include <string.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); }
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* PStringData; explicit CCharString(long v) : PStringData((void*)v) { } ~CCharString(); };
CCharString::~CCharString() { Log("~S;"); }
class CTCPhysicsBase { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual void v17() {}
    virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {} virtual void v29() {}
    virtual void v30() {}
    virtual void ResetPosition(const C3DVector& p) { Log("Reset;"); Got = p; }
    C3DVector Got;
};
class CThing { public:
    unsigned char Pad0[0x60]; CTCPhysicsBase* PhysicsTC; unsigned char Pad64[0x2c]; signed char PlayerNumber;
    unsigned char Pad91[0x1f]; float MaxHealth; float Health;
    C3DVector Pos;
    const C3DVector& GetPos() const;
};
const C3DVector& CThing::GetPos() const { Log("GetPos;"); return Pos; }
class CThingPlayerCreature : public CThing { };
static CThingPlayerCreature g_hero;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { Log("Creature;"); return &g_hero; }
static CPlayer g_player;
static long g_swapped = -99, g_initPlayer = -99, g_getPlayer = -99;
class CPlayerManager { public:
    long GetMainPlayerNumber() const; void SwapToHero(long n); CCharString GetSpiritDefName(long n);
    void InitialisePlayerCharacterAs(long n, const CCharString& s); CPlayer& GetPlayer(long n);
};
long CPlayerManager::GetMainPlayerNumber() const { Log("Main;"); return 2; }
void CPlayerManager::SwapToHero(long n) { Log("Swap;"); g_swapped = n; }
CCharString CPlayerManager::GetSpiritDefName(long n) { Log("Name;"); return CCharString(n + 100); }
void CPlayerManager::InitialisePlayerCharacterAs(long n, const CCharString& s) { Log("Init;"); g_initPlayer = n + 1000 * ((long)s.PStringData == 102); }
CPlayer& CPlayerManager::GetPlayer(long n) { Log("Player;"); g_getPlayer = n; return g_player; }
static CPlayerManager g_manager;
class CTCBase { public:
    void* VTable; CThing* ParentThing; bool ToBeRemoved;
protected:
    CPlayerManager& GetPlayerManager();
};
CPlayerManager& CTCBase::GetPlayerManager() { return g_manager; }
class CTCCoopSpirit : public CTCBase {
    void SwapToHero();
public:
    static void Run(CTCCoopSpirit& spirit) { spirit.SwapToHero(); }
};
int main()
{
    CTCPhysicsBase physics; physics.Got.X = physics.Got.Y = physics.Got.Z = 0;
    g_hero.PhysicsTC = &physics;
    CThing spirit; spirit.PlayerNumber = -3; spirit.MaxHealth = 75.0f; spirit.Health = 5.0f;
    spirit.Pos.X = 1.5f; spirit.Pos.Y = -2.0f; spirit.Pos.Z = 8.25f;
    CTCCoopSpirit tc; tc.VTable = 0; tc.ParentThing = &spirit; tc.ToBeRemoved = false;
    CTCCoopSpirit::Run(tc);
    if (strcmp(g_log, "Main;GetPos;Swap;Name;Init;~S;Player;Creature;Reset;") != 0) { printf("FAIL order %s\n", g_log); return 1; }
    if (g_swapped != -3 || g_initPlayer != 1002 || g_getPlayer != 2) { printf("FAIL args\n"); return 1; }
    if (physics.Got.X != 1.5f || physics.Got.Y != -2.0f || physics.Got.Z != 8.25f) { printf("FAIL pos\n"); return 1; }
    if (spirit.Health != 75.0f || spirit.MaxHealth != 75.0f) { printf("FAIL health\n"); return 1; }
    printf("COOP_SPIRIT_SWAP_TO_HERO_PASS\n");
    return 0;
}

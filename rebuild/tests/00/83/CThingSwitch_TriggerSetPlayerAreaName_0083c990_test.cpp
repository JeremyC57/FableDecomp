// Behaviour test for 0083c990: final.cpp copied verbatim, the declared callees stubbed below so the switch's trigger
// square, the hero look-up, the box test argument, the interface look-up and the area-name call are observable.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CThingSwitch::TriggerSetPlayerAreaName() -- once: when the living hero stands inside the switch's trigger square and has a region display, show the switch's script name as the current area name
// Leaf: TriggerSetPlayerAreaName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083C990, length 255 bytes (+ int3 padding).
// Callees: CThing::GetPosition() const @ 0x006A4D00 (four times), the player-manager accessor @ 0x004C7A00,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0, C2DBoxF::ContainsPoint(const
// C2DVector&) const @ 0x00535CD0 (out of line), CVectorMap::LowerBound @ 0x0040F020 (interface 0x8e = CTCRegionDisplay,
// bit 14 of +0x30), CTCRegionDisplay::SetCurrentAreaName(const CCharString&, float, bool, bool, bool) @ 0x00639BA0.
// Layout (retail): CThing ScriptName +0x74, PhysicsTC +0x60 (position +0x0c), interface bitset +0x20 / map +0x44, Dead
// bit +0x91; CThingSwitch TriggerRadius +0xd0, the area-name-shown flag +0xdc (Triggered in Ego_r.pdb at that offset).
// Shape: the sibling TriggerSetEnvironment 0x0083BE90 with the box test out of line (the C2DVector conversion
// temporary in the frame, passed by reference); the flag is set between the argument pushes and the call.
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    bool ContainsPoint(const C2DVector& p) const;              // 0x00535CD0
};
class CCharString { public: void* PStringData; };
class CTCBase;
class CTCRegionDisplay {
public:
    void SetCurrentAreaName(const CCharString& name, float time, bool a, bool b, bool c);   // 0x00639BA0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                                    // +0x0c
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                          // +0x20
    CInterfaceMap   m_interfaces;                              // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                                 // +0x60
    unsigned char   _pad64[0x74 - 0x64];
    CCharString     ScriptName;                                // +0x74
    unsigned char   _pad78[0x91 - 0x78];
    unsigned char   Dead : 1;                                  // +0x91 bit 0
    const C3DVector& GetPhysicsPosition() const { return PhysicsTC->Position; }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CPlayer {
public:
    CThing* GetHeroCreature();                                 // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                                  // 0x00449970
};
class CThingSwitch : public CThing {
public:
    unsigned char _pad94[0xd0 - 0x94];
    float         TriggerRadius;                               // +0xd0
    CCharString   EnvironmentDef;                              // +0xd4
    float         TimeToChangeEnvironmentDef;                  // +0xd8
    bool          AreaNameShown;                               // +0xdc
    const C3DVector& GetPosition() const;                      // 0x006A4D00
    CPlayerManager* GetPlayerManager();                        // 0x004C7A00
    void TriggerSetPlayerAreaName();
};

void CThingSwitch::TriggerSetPlayerAreaName()
{
    if (AreaNameShown)
        return;
    C2DBoxF area;
    area.TLX = GetPosition().X - TriggerRadius;
    area.TLY = GetPosition().Y - TriggerRadius;
    area.BRX = GetPosition().X + TriggerRadius;
    area.BRY = GetPosition().Y + TriggerRadius;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    if (hero && !hero->Dead) {
        const C3DVector& pos = hero->GetPhysicsPosition();
        if (area.ContainsPoint(C2DVector(pos)) && hero->HasTC(0x8e)) {
        CTCRegionDisplay* display = (CTCRegionDisplay*)hero->GetTC(0x8e);
        AreaNameShown = true;
        display->SetCurrentAreaName(ScriptName, -1.0f, false, false, true);
        }
    }
}

// ---- stubs ----
static C3DVector g_switchPos = { 10.0f, 20.0f, 0.0f };
static CPlayerManager g_playerManager;
static CPlayer g_player;
static CThing* g_hero = 0;
static CInterfaceMap::SEntry g_entries[2];
static CTCRegionDisplay g_display;
static bool g_contains = false;
static float g_boxTLX, g_boxTLY, g_boxBRX, g_boxBRY;
static char g_buf[256];

const C3DVector& CThingSwitch::GetPosition() const { note("GetPosition"); return g_switchPos; }
CPlayerManager* CThingSwitch::GetPlayerManager() { note("GetPlayerManager"); return &g_playerManager; }
CPlayer* CPlayerManager::GetMainPlayer() { note("GetMainPlayer"); return &g_player; }
CThing* CPlayer::GetHeroCreature() { note("GetHeroCreature"); return g_hero; }
bool C2DBoxF::ContainsPoint(const C2DVector& p) const
{
    g_boxTLX = TLX; g_boxTLY = TLY; g_boxBRX = BRX; g_boxBRY = BRY;
    _snprintf(g_buf, sizeof(g_buf), "ContainsPoint(%g,%g)[%g,%g,%g,%g]", p.X, p.Y, TLX, TLY, BRX, BRY);
    note(g_buf);
    return g_contains;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    _snprintf(g_buf, sizeof(g_buf), "LowerBound(%ld)", *type);
    note(g_buf);
    for (SEntry* it = first; it != last; ++it)
        if (it->type >= *type)
            return it;
    return last;
}
void CTCRegionDisplay::SetCurrentAreaName(const CCharString& name, float time, bool a, bool b, bool c)
{
    _snprintf(g_buf, sizeof(g_buf), "SetCurrentAreaName(%s,%g,%d,%d,%d)", (const char*)name.PStringData, time, (int)a, (int)b, (int)c);
    note(g_buf);
}

static CThingSwitch* MakeSwitch()
{
    CThingSwitch* s = (CThingSwitch*)calloc(1, sizeof(CThingSwitch));
    s->TriggerRadius = 2.5f;
    s->ScriptName.PStringData = (void*)"SwitchArea";
    return s;
}
static CThing* MakeHero(float x, float y, bool dead, bool hasRegionDisplay)
{
    CThing* h = (CThing*)calloc(1, sizeof(CThing));
    CTCPhysicsBase* phys = (CTCPhysicsBase*)calloc(1, sizeof(CTCPhysicsBase));
    phys->Position.X = x; phys->Position.Y = y; phys->Position.Z = 7.0f;
    h->PhysicsTC = phys;
    h->Dead = dead ? 1 : 0;
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x8e; g_entries[1].component = (CTCBase*)&g_display;
    h->m_interfaces.first = g_entries;
    if (hasRegionDisplay) {
        h->m_presentBits[0x8e >> 5] |= 1UL << (0x8e & 31);
        h->m_interfaces.last = g_entries + 2;
    } else {
        h->m_interfaces.last = g_entries + 1;
    }
    return h;
}

int main()
{
    // 1. already shown: nothing happens
    CThingSwitch* s = MakeSwitch();
    s->AreaNameShown = true;
    g_log[0] = 0;
    s->TriggerSetPlayerAreaName();
    CHECKLOG("");
    CHECK(s->AreaNameShown);

    // 2. no hero
    s->AreaNameShown = false; g_hero = 0; g_log[0] = 0;
    s->TriggerSetPlayerAreaName();
    CHECKLOG("GetPosition;GetPosition;GetPosition;GetPosition;GetPlayerManager;GetMainPlayer;GetHeroCreature;");
    CHECK(!s->AreaNameShown);

    // 3. dead hero: no box test
    g_hero = MakeHero(11.0f, 21.0f, true, true); g_contains = true; g_log[0] = 0;
    s->TriggerSetPlayerAreaName();
    CHECKLOG("GetPosition;GetPosition;GetPosition;GetPosition;GetPlayerManager;GetMainPlayer;GetHeroCreature;");
    CHECK(!s->AreaNameShown);

    // 4. living hero outside the square: box built from position +/- radius, hero's X/Y passed, no interface look-up
    g_hero = MakeHero(11.0f, 21.0f, false, true); g_contains = false; g_log[0] = 0;
    s->TriggerSetPlayerAreaName();
    CHECKLOG("GetPosition;GetPosition;GetPosition;GetPosition;GetPlayerManager;GetMainPlayer;GetHeroCreature;ContainsPoint(11,21)[7.5,17.5,12.5,22.5];");
    CHECK(g_boxTLX == 7.5f && g_boxTLY == 17.5f && g_boxBRX == 12.5f && g_boxBRY == 22.5f);
    CHECK(!s->AreaNameShown);

    // 5. inside, but no region display interface
    g_hero = MakeHero(9.0f, 19.0f, false, false); g_contains = true; g_log[0] = 0;
    s->TriggerSetPlayerAreaName();
    CHECKLOG("GetPosition;GetPosition;GetPosition;GetPosition;GetPlayerManager;GetMainPlayer;GetHeroCreature;ContainsPoint(9,19)[7.5,17.5,12.5,22.5];");
    CHECK(!s->AreaNameShown);

    // 6. inside with a region display: flag set, name shown with -1 time and (false,false,true)
    g_hero = MakeHero(9.0f, 19.0f, false, true); g_contains = true; g_log[0] = 0;
    s->TriggerSetPlayerAreaName();
    CHECKLOG("GetPosition;GetPosition;GetPosition;GetPosition;GetPlayerManager;GetMainPlayer;GetHeroCreature;ContainsPoint(9,19)[7.5,17.5,12.5,22.5];LowerBound(142);SetCurrentAreaName(SwitchArea,-1,0,0,1);");
    CHECK(s->AreaNameShown);

    // 7. once only: the second call returns at the flag
    g_log[0] = 0;
    s->TriggerSetPlayerAreaName();
    CHECKLOG("");

    if (fails == 0) printf("0083c990_TEST PASS\n");
    else printf("0083c990_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}
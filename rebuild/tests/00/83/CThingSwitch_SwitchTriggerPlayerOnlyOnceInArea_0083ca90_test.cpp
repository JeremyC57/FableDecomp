#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingSwitch::SwitchTriggerPlayerOnlyOnceInArea() -- one-shot area switch: unless already triggered, for each initialised player whose hero (alive or not: no Dead test here) is inside the trigger square count the trigger, latch Triggered and queue a CEventTriggered for the hero (component 0x42)
// Leaf: SwitchTriggerPlayerOnlyOnceInArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083CA90, length 369 bytes (plain `ret`).
// Callees: as the sibling SwitchTriggerPlayerOnlyMultipleTimesInArea 0x0083C690 (same TU): CThing::GetPosition() const
//   @ 0x006A4D00, player-manager accessors @ 0x004C79F0 (const) / 0x004C7A00, CPlayerManager::GetPlayer(long) const
//   @ 0x00449910 / non-const @ 0x004498C0, CInitBaseClass::IsInitialised() const @ 0x0099A330, CPlayer::GetHeroCreature
//   @ 0x00487DC0, LowerBound @ 0x0040F020, operator new @ 0x00BFEA1A, NEntityEvents::CEventTriggered(CThing*)
//   @ 0x0083CF20, CTCEntityEvents::AddEvent @ 0x008113C0.
// Layout (retail): as the sibling, plus Triggered +0xdc (Ego_r.pdb +0xf4, CThingPhysical 0x18 lower).
// Shape: the early `if (Triggered) return;` keeps the ebx/ebp/edi pushes below the test; the loop does not stop after
//   a trigger (every hero inside in that frame queues an event).
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    __forceinline bool ContainsPoint(const C2DVector& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};

struct CEventBase { virtual void v00(); };
class CThing;
namespace NEntityEvents {
class CEventTriggered : public CEventBase { public:
    unsigned char _body[0x10 - 4];
    CEventTriggered(CThing* triggerer);                 // 0x0083CF20
};
}
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);                   // 0x008113C0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CPlayer { public:
    bool IsInitialised() const;                         // 0x0099A330 (CInitBaseClass)
    CThing* GetHeroCreature();                          // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPlayer(long number);                    // 0x004498C0
    const CPlayer* GetPlayer(long number) const;        // 0x00449910
};

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    const CPlayerManager* GetPlayerManager() const;     // 0x004C79F0
    CPlayerManager* GetPlayerManager();                 // 0x004C7A00
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CThingPhysical : public CThing { public:
    const C3DVector& GetPosition() const;               // 0x006A4D00
};

class CThingSwitch : public CThingPhysical { public:
    unsigned char _pad94[0xd0 - 0x94];
    float         TriggerRadius;                        // +0xd0
    unsigned char _padd4[0xdc - 0xd4];
    bool          Triggered;                            // +0xdc
    unsigned char _paddd[0xe4 - 0xdd];
    long          TimesTriggered;                       // +0xe4
    void SwitchTriggerPlayerOnlyOnceInArea();
};

void CThingSwitch::SwitchTriggerPlayerOnlyOnceInArea()
{
    if (Triggered)
        return;
    C2DBoxF area;
    area.TLX = GetPosition().X - TriggerRadius;
    area.TLY = GetPosition().Y - TriggerRadius;
    area.BRX = GetPosition().X + TriggerRadius;
    area.BRY = GetPosition().Y + TriggerRadius;
    for (long i = 0; i < 4; ++i) {
        const CThing* self = this;
        if (self->GetPlayerManager()->GetPlayer(i)->IsInitialised()) {
            CThingPhysical* hero = static_cast<CThingPhysical*>(GetPlayerManager()->GetPlayer(i)->GetHeroCreature());
            if (hero && area.ContainsPoint(C2DVector(hero->GetPosition().X, hero->GetPosition().Y))) {
                ++TimesTriggered;
                Triggered = true;
                if (HasTC(0x42)) {
                    CTCEntityEvents* events = (CTCEntityEvents*)GetTC(0x42);
                    events->AddEvent(new NEntityEvents::CEventTriggered(hero));
                }
            }
        }
    }
}


void CEventBase::v00() {}
static int g_events = 0; static CThing* g_trig = 0;
NEntityEvents::CEventTriggered::CEventTriggered(CThing* t) { g_trig = t; }
void CTCEntityEvents::AddEvent(CEventBase* e) { ++g_events; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static const CThing* g_posThing[8]; static C3DVector g_pos[8]; static int g_nPos = 0;
static void SetPos(const CThing* t, float x, float y)
{ int i = 0; while (i < g_nPos && g_posThing[i] != t) ++i; if (i == g_nPos) ++g_nPos; g_posThing[i] = t; g_pos[i].X = x; g_pos[i].Y = y; g_pos[i].Z = 0; }
const C3DVector& CThingPhysical::GetPosition() const
{ for (int i = 0; i < g_nPos; ++i) if (g_posThing[i] == this) return g_pos[i]; static C3DVector z; return z; }

static CPlayer g_players[4]; static bool g_init[4]; static CThing* g_heroes[4];
bool CPlayer::IsInitialised() const { return g_init[this - g_players]; }
CThing* CPlayer::GetHeroCreature() { return g_heroes[this - g_players]; }
static CPlayerManager g_pm;
CPlayer* CPlayerManager::GetPlayer(long n) { return &g_players[n]; }
const CPlayer* CPlayerManager::GetPlayer(long n) const { return &g_players[n]; }
const CPlayerManager* CThing::GetPlayerManager() const { return &g_pm; }
CPlayerManager* CThing::GetPlayerManager() { return &g_pm; }
static CTCEntityEvents g_ev; static CInterfaceMap::SEntry g_entries[2];
static CThingSwitch sw; static CThingPhysical h0, h1, h2;
static void Setup()
{
    g_entries[0].type = 0x42; g_entries[0].component = (CTCBase*)&g_ev; g_entries[1].type = 0x7fffffff; g_entries[1].component = 0;
    sw.m_interfaces.first = g_entries; sw.m_interfaces.last = g_entries + 1; sw.m_presentBits[2] = 4;
    sw.TriggerRadius = 5.0f; SetPos(&sw, 10.0f, 20.0f);
    g_init[0] = true; g_heroes[0] = &h0; SetPos(&h0, 12.0f, 22.0f);      // inside [5,15) x [15,25)
    g_init[1] = true; g_heroes[1] = &h1; SetPos(&h1, 30.0f, 30.0f);      // outside
    g_init[2] = false; g_heroes[2] = &h2; SetPos(&h2, 11.0f, 21.0f);     // inside but player not initialised
    g_init[3] = true; g_heroes[3] = 0;                                    // no hero
}
#define CHECK(c) if (!(c)) { std::printf("%s_TEST FAIL line %d\n", "0083ca90", __LINE__); return 1; }

int main()
{
    Setup();
    h0.Dead = 1;                                                        // this variant ignores Dead
    sw.SwitchTriggerPlayerOnlyOnceInArea();
    CHECK(sw.TimesTriggered == 1 && g_events == 1 && g_trig == &h0 && sw.Triggered);
    sw.SwitchTriggerPlayerOnlyOnceInArea();
    CHECK(sw.TimesTriggered == 1 && g_events == 1);
    sw.Triggered = false; SetPos(&h0, 40.0f, 40.0f);
    sw.SwitchTriggerPlayerOnlyOnceInArea();
    CHECK(sw.TimesTriggered == 1 && !sw.Triggered);
    std::printf("0083ca90_TEST PASS\n");
    return 0;
}
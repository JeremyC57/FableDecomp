// Behaviour test for CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsRunnable (0x00939ED0): model copy with stub callees.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char m_flags91; C3DVector pos; int villager; int perceive; int lineOfSight;
    const C3DVector& GetPosition() const { return pos; }
    bool HasTC(long t) const { return t == 0x23 ? villager != 0 : perceive != 0; }
};
struct CPerceive { bool los; bool IsPlayerExtendedLineOfSight() const { return los; } };
static CPerceive g_perceive;
static int g_village;
static int* GFGetPVillageInterface(const CThing* c) { return c->villager == 2 ? &g_village : 0; }
struct CIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    CIntelligentPointer& operator=(CThing* t) { m_pItem = t; return *this; }
};
static CThing* g_hero;
static long g_frame;
static long g_framesPerSecond = 30;
struct CWorld { static long GetWorldFrame() { return g_frame; } };
static CWorld g_world;
struct CAIBrain { CThing* PCreature; };

struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon {
    CAIBrain* PBrain;
    long NumberOfTimesPlayerTold;
    unsigned long FrameLastToldPlayer;
    CIntelligentPointer Player;
    bool carrying;
    CWorld* GetPWorld() { return &g_world; }
    CWorld* GetPThingWorld() { return &g_world; }
    CThing* GetHero() { return g_hero; }
    bool IsPlayerCarryingWeapon() const { return carrying; }
    bool IsRunnable();
};

bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsRunnable()
{
    CThing* creature = PBrain->PCreature;
    if (creature->HasTC(0x23) && GFGetPVillageInterface(creature) != 0) {
        if (Player.GetPItem() == 0) {
            NumberOfTimesPlayerTold = 0;
            FrameLastToldPlayer = 0;
            Player = GetHero();
        } else if (NumberOfTimesPlayerTold > 0) {
            CWorld* world = GetPWorld();
            long fps = g_framesPerSecond;
            if (world->GetWorldFrame() - FrameLastToldPlayer > (unsigned long)(fps * 120)) {
                --NumberOfTimesPlayerTold;
                FrameLastToldPlayer = GetPThingWorld()->GetWorldFrame();
            }
        }
        if (Player.GetPItem() != 0 && !(Player.GetPItem()->m_flags91 & 1) && IsPlayerCarryingWeapon()) {
            CThing* self = PBrain->PCreature;
            if (self->HasTC(0x34) && g_perceive.IsPlayerExtendedLineOfSight()) {
                CThing* me = PBrain->PCreature;
                const C3DVector& player = Player.GetPItem()->GetPosition();
                const C3DVector& mine = me->GetPosition();
                float dx = mine.X - player.X;
                float dy = mine.Y - player.Y;
                if (dx * dx + dy * dy < 225.0f)
                    return true;
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CThing hero = { 0, { 10, 0, 100 }, 0, 0, 0 };
    CThing guard = { 0, { 0, 0, 0 }, 2, 1, 0 };
    CAIBrain brain = { &guard };
    CAIStateGroup_MinionGuardReactToPlayerWithWeapon s;
    s.PBrain = &brain; s.NumberOfTimesPlayerTold = 7; s.FrameLastToldPlayer = 9; s.Player.m_pItem = 0; s.carrying = true;
    g_hero = &hero; g_perceive.los = true; g_frame = 5000;
    // acquires the hero, resets the counters; 10 units away (Z ignored): runnable
    if (!s.IsRunnable() || s.Player.m_pItem != &hero || s.NumberOfTimesPlayerTold != 0 || s.FrameLastToldPlayer != 0) ok = 0;
    // decay: told twice, more than two minutes (3600 frames) since: count drops, frame refreshed
    s.NumberOfTimesPlayerTold = 2; s.FrameLastToldPlayer = 1000; g_frame = 4601;
    s.IsRunnable();
    if (s.NumberOfTimesPlayerTold != 1 || s.FrameLastToldPlayer != 4601) ok = 0;
    // exactly two minutes: no decay
    g_frame = 4601 + 3600; s.IsRunnable();
    if (s.NumberOfTimesPlayerTold != 1) ok = 0;
    // too far (16 units)
    hero.pos.X = 16; if (s.IsRunnable()) ok = 0;
    hero.pos.X = 10;
    // not carrying, no line of sight, dead hero, no perceiver, no village
    s.carrying = false; if (s.IsRunnable()) ok = 0; s.carrying = true;
    g_perceive.los = false; if (s.IsRunnable()) ok = 0; g_perceive.los = true;
    hero.m_flags91 = 1; if (s.IsRunnable()) ok = 0; hero.m_flags91 = 0;
    guard.perceive = 0; if (s.IsRunnable()) ok = 0; guard.perceive = 1;
    guard.villager = 1; if (s.IsRunnable()) ok = 0; guard.villager = 2;
    if (!s.IsRunnable()) ok = 0;
    if (!ok) { std::printf("00939ed0_TEST FAIL\n"); return 1; }
    std::printf("00939ed0_TEST PASS\n");
    return 0;
}

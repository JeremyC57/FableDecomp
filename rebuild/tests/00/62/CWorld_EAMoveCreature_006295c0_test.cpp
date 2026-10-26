// Behaviour test for CWorld::EAMoveCreature 0x006295C0: payload (UID, X, Y, action) -> ApplyRelativeMovementAcceleration on a live creature.
#include <cstdio>
#include <cstring>

struct CMemoryBuffer {
    unsigned char Data[0x20];
    long Length, ReadPos, WritePos;
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data) { std::memcpy(data, Data + ReadPos, length); ReadPos += (long)length; }
};
class CGameEvent { public:
    signed char Player;
    unsigned char Data[32];
    long GetData(CMemoryBuffer& buffer) const { std::memcpy(buffer.Data, Data, 32); buffer.Length = 32; return 32; }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
enum EGameAction { GAME_ACTION_NONE = 0, GAME_ACTION_RUN = 7 };

static int g_applied = 0;
static C3DVector g_lastAccel(0, 0, 0);
static EGameAction g_lastAction = GAME_ACTION_NONE;
static long g_lastPlayer = -1;

class CThingPlayerCreature { public:
    bool Dead;
    void ApplyRelativeMovementAcceleration(const C3DVector& a, EGameAction action) { ++g_applied; g_lastAccel = a; g_lastAction = action; }
};
static CThingPlayerCreature g_creature;
class CThingManager { public:
    CThingPlayerCreature* GetThingWithUID(long long uid) { return uid == 0x123456789LL ? &g_creature : 0; }
};
static CThingManager g_manager;
class CPlayer;

class CWorld { public:
    virtual CThingManager* GetThingManager() { return &g_manager; }
    virtual ~CWorld() {}
    CPlayer* GetPlayer(long number) { g_lastPlayer = number; return 0; }
    void EAMoveCreature(const CGameEvent* event);
};

void CWorld::EAMoveCreature(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    (void)player;
    CMemoryBuffer buffer;
    event->GetData(buffer);
    long long uid;
    buffer.Read(8, &uid);
    float x;
    buffer.Read(4, &x);
    float y;
    buffer.Read(4, &y);
    EGameAction action;
    buffer.Read(4, &action);
    C3DVector acceleration(x, y, 0.0f);
    CThingPlayerCreature* creature = GetThingManager()->GetThingWithUID(uid);
    if (creature && !creature->Dead)
        creature->ApplyRelativeMovementAcceleration(acceleration, action);
}

static void Fill(CGameEvent& e, long long uid, float x, float y, long action)
{
    std::memset(e.Data, 0, sizeof(e.Data));
    std::memcpy(e.Data, &uid, 8); std::memcpy(e.Data + 8, &x, 4); std::memcpy(e.Data + 12, &y, 4); std::memcpy(e.Data + 16, &action, 4);
}

int main()
{
    CWorld w; CGameEvent e; e.Player = 1; bool ok = true;
    g_creature.Dead = false;
    Fill(e, 0x123456789LL, 1.5f, -2.0f, 7);
    w.EAMoveCreature(&e);
    if (g_applied != 1 || g_lastAccel.X != 1.5f || g_lastAccel.Y != -2.0f || g_lastAccel.Z != 0.0f || g_lastAction != GAME_ACTION_RUN || g_lastPlayer != 1) ok = false;
    g_creature.Dead = true;
    w.EAMoveCreature(&e);
    if (g_applied != 1) ok = false;
    g_creature.Dead = false;
    Fill(e, 42, 1.0f, 1.0f, 0);
    w.EAMoveCreature(&e);
    if (g_applied != 1) ok = false;
    if (!ok) { std::printf("006295c0_TEST FAIL\n"); return 1; }
    std::printf("006295c0_TEST PASS\n");
    return 0;
}
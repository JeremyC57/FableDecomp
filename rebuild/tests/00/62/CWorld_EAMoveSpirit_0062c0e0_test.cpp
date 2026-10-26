// Behaviour test for CWorld::EAMoveSpirit 0x0062C0E0: hero with a co-op spirit TC gets the event's (X, Y, 0) movement.
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
static int g_moves = 0; static C3DVector g_last(0, 0, 0);
class CTCCoopSpirit { public: void ApplyMovementVector(const C3DVector& m) { ++g_moves; g_last = m; } };
class CThing { public:
    bool HasSpirit; CTCCoopSpirit* Spirit;
    bool HasTC(long type) const { return type == 0xC7 && HasSpirit; }
    template<class T> T* GetTC(long) { return Spirit; }
};
static CThing* g_hero = 0; static int g_heroCalls = 0;
class CPlayer { public: CThing* GetHeroThing() { ++g_heroCalls; return g_hero; } };
static CPlayer g_player;
class CWorld { public:
    CPlayer* GetPlayer(long) { return &g_player; }
    void EAMoveSpirit(const CGameEvent* event);
};

void CWorld::EAMoveSpirit(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->GetHeroThing())
    {
        CThing* hero = player->GetHeroThing();
        if (hero->HasTC(0xC7))
        {
            CTCCoopSpirit* spirit = hero->GetTC<CTCCoopSpirit>(0xC7);
            {
                CMemoryBuffer buffer;
                event->GetData(buffer);
                float x;
                buffer.Read(4, &x);
                float y;
                buffer.Read(4, &y);
                C3DVector movement(x, y, 0.0f);
                spirit->ApplyMovementVector(movement);
            }
        }
    }
}

int main()
{
    CWorld w; CGameEvent e; e.Player = 0; float x = 0.25f, y = -4.0f;
    std::memset(e.Data, 0, 32); std::memcpy(e.Data, &x, 4); std::memcpy(e.Data + 4, &y, 4);
    CTCCoopSpirit spirit; CThing hero; hero.HasSpirit = true; hero.Spirit = &spirit;
    bool ok = true;
    w.EAMoveSpirit(&e); if (g_moves != 0 || g_heroCalls != 1) ok = false;
    g_hero = &hero; w.EAMoveSpirit(&e);
    if (g_moves != 1 || g_last.X != 0.25f || g_last.Y != -4.0f || g_last.Z != 0.0f || g_heroCalls != 3) ok = false;
    hero.HasSpirit = false; w.EAMoveSpirit(&e); if (g_moves != 1) ok = false;
    if (!ok) { std::printf("0062c0e0_TEST FAIL\n"); return 1; }
    std::printf("0062c0e0_TEST PASS\n");
    return 0;
}
// Behaviour test for CWorld::EACloseHeroCentreDoor 0x0062C300: live hero + UID of a thing with a hero-centre TC -> OnHeroCloseDoor.
// (Retail calls through an unset pointer when the thing lacks the component; the test only drives defined paths.)
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
static int g_closes = 0;
class CTCHeroCentre { public: void OnHeroCloseDoor() { ++g_closes; } };
class CThing { public:
    bool Dead; CTCHeroCentre* Centre;
    bool HasTC(long type) const { return type == 0x6A && Centre != 0; }
    template<class T> T* GetTC(long) { return Centre; }
    template<class T> T* PeekTC(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
static CThing g_door;
class CThingManager { public: CThing* GetThingWithUID(long long uid) { return uid == 77 ? &g_door : 0; } };
static CThingManager g_manager;
static CThing* g_hero = 0;
class CPlayer { public: CThing* GetHeroThing() { return g_hero; } };
static CPlayer g_player;
class CWorld { public:
    virtual CThingManager* GetThingManager() { return &g_manager; }
    virtual ~CWorld() {}
    CPlayer* GetPlayer(long) { return &g_player; }
    void EACloseHeroCentreDoor(const CGameEvent* event);
};

void CWorld::EACloseHeroCentreDoor(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetHeroThing();
    if (hero && !hero->Dead)
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        long long uid;
        buffer.Read(8, &uid);
        CThing* door = GetThingManager()->GetThingWithUID(uid);
        if (door)
            door->PeekTC<CTCHeroCentre>(0x6A)->OnHeroCloseDoor();
    }
}

int main()
{
    CWorld w; CGameEvent e; e.Player = 0; long long uid = 77; std::memset(e.Data, 0, 32); std::memcpy(e.Data, &uid, 8);
    CTCHeroCentre centre; g_door.Centre = &centre; g_door.Dead = false;
    CThing hero; hero.Dead = false; hero.Centre = 0;
    bool ok = true;
    w.EACloseHeroCentreDoor(&e); if (g_closes != 0) ok = false;         // no hero
    g_hero = &hero; w.EACloseHeroCentreDoor(&e); if (g_closes != 1) ok = false;
    hero.Dead = true; w.EACloseHeroCentreDoor(&e); if (g_closes != 1) ok = false;
    hero.Dead = false; uid = 5; std::memcpy(e.Data, &uid, 8); w.EACloseHeroCentreDoor(&e); if (g_closes != 1) ok = false;
    if (!ok) { std::printf("0062c300_TEST FAIL\n"); return 1; }
    std::printf("0062c300_TEST PASS\n");
    return 0;
}
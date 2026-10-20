// Behaviour test for CWorld::EAOpenHeroCentreDoor (0x0062C1C0): no call for a missing/dead hero or an open inventory
// or an unknown door UID; otherwise the door's hero-centre component gets the event's door type.
#include <cstdio>
#include <cstring>
struct CMemoryBuffer {
    unsigned char Data[0x20]; long Length, ReadPos, WritePos;
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) {}
    void Read(unsigned long length, void* data) { std::memcpy(data, Data + ReadPos, length); ReadPos += (long)length; }
};
struct CGameEvent { signed char Player; unsigned char Payload[0x20]; long GetData(CMemoryBuffer& b) const { std::memcpy(b.Data, Payload, 0x20); return 12; } };
struct CTCInventoryBase { bool InventoryOpen; };
enum EHeroCentreDoorType { HERO_CENTRE_DOOR_NONE = 0 };
static int g_door = -1; static int g_calls = 0;
struct CTCHeroCentre { void OnHeroWalkedThroughDoor(EHeroCentreDoorType d) { g_door = d; ++g_calls; } };
struct CThing {
    bool Dead; CTCInventoryBase* Inventory; CTCHeroCentre* Centre;
    template<class T> const T* PeekTC(long) const;
    template<class T> T* PeekTC(long);
};
template<> const CTCInventoryBase* CThing::PeekTC<CTCInventoryBase>(long t) const { return t == 0x11 ? Inventory : 0; }
template<> CTCHeroCentre* CThing::PeekTC<CTCHeroCentre>(long t) { return t == 0x6A ? Centre : 0; }
static CThing* g_door_thing; static long long g_uid_asked;
struct CThingManager { CThing* GetThingWithUID(long long uid) { g_uid_asked = uid; return uid == 77 ? g_door_thing : 0; } };
static CThingManager g_tm;
struct CPlayer { CThing* Hero; CThing* GetHeroThing() { return Hero; } };
struct CWorld {
    CPlayer* P;
    CThingManager* GetThingManager() { return &g_tm; }
    CPlayer* GetPlayer(long) { return P; }
    void EAOpenHeroCentreDoor(const CGameEvent* event)
    {
        const CThing* hero = GetPlayer(event->Player)->GetHeroThing();
        if (hero && !hero->Dead)
        {
            if (!hero->PeekTC<CTCInventoryBase>(0x11)->InventoryOpen)
            {
                CMemoryBuffer buffer;
                event->GetData(buffer);
                int door;              // EHeroCentreDoorType: a 4-byte payload word
                buffer.Read(4, &door);
                long long uid;
                buffer.Read(8, &uid);
                CThing* thing = GetThingManager()->GetThingWithUID(uid);
                if (thing)
                    thing->PeekTC<CTCHeroCentre>(0x6A)->OnHeroWalkedThroughDoor((EHeroCentreDoorType)door);
            }
        }
    }
};
static bool run(bool hasHero, bool dead, bool open, long long uid, int wantCalls, int wantDoor)
{
    CTCInventoryBase inv = { open }; CTCHeroCentre centre; CThing hero = { dead, &inv, 0 }; CThing door = { false, 0, &centre };
    g_door_thing = &door; CPlayer p = { hasHero ? &hero : 0 }; CWorld w; w.P = &p;
    CGameEvent e; std::memset(&e, 0, sizeof e); int type = 3; std::memcpy(e.Payload, &type, 4); std::memcpy(e.Payload + 4, &uid, 8);
    g_calls = 0; g_door = -1;
    w.EAOpenHeroCentreDoor(&e);
    return g_calls == wantCalls && g_door == wantDoor;
}
int main()
{
    bool ok = run(false, false, false, 77, 0, -1) && run(true, true, false, 77, 0, -1) && run(true, false, true, 77, 0, -1)
        && run(true, false, false, 5, 0, -1) && run(true, false, false, 77, 1, 3);
    if (!ok) { std::printf("0062c1c0_TEST FAIL\n"); return 1; }
    std::printf("0062c1c0_TEST PASS\n");
    return 0;
}
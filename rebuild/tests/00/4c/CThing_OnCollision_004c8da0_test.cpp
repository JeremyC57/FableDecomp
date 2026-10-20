// Behaviour test for CThing::OnCollision (0x004C8DA0): on phase 1 the colliding thing (fourth argument) reaches the
// assassin-rush component (0xA9) and the 0xA8 component only when present; a live thing is re-added to the update
// list on every phase, a dead one never.
#include <cstdio>
struct CThing;
static int g_rush = 0, g_handler = 0, g_updates = 0; static CThing* g_rushThing = 0; static CThing* g_handlerThing = 0; static CThing* g_updated = 0;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CTCAssassinRush { void SetThingCollisionPtr(CThing* thing) { ++g_rush; g_rushThing = thing; } };
struct CTCCollisionHandler_0xA8 { void OnThingCollision(CThing* thing) { ++g_handler; g_handlerThing = thing; } };
struct CThingManager { void AddToUpdateList(CThing* thing) { ++g_updates; g_updated = thing; } };
static CThingManager g_manager; static CThingManager* g_pThingManager = &g_manager;
struct CThing {
    unsigned char _pad00[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char _pad4c[0x91 - 0x4c]; unsigned char Dead : 1;
    template<class T> bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
    void OnCollision(long phase, long a, long b, CThing* other);
};
void CThing::OnCollision(long phase, long, long, CThing* other)
{
    if (phase == 1) {
        CTCAssassinRush* rush;
        if (GetInterface(&rush, 0xa9)) rush->SetThingCollisionPtr(other);
        CTCCollisionHandler_0xA8* handler;
        if (GetInterface(&handler, 0xa8)) handler->OnThingCollision(other);
    }
    if (!Dead) g_pThingManager->AddToUpdateList(this);
}
static int fail(const char* why) { std::printf("004c8da0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CTCAssassinRush rush; static CTCCollisionHandler_0xA8 handler; static CThing other;
    static CInterfaceMap::SEntry entries[3] = { { 0x10, 0 }, { 0xa8, &handler }, { 0xa9, &rush } };
    static CThing thing; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3; thing.Dead = 0;
    thing.m_presentBits[0xa9 >> 5] = 1UL << (0xa9 & 31);                 // only the assassin rush present
    thing.OnCollision(1, 7, 8, &other);
    if (g_rush != 1 || g_rushThing != &other || g_handler != 0 || g_updates != 1 || g_updated != &thing) return fail("rush only");
    thing.m_presentBits[0xa8 >> 5] |= 1UL << (0xa8 & 31);
    thing.OnCollision(1, 0, 0, &other);
    if (g_rush != 2 || g_handler != 1 || g_handlerThing != &other || g_updates != 2) return fail("both components");
    thing.OnCollision(2, 0, 0, &other);
    if (g_rush != 2 || g_handler != 1 || g_updates != 3) return fail("other phase only updates");
    thing.Dead = 1; thing.OnCollision(1, 0, 0, &other);
    if (g_rush != 3 || g_updates != 3) return fail("dead thing not re-listed");
    std::printf("004c8da0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
struct CTCBase { long m_tag; };
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
struct CThingPlayerCreature { unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; } return false; } };
struct CPlayer { CThingPlayerCreature* m_pCreature; CThingPlayerCreature* GetPlayerCreature() { return m_pCreature; } };

struct CTCInGameMenu : CTCBase { };
struct CInputProcessInGameMenu { virtual void v00() { } virtual void v01() { } virtual void Init() { MyPlayer->GetPlayerCreature()->GetInterface(&PInGameMenu, 0xc6); FrameSinceLastMovement = 0; CurrentMovement = 0; StartedRepeatingMovement = false; }
    unsigned char _pad04[0x10]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface;
    CTCInGameMenu* PInGameMenu; long FrameSinceLastMovement; long CurrentMovement; bool StartedRepeatingMovement;
};
int main()
{
    CThingPlayerCreature creature; std::memset(&creature, 0, sizeof creature); CInterfaceMap::SEntry entries[4]; CTCInGameMenu comp; entries[0].type = 3; entries[0].component = 0; entries[1].type = 0xc6; entries[1].component = &comp; entries[2].type = 0x200; entries[2].component = 0; entries[3].type = 0; entries[3].component = 0;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3; CPlayer player; player.m_pCreature = &creature; CInputProcessInGameMenu p; p.MyPlayer = &player; p.PInGameMenu = 0; p.FrameSinceLastMovement = 5; p.CurrentMovement = 6; p.StartedRepeatingMovement = true;
    p.Init(); if (p.PInGameMenu != 0 || !(p.FrameSinceLastMovement == 0 && p.CurrentMovement == 0 && !p.StartedRepeatingMovement)) { std::printf("006907d0_TEST FAIL\n"); return 1; }
    creature.m_presentBits[0xc6 >> 5] |= 1UL << (0xc6 & 31); p.PInGameMenu = 0; p.FrameSinceLastMovement = 5; p.CurrentMovement = 6; p.StartedRepeatingMovement = true;
    p.Init(); if (p.PInGameMenu != &comp || !(p.FrameSinceLastMovement == 0 && p.CurrentMovement == 0 && !p.StartedRepeatingMovement)) { std::printf("006907d0_TEST FAIL\n"); return 1; }
    entries[1].type = 0xc6 + 1; p.PInGameMenu = 0; p.FrameSinceLastMovement = 5; p.CurrentMovement = 6; p.StartedRepeatingMovement = true; p.Init(); if (p.PInGameMenu != 0) { std::printf("006907d0_TEST FAIL\n"); return 1; }
    std::printf("006907d0_TEST PASS\n");
    return 0;
}

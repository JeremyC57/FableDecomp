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

struct CTCInventoryQuests : CTCBase { };
struct CInputProcessInventoryQuestsScreen { virtual void v00() { } virtual void v01() { } virtual void Init() { MyPlayer->GetPlayerCreature()->GetInterface(&PInventory, 0x19); CurrentQuestMovement = 0; CurrentQuestSelectMovement = 0; }
    unsigned char _pad04[0x10]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface;
    unsigned char _pad28[0x0c]; CTCInventoryQuests* PInventory; long CurrentQuestMovement; long CurrentQuestSelectMovement;
};
int main()
{
    CThingPlayerCreature creature; std::memset(&creature, 0, sizeof creature); CInterfaceMap::SEntry entries[4]; CTCInventoryQuests comp; entries[0].type = 3; entries[0].component = 0; entries[1].type = 0x19; entries[1].component = &comp; entries[2].type = 0x200; entries[2].component = 0; entries[3].type = 0; entries[3].component = 0;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3; CPlayer player; player.m_pCreature = &creature; CInputProcessInventoryQuestsScreen p; p.MyPlayer = &player; p.PInventory = 0; p.CurrentQuestMovement = 5; p.CurrentQuestSelectMovement = 6;
    p.Init(); if (p.PInventory != 0 || !(p.CurrentQuestMovement == 0 && p.CurrentQuestSelectMovement == 0)) { std::printf("0068bc20_TEST FAIL\n"); return 1; }
    creature.m_presentBits[0x19 >> 5] |= 1UL << (0x19 & 31); p.PInventory = 0; p.CurrentQuestMovement = 5; p.CurrentQuestSelectMovement = 6;
    p.Init(); if (p.PInventory != &comp || !(p.CurrentQuestMovement == 0 && p.CurrentQuestSelectMovement == 0)) { std::printf("0068bc20_TEST FAIL\n"); return 1; }
    entries[1].type = 0x19 + 1; p.PInventory = 0; p.CurrentQuestMovement = 5; p.CurrentQuestSelectMovement = 6; p.Init(); if (p.PInventory != 0) { std::printf("0068bc20_TEST FAIL\n"); return 1; }
    std::printf("0068bc20_TEST PASS\n");
    return 0;
}

#include <cstdio>
#include <cstring>
class CTCBase { public: };
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
class CThingCreatureBase { public: unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type); if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; }
        return false; } };

class CThing { public: int id; };
static CThing* g_cast; static int g_casts;
struct CTCFireballSpell_Calls { void CastFireball(CThing* target) { g_cast = target; ++g_casts; } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)m_pItem; } };
class CCreatureAction_FinishFireballSpell { public: virtual void v00() { } virtual void PerformAction(); unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xa8 - 0x10]; CBaseIntelligentPointer PTarget; };
void CCreatureAction_FinishFireballSpell::PerformAction() { CTCFireballSpell_Calls* pSpell; if (PCreature.GetPItem()->GetInterface(&pSpell, 0x9d)) pSpell->CastFireball((CThing*)PTarget.GetPItem()); }
int main()
{
    CThingCreatureBase creature; std::memset(&creature, 0, sizeof(creature)); CThing target;
    CTCFireballSpell_Calls spell;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x9d, (CTCBase*)&spell } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    CCreatureAction_FinishFireballSpell a; a.PCreature.m_pItem = &creature; a.PTarget.m_pItem = &target;
    a.PerformAction();
    if (g_casts != 0) { std::printf("008babe0_TEST FAIL\n"); return 1; }
    creature.m_presentBits[4] |= 0x20000000;
    a.PerformAction();
    if (g_casts != 1 || g_cast != &target || (char*)&a.PTarget - (char*)&a != 0xa8) { std::printf("008babe0_TEST FAIL\n"); return 1; }
    std::printf("008babe0_TEST PASS\n");
    return 0;
}
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

struct CTCEnflameSpell { unsigned char _pad0[0x0d]; unsigned char m_cast; };
struct CBaseIntelligentPointer { virtual void v00() { } CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
class CCreatureAction_CastWillEnflameSpell { public: virtual void v00() { } virtual void PerformAction(); unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; };
void CCreatureAction_CastWillEnflameSpell::PerformAction() { CTCEnflameSpell* pSpell; if (PCreature.GetPItem()->GetInterface(&pSpell, 0x95)) pSpell->m_cast = 1; }
int main()
{
    CThingCreatureBase creature; std::memset(&creature, 0, sizeof(creature));
    CTCEnflameSpell spell; spell.m_cast = 0;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x95, (CTCBase*)&spell } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    CCreatureAction_CastWillEnflameSpell a; a.PCreature.m_pItem = &creature;
    a.PerformAction();
    if (spell.m_cast != 0) { std::printf("008ba370_TEST FAIL\n"); return 1; }
    creature.m_presentBits[4] |= 0x200000;
    a.PerformAction();
    if (spell.m_cast != 1) { std::printf("008ba370_TEST FAIL\n"); return 1; }
    std::printf("008ba370_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
struct CTCBase { int id; };
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
struct CThing { unsigned char _pad0[0x10]; unsigned char m_type; unsigned char _pad11[0x0f]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; } return false; } };
static int g_aware = 0; static CThing* g_perceived;
struct CTCPerceiveThing : CTCBase { CThing* GetPPerceivedThing() { return g_perceived; } void SetAsDirectlyAwareOfPerceivedThing() { ++g_aware; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCreatureAction_AlertToPlayer { virtual void PerformAction(); unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; };
void CCreatureAction_AlertToPlayer::PerformAction() { CTCPerceiveThing* pPerceive; if (PCreature.GetPItem()->GetInterface(&pPerceive, 0x34)) { CThing* pPerceived = pPerceive->GetPPerceivedThing(); if (pPerceived && pPerceived->m_type == 2) pPerceive->SetAsDirectlyAwareOfPerceivedThing(); } }
int main()
{
    CTCPerceiveThing perceive; CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x34, &perceive } };
    CThing creature; std::memset(&creature, 0, sizeof(creature)); creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    CThing player; std::memset(&player, 0, sizeof(player)); player.m_type = 2; CThing other; std::memset(&other, 0, sizeof(other)); other.m_type = 1;
    CCreatureAction_AlertToPlayer a; a.PCreature.m_pItem = &creature;
    g_perceived = &player; a.PerformAction(); int c1 = g_aware;                       // no interface bit: nothing
    creature.m_presentBits[1] |= 0x100000; g_perceived = &other; a.PerformAction(); int c2 = g_aware;
    g_perceived = 0; a.PerformAction(); int c3 = g_aware;
    g_perceived = &player; a.PerformAction();
    if (c1 != 0 || c2 != 0 || c3 != 0 || g_aware != 1) { std::printf("00849320_TEST FAIL\n"); return 1; }
    std::printf("00849320_TEST PASS\n");
    return 0;
}
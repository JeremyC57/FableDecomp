#include <cstdio>
#include <cstring>
struct CTCBase { };
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
struct CThingCreatureBase { unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type); if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; }
        return false; } };
struct CTCSpellCastState { unsigned char _pad0[0x0c]; unsigned char m_isCasting; long m_castLevel; };
struct CBaseIntelligentPointer { virtual void v00() { } CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
static int g_baseCalls;
struct CCombatActionBase { virtual void v00() { } virtual void FinishAction() { ++g_baseCalls; } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0x114 - 0x10]; };
struct CCreatureAction_CastPhysicalShieldSpell : CCombatActionBase { virtual void FinishAction(); };
void CCreatureAction_CastPhysicalShieldSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCSpellCastState* pState;
    if (PCreature.GetPItem()->GetInterface(&pState, 0xa7)) { pState->m_isCasting = 0; pState->m_castLevel = 0; }
}
int main()
{
    CThingCreatureBase creature; std::memset(&creature, 0, sizeof(creature));
    CTCSpellCastState state; state.m_isCasting = 1; state.m_castLevel = 3;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0xa7, (CTCBase*)&state } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    CCreatureAction_CastPhysicalShieldSpell a; a.PCreature.m_pItem = &creature;
    a.FinishAction();                                   // component absent: untouched
    if (g_baseCalls != 1 || state.m_isCasting != 1 || state.m_castLevel != 3) { std::printf("008ba790_TEST FAIL\n"); return 1; }
    creature.m_presentBits[5] |= 0x80;
    a.FinishAction();
    if (g_baseCalls != 2 || state.m_isCasting != 0 || state.m_castLevel != 0) { std::printf("008ba790_TEST FAIL\n"); return 1; }
    std::printf("008ba790_TEST PASS\n");
    return 0;
}

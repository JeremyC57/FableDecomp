// Behaviour test for 00954ab0: body copied from src/00954ab0.cpp (CAIStateGroup_MazeTutorialBattle::IsRunnable), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
static CThing* g_target = 0; static int g_getTarget = 0;
struct CTCTargeting {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {}
    virtual CThing* GetTarget() { ++g_getTarget; return g_target; }
};
static int g_setPItem = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return (CThing*)m_pItem; }
    void SetPItem(void* item) { ++g_setPItem; m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain;
    unsigned char _pad[0x0c];
    virtual bool IsRunnable() { return false; }
};
struct CAIStateGroup_MazeTutorialBattle : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;
    long _base1c;
    long FrameOfLastShield;
    virtual bool IsRunnable();
};
bool CAIStateGroup_MazeTutorialBattle::IsRunnable()
{
    CTCTargeting* targeting;
    if (PBrain->PCreature->HasTC(8))
        targeting = PBrain->PCreature->GetTC<CTCTargeting>(8);
    CThing* target = targeting->GetTarget();
    if (target && !(target->m_flags91 & 1)) {
        PTargetThing = CIntelligentPointer<CThing>(target);
        return true;
    }
    return false;
}
int main()
{
    CThing creature; memset(&creature, 0, sizeof(creature)); CTCTargeting targeting; CThing enemy; memset(&enemy, 0, sizeof(enemy));
    CInterfaceMap::SEntry entries[2] = { { 8, (CTCBase*)&targeting }, { 0x30, (CTCBase*)0x99 } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    creature.m_presentBits[0] = 1UL << 8;                 // a creature always carries its targeting component (the miss arm uses an unset pointer)
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_MazeTutorialBattle g; g.PBrain = &brain;
    // 1. no target: false, nothing stored
    g_target = 0; CHECK(g.IsRunnable() == false && g_getTarget == 1 && g_setPItem == 0 && g.PTargetThing.m_pItem == 0);
    // 2. dead target: false
    g_target = &enemy; enemy.m_flags91 = 1; CHECK(g.IsRunnable() == false && g_getTarget == 2 && g_setPItem == 0);
    // 3. live target: stored through a temporary intelligent pointer, true
    enemy.m_flags91 = 0; CHECK(g.IsRunnable() == true && g_getTarget == 3 && g_setPItem == 2 && g.PTargetThing.m_pItem == &enemy);
    if (fails == 0) printf("00954ab0_TEST PASS\n"); else printf("00954ab0_TEST FAIL\n");
    return fails;
}
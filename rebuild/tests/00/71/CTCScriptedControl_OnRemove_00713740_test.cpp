// Behaviour test for 00713740: body copied from src/00713740.cpp (CTCScriptedControl::vfunc_13 / OnRemove), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBaseAny;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseAny* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
struct CAnimateThing {
    unsigned char _pad00[0x94];
    unsigned char m_changed : 1;
    unsigned char m_flags94_1 : 7;
};
static int g_removed = 0; static CTCBaseAny* g_removedOn = 0; static CAnimateThing g_anim; static int g_getAnim = 0;
struct CTCBaseAny {
    void OnStateGroupRemoved() { ++g_removed; g_removedOn = this; }
    CAnimateThing& GetAnimation() { ++g_getAnim; return g_anim; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};
static int g_baseRemove = 0;
struct CTCBase {
    virtual void vfunc_13() { ++g_baseRemove; }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
};
struct CTCScriptedControl : CTCBase {
    virtual void vfunc_13();
};
void CTCScriptedControl::vfunc_13()
{
    if (ParentThing->HasInterface(7)) {
        long type = 7;
        CInterfaceMap* map = &ParentThing->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || 7 < it->type)
            it = map->last;
        it->component->OnStateGroupRemoved();
    }
    if (ParentThing->HasInterface(0x5a)) {
        long type = 0x5a;
        CInterfaceMap* map = &ParentThing->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || 0x5a < it->type)
            it = map->last;
        {
            CAnimateThing& animation = it->component->GetAnimation();
            animation.m_changed = 1;
        }
    }
    CTCBase::vfunc_13();
}
int main()
{
    static CThing thing; static CTCBaseAny stateTC, animTC; memset(&thing, 0, sizeof(thing)); memset(&g_anim, 0, sizeof(g_anim)); g_anim.m_flags94_1 = 0x55;
    CInterfaceMap::SEntry entries[3] = { { 7, &stateTC }, { 0x30, 0 }, { 0x5a, &animTC } }; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    CTCScriptedControl tc; tc.ParentThing = &thing;
    // neither component present: only the base
    tc.vfunc_13(); CHECK(g_removed == 0 && g_getAnim == 0 && g_baseRemove == 1 && g_anim.m_changed == 0);
    // interface 7 only
    thing.m_presentBits[0] |= 1UL << 7; tc.vfunc_13(); CHECK(g_removed == 1 && g_removedOn == &stateTC && g_getAnim == 0 && g_baseRemove == 2);
    // both: the animation flagged as changed, the other bits kept
    thing.m_presentBits[0x5a >> 5] |= 1UL << (0x5a & 31); tc.vfunc_13(); CHECK(g_removed == 2 && g_getAnim == 1 && g_anim.m_changed == 1 && g_anim.m_flags94_1 == 0x55 && g_baseRemove == 3);
    if (fails == 0) printf("00713740_TEST PASS\n"); else printf("00713740_TEST FAIL\n");
    return fails;
}
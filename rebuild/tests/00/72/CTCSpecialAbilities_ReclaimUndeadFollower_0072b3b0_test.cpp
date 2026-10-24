// Behaviour test for 0x0072B3B0 CTCSpecialAbilities::ReclaimUndeadFollower.
#include <stdio.h>
struct CTCBase { void* vt; struct CThing* ParentThing; bool ToBeRemoved; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned int m_presentBits[9];
    CInterfaceMap m_interfaces;
    int killed;
    template <class T> bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (const T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    void Kill(bool silent) { killed = silent ? 2 : 1; }
};
struct CThingGameObject : CThing { float Health; };
struct CTCRandomAppearanceMorph { unsigned int Seed; };
struct CUndeadFollower { long Health; void* PThing; bool Active; unsigned int AppearanceSeed; };
struct CTCSpecialAbilities : CTCBase {
    CUndeadFollower records[2];
    const CThing* owners[2];
    CUndeadFollower* GetUndeadFollower(const CThing* thing)
    {
        for (int i = 0; i < 2; ++i) if (owners[i] == thing) return &records[i];
        return 0;
    }
    void ReclaimUndeadFollower(CThingGameObject& thing);
};
void CTCSpecialAbilities::ReclaimUndeadFollower(CThingGameObject& thing)
{
    CUndeadFollower* follower = GetUndeadFollower(&thing);
    follower->Health = (long)thing.Health;
    const CTCRandomAppearanceMorph* morph;
    if (thing.GetInterface(&morph, 3))
        follower->AppearanceSeed = morph->Seed;
    follower->Active = false;
    thing.Kill(false);
}
int main()
{
    int fail = 0;
    CTCRandomAppearanceMorph morph; morph.Seed = 0xBEEF;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 3; entries[0].component = (CTCBase*)&morph;
    entries[1].type = 9; entries[1].component = 0;
    CThingGameObject a; for (int i = 0; i < 9; ++i) a.m_presentBits[i] = 0;
    a.m_presentBits[0] = 8; a.m_interfaces.first = entries; a.m_interfaces.last = entries + 2; a.killed = 0; a.Health = 37.9f;
    CThingGameObject b; for (int i = 0; i < 9; ++i) b.m_presentBits[i] = 0;
    b.m_interfaces.first = entries; b.m_interfaces.last = entries + 2; b.killed = 0; b.Health = -4.5f;
    CTCSpecialAbilities sa;
    sa.owners[0] = &a; sa.owners[1] = &b;
    for (int i = 0; i < 2; ++i) { sa.records[i].Health = 0; sa.records[i].Active = true; sa.records[i].AppearanceSeed = 7; }
    sa.ReclaimUndeadFollower(a);
    if (sa.records[0].Health != 37 || sa.records[0].Active || sa.records[0].AppearanceSeed != 0xBEEF || a.killed != 1) fail |= 1;
    sa.ReclaimUndeadFollower(b);
    if (sa.records[1].Health != -4 || sa.records[1].Active || sa.records[1].AppearanceSeed != 7 || b.killed != 1) fail |= 2;
    if (fail) { printf("0072b3b0_TEST FAIL %d\n", fail); return 1; }
    printf("0072b3b0_TEST PASS\n");
    return 0;
}
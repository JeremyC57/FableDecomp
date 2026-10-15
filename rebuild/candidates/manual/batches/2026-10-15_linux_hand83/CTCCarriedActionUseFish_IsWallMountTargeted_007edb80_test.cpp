// Behaviour test for 007edb80 (CTCCarriedActionUseFish::IsWallMountTargeted): true only when the user has interface 8 and that component's target carries flag 0x4000000.
#include <cstdio>

struct CThingTarget {
    unsigned long m_flags;
};

struct CTCWallMountable {
    CThingTarget* m_target;
    virtual CThingTarget* GetTarget() { return m_target; }
    virtual ~CTCWallMountable() { }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCWallMountable* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CInterfaceMap::SEntry* FindEntry(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->type <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
};

struct CTCCarriedActionUseFish {
    bool IsWallMountTargeted(CThing* user)
    {
        if (user->HasInterface(8)) {
            CThingTarget* target = user->FindEntry(8)->component->GetTarget();
            if (target && (target->m_flags & 0x4000000))
                return true;
        }
        return false;
    }
};

int main()
{
    CThingTarget target; target.m_flags = 0x4000000;
    CTCWallMountable mount; mount.m_target = 0;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 8; entries[0].component = &mount;
    entries[1].type = 9; entries[1].component = 0;
    CThing user;
    for (int i = 0; i < 9; ++i) user.m_presentBits[i] = 0;
    user.m_interfaces.first = entries; user.m_interfaces.last = entries + 2;
    CTCCarriedActionUseFish action;
    bool ok = !action.IsWallMountTargeted(&user);       // no interface 8
    user.m_presentBits[0] = 1UL << 8;
    ok = ok && !action.IsWallMountTargeted(&user);      // no target
    mount.m_target = &target;
    ok = ok && action.IsWallMountTargeted(&user);
    target.m_flags = 0x2000000;
    ok = ok && !action.IsWallMountTargeted(&user);
    if (!ok) { std::printf("007edb80_TEST FAIL\n"); return 1; }
    std::printf("007edb80_TEST PASS\n");
    return 0;
}

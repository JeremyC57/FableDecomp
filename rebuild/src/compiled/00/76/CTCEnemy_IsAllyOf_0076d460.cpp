// CTCEnemy::IsAllyOf(const CThing&) const -- the parent itself is an ally, an enemy never is, otherwise allied when both factions are known and the faction table allies them
// Leaf: IsAllyOf
//
// Retail 0x0076D460, 132 bytes (`ret 4`). Ego_r.pdb `?IsAllyOf@CTCEnemy@@ABE_NABVCThing@@@Z`. Callees:
// CTCEnemy::IsEnemyOf(const CThing&) const @ 0x0076CCE0, CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound @ 0x004365B0 (inlined lookup of the other thing's enemy component, interface 0x49 = bit 9
// of +0x28), the faction ally test @ 0x006B9460 (manifest "FactionMap_FindAndCompare"; `this` = our
// faction, the other faction pushed).
// Layout: Ego_r.pdb CTCEnemy PFaction +0x28 sits at +0x18 in retail (the two lists ahead of it are
// 4-byte STLport lists there).
// Shape: the other thing's component comes from an inline accessor returning an UNSET local when the
// interface is absent (`CTCEnemy* tc; if (Has) tc = Get; return tc;`): its slot is the dead argument
// slot that also holds the lookup key, so the absent path reads it back (`mov eax,[esp+10h]`); every
// return keeps its own exit (`mov al,1` / `xor al,al` / the ally test's al).
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

__forceinline CTCBase* GetTCIfPresent(const CThing& thing, long kind)
{
    CTCBase* tc;
    if (thing.HasInterface(kind))
        tc = thing.GetInterface(kind);
    return tc;
}

class CFaction { public:
    bool IsAlliedWith(const CFaction* other) const;         // 0x006B9460
};

class CTCEnemy { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[0x18 - 0x0a];                      // flags, EnemyThings, AllyThings (retail sizes)
    CFaction*     PFaction;                                 // +0x18
    bool IsEnemyOf(const CThing& thing) const;              // 0x0076CCE0
    bool IsAllyOf(const CThing& thing) const;
};

bool CTCEnemy::IsAllyOf(const CThing& thing) const
{
    if (&thing == ParentThing)
        return true;
    if (IsEnemyOf(thing))
        return false;
    CFaction* faction = PFaction;
    if (faction)
    {
        CFaction* otherFaction = ((const CTCEnemy*)GetTCIfPresent(thing, 0x49))->PFaction;
        if (otherFaction)
            return faction->IsAlliedWith(otherFaction);
    }
    return false;
}
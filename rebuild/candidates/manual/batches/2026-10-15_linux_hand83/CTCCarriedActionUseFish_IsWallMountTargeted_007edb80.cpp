// CTCCarriedActionUseFish::IsWallMountTargeted(CThing* user) -- true when the user has interface 8 and that component's current target (virtual slot 33) carries flag 0x4000000 at +0x38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EDB80, length 85 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0; the component's virtual slot 33
// (+0x84) returning the targeted thing. Called from the fish carried-action's slots 28/32 (0x007EDBFA,
// 0x007EDCE2) with `this` in ecx; `this` itself is unused (`ret 4`).
// Shape: the bitset test is the raw word (type 8 -> word 0 bit 8: `test ch,1`); the lookup key is
// homed in the dead `user` argument slot (`lea eax,[esp+8]`, no frame of its own). The component is
// used as `this` of a virtual call, and retail loads it straight into ecx (`mov ecx,[eax+4]`): that
// needs the entry selected by an explicit three-way `if (it == end) sel = end; else if (it->type <=
// type) sel = it; else sel = end;` (the landed 0x0089DFF0 form). The out-parameter GetInterface and
// a `(it == end || type < it->type) ? end : it` select both leave the component in eax and copy it.
struct CThingTarget {
    unsigned char _pad[0x38];
    unsigned long m_flags;                          // +0x38
};

struct CTCWallMountable {                           // the interface-8 component (class not established)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThingTarget* GetTarget();              // slot 33 (+0x84)
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCWallMountable* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
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
    virtual void v00();
    bool IsWallMountTargeted(CThing* user);
};

bool CTCCarriedActionUseFish::IsWallMountTargeted(CThing* user)
{
    if (user->HasInterface(8)) {
        CThingTarget* target = user->FindEntry(8)->component->GetTarget();
        if (target && (target->m_flags & 0x4000000))
            return true;
    }
    return false;
}

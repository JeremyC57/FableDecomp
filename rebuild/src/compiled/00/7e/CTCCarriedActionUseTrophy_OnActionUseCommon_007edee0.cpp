// CTCCarriedActionUseTrophy::OnActionUseCommon(CThingCreatureBase* user) -- a player aiming at a live wall mount (interface 0xda) while no trophy minigame runs hands the use to the wall-mount action; otherwise the trophy component handles it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EDEE0, 202 bytes (`ret 4`). Called by vfunc_28/vfunc_32 (landed 0x007EDFB0/0x007EDFD0).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined lookups), the user's virtual slot 77
// IsPlayer() (+0x134; CThingPlayerCreature 0x0052ABE0 returns 1, CThingAICreature 0x00835940 returns 0), the user's interface-8
// component's virtual slot 33 (+0x84, returns the thing it targets; name descriptive), the embedded wall-mount action's
// virtual slot 28 (+0x70, OnActionUse), CTCTrophy::OnTrophyUseAction @ 0x006DDD30.
// Layout: CTCCarriedActionUseTrophy (Ego_r.pdb 0x28) PTCCarriedActionUseWallMount +0x14 (an embedded CTCCarriedActionUseWallMount);
// CTCTrophy (interface 0x39, bit 25 of the +0x24 word) MinigameInProgress +0x14; CThing bitset +0x20 (type 8 = `test ah,1`,
// type 0xda = bit 26 of the +0x38 word), map +0x44, dead bit +0x91.
// Shape: the trophy lookup is the unchecked out-parameter GetInterface (the absent arm reads the never-written local from the
// dead `user` argument slot, GOTCHAS chunk 129); the target component is loaded straight into ecx (HasTC + one-ternary GetTC).
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    virtual void v000();                                // the polymorphic root holds the vptr
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_dead : 1;                           // +0x91 bit 0
    unsigned char m_91_1 : 7;
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCBase* GetTCSelect(long interfaceType)   // one ternary: the component goes straight to ecx
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
#define V(n) virtual void v##n();
    V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76)
    virtual bool IsPlayer() const;                      // slot 77 (+0x134)
};
struct CTCTargetSource {                                // the interface-8 component
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32)
    virtual CThing* GetTargetThing();                   // slot 33 (+0x84), name descriptive
};
struct CTCCarriedActionUseWallMount {                   // embedded action (CTCCarriedActionUseBase-derived)
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27)
    virtual void OnActionUse(CThingCreatureBase* user); // slot 28 (+0x70)
#undef V
};
struct CTCTrophy {
    unsigned char _pad00[0x14];
    bool MinigameInProgress;                            // +0x14
    void OnTrophyUseAction(CThingCreatureBase* user);   // 0x006DDD30
};
struct CTCCarriedActionUseTrophy {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x14 - 0x08];
    CTCCarriedActionUseWallMount PTCCarriedActionUseWallMount;   // +0x14
    void OnActionUseCommon(CThingCreatureBase* user);
};

void CTCCarriedActionUseTrophy::OnActionUseCommon(CThingCreatureBase* user)
{
    CTCTrophy* trophy;
    ParentThing->GetInterface(&trophy, 0x39);
    if (!trophy->MinigameInProgress && user->IsPlayer() && user->HasTC(8)) {
        CThing* target = ((CTCTargetSource*)user->GetTCSelect(8))->GetTargetThing();
        if (target && !target->m_dead && target->HasTC(0xda)) {
            PTCCarriedActionUseWallMount.OnActionUse(user);
            return;
        }
    }
    trophy->OnTrophyUseAction(user);
}
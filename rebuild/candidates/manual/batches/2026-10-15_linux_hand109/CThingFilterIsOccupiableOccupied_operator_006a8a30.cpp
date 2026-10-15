// CThingFilter_IsOccupiableOccupied::operator()(const CThing& thing) const -- whether the thing is a single-person occupiable (kind 0xac) that a live creature currently occupies; false otherwise
// Leaf: ??RCThingFilter_IsOccupiableOccupied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8A30, length 70 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation), the occupiable's
// IsOccupied @ 0x0087C3F0 (its occupier intelligent pointer at +0xc resolves and is not dead; manifest
// "IS_THING_ALIVE<...>" is propagated).
// Identity: Ego_r.pdb has `??RCThingFilter_IsOccupiableOccupied@@QBE_NABVCThing@@@Z`; the catalog's
// "CThingCreatureBase::IsBlocking" is a propagated label. Callers include CAIStateGroup_GoToBed::FindFreeBanditBed and the
// CIsFreeBed find_if (ecx = the filter object, `this` unused, `ret 4`).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: `HasTC(k)` then the pointer-returning inline find on the const thing (the component loaded straight into ecx:
// `mov ecx,[eax+4]`), the key converted into a frame temporary for the const LowerBound.
// `if (HasTC(k)) return GetTC(k)->IsOccupied(); return false;`.
struct CTCBase;

struct STCEntry {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (CVectorMap<ETCInterfaceType,...>::LowerBound)
    CTCBase* Find(long kind) const                 // the inlined map find: lower bound, then end when the key is absent
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
};

struct CTCSinglePersonOccupiable {              // component kind 0xac
    bool IsOccupied() const;                    // 0x0087C3F0
};

struct CThingFilter_IsOccupiableOccupied {
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsOccupiableOccupied::operator()(const CThing& thing) const
{
    if (thing.HasTC(0xac))
        return ((CTCSinglePersonOccupiable*)thing.GetTC(0xac))->IsOccupied();
    return false;
}

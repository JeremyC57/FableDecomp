// CThingCreatureBase::IsBlocking() const -- whether the creature's combat component (kind 0x30) says it is blocking; false without one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664D00, length 63 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation, key by address), the
// component's bool query @ 0x007331E0 (manifest "CTCInventoryBase::GetCanItemBeQuickAssigned" is propagated).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last}; engine/CThingCreatureBase.h is the PDB layout and names
// nothing at +0x20..+0x50, so a local CThing model is used.
// Shape: Ego_r.pdb `?IsBlocking@CThingCreatureBase@@QBE_NXZ`. `if (HasTC(k)) return GetTC(k)->f(); return false;` with
// the pointer-returning inline find (component straight into ecx), kind 0x30 = `test dword ptr [ecx+24h],10000h`.
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

struct CTCCombatBlocking {                      // component kind 0x30
    bool IsBlocking() const;                    // 0x007331E0
};

struct CThingCreatureBase : CThing {
    bool IsBlocking() const;
};

bool CThingCreatureBase::IsBlocking() const
{
    if (HasTC(0x30))
        return ((CTCCombatBlocking*)GetTC(0x30))->IsBlocking();
    return false;
}

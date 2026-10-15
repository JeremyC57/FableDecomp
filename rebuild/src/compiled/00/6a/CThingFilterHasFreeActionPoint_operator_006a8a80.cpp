// CThingFilter_HasFreeActionPoint::operator()(const CThing& thing) const -- whether the thing's action-point component (kind 0x6d) has a free action point
// Leaf: ??RCThingFilter_HasFreeActionPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8A80, length 73 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation), the component's bool
// query @ 0x00780400 (manifest "CTCFlammable::IsOnFire" is propagated).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: `HasTC(k)` then the pointer-returning inline find on the const thing (the component loaded straight into ecx:
// `mov ecx,[eax+4]`), the key converted into a frame temporary for the const LowerBound.
// Ego_r.pdb `??RCThingFilter_HasFreeActionPoint@@QBE_NABVCThing@@@Z`. `if (HasTC(k) && GetTC(k)->f()) return true;
// return false;` (two exits); kind 0x6d = `mov ecx,[eax+2Ch] ; test ch,20h`.
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

struct CTCActionPoints {                        // component kind 0x6d
    bool HasFreeActionPoint() const;            // 0x00780400
};

struct CThingFilter_HasFreeActionPoint {
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasFreeActionPoint::operator()(const CThing& thing) const
{
    if (thing.HasTC(0x6d) && ((CTCActionPoints*)thing.GetTC(0x6d))->HasFreeActionPoint())
        return true;
    return false;
}
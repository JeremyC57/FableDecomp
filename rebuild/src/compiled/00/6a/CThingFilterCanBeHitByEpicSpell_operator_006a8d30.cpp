// CThingFilter_CanBeHitByEpicSpell::operator()(const CThing& thing) const -- false when the thing's component 0x31 holds an object of type 0x40 or 0x41; true otherwise (also without the component)
// Leaf: ??RCThingFilter_CanBeHitByEpicSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8D30, length 87 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation), the component's
// object getter @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated), that object's virtual slot 1
// (+4, a type id).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: `HasTC(k)` then the pointer-returning inline find on the const thing (the component loaded straight into ecx:
// `mov ecx,[eax+4]`), the key converted into a frame temporary for the const LowerBound.
// Ego_r.pdb `??RCThingFilter_CanBeHitByEpicSpell@@QBE_NABVCThing@@@Z`. The type compares are `== 0x41 || == 0x40`
// sharing one `return false`, the true exit is the cold tail shared with the absent case.
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

struct CTypedObject {
    virtual void v000();
    virtual long GetType() const;               // slot 1 (+4)
};

struct CTCEpicSpellTarget {                     // component kind 0x31
    CTypedObject* GetObject() const;            // 0x0073A6E0
};

struct CThingFilter_CanBeHitByEpicSpell {
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_CanBeHitByEpicSpell::operator()(const CThing& thing) const
{
    if (thing.HasTC(0x31)) {
        long type = ((CTCEpicSpellTarget*)thing.GetTC(0x31))->GetObject()->GetType();
        if (type == 0x41 || type == 0x40)
            return false;
    }
    return true;
}
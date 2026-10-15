// CThingCreatureBase::OnKill() -- when the creature holds component 0x31, let that component drop its child primitives; then run the base thing's OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664370, length 70 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the thing's component map at +0x44, the key
// by address), the component's method @ 0x0073AE70 (manifest "CEnginePrimitiveRenderer::ClearAllChildPrimitives", a
// zero-argument __fastcall), CThing::OnKill @ 0x004C7CF0 (the base call).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind 0x31 = word 1 bit 17:
// `test dword ptr [edi+24h],20000h`), the sorted {kind, component} vector map at +0x44 {first, last}.
// Engine header: engine/CThingCreatureBase.h is the PDB layout and names nothing at +0x20..+0x50; a local model is used.
// Shape: Ego_r.pdb `?OnKill@CThingCreatureBase@@UAEXXZ`. `if (HasTC(k)) GetTC(k)->f();` with the inlined map find
// (`it == last || k < it->kind ? last : it`) whose component is loaded straight into ecx (the pointer-returning find,
// as the landed CGameScriptInterface::IsThingTurncoated 0x008A19B0), then the qualified base call.
struct CTCBase;

struct STCEntry {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // the thing's component map: a sorted vector of pairs
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);        // 0x0040F020
    CTCBase* Find(long kind)                    // the inlined map find: lower bound, then end when the key is absent
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};

struct CTCPrimitiveOwner {                      // component kind 0x31
    void ClearAllChildPrimitives();             // 0x0073AE70
};

struct CThing {
    virtual void v000();
    unsigned char  _pad04[0x1c];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) { return m_tcs.Find(kind); }
    void OnKill();                              // 0x004C7CF0
};

struct CThingCreatureBase : CThing {
    void OnKill();
};

void CThingCreatureBase::OnKill()
{
    if (HasTC(0x31))
        ((CTCPrimitiveOwner*)GetTC(0x31))->ClearAllChildPrimitives();
    CThing::OnKill();
}
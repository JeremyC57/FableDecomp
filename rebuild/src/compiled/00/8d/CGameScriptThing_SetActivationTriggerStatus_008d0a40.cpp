// CGameScriptThing::SetActivationTriggerStatus(bool) -- forwards the flag to virtual slot 28 (+0x70) of the scripted thing's kind-0x66 component when the thing is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0A40, length 128 bytes (padded).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls), the non-const component-map LowerBound @ 0x0040F020 (kind 0x66),
//          the component's virtual SetStatus (slot 28).
// Shape: the component comes from an inline accessor whose local stays UNSET when the mask bit is clear, so retail reads a dead
// slot on that path and emits the virtual call twice.
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: end entry when the key is absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase_Retail* GetTC(long kind) { return m_tcs.Find(kind); }
    __forceinline CTCBase_Retail* GetTC(long kind) const { return m_tcs.Find(kind); }
    template <class T> __forceinline bool GetInterface(T** out, long kind)   // the out-parameter lookup: the component stays in eax
    {
        if (HasTC(kind)) {
            *out = (T*)GetTC(kind);
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** out, long kind) const
    {
        if (HasTC(kind)) {
            *out = (T*)GetTC(kind);
            return true;
        }
        return false;
    }
    __forceinline CTCBase_Retail* GetTCIfPresent(long kind)   // the component, deliberately unset when the mask bit is clear (retail reads a dead slot)
    {
        CTCBase_Retail* tc;
        if (HasTC(kind))
            tc = GetTC(kind);
        return tc;
    }
    __forceinline const CTCBase_Retail* GetTCIfPresent(long kind) const
    {
        const CTCBase_Retail* tc;
        if (HasTC(kind))
            tc = GetTC(kind);
        return tc;
    }
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<CThing>: vptr + item (8 bytes)
    void*          m_pVtbl;
    void*          m_pItem;
    CThing_Retail* GetPItem() const;               // 0x00A01B50
};
struct CTCActivationTrigger_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetStatus(bool status);           // slot 28 (+0x70)
};
class CGameScriptThing { public:
    void*                   m_pVtbl;               // +0x00
    unsigned char           _pad04[8];
    CBaseIntelligentPointer PThing;                // +0x0c CIntelligentPointer<CThing>
    void SetActivationTriggerStatus(bool status);
};

void CGameScriptThing::SetActivationTriggerStatus(bool status)
{
    if (PThing.GetPItem() && !PThing.GetPItem()->m_dead)
        ((CTCActivationTrigger_Retail*)PThing.GetPItem()->GetTCIfPresent(0x66))->SetStatus(status);
}
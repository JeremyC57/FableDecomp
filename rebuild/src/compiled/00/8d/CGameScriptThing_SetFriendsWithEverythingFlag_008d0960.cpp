// CGameScriptThing::SetFriendsWithEverythingFlag(bool) -- stores the flag into byte +0x0c of the scripted thing's kind-0x49 component when the thing is alive and has one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0960, length 112 bytes (padded).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls), the non-const component-map LowerBound @ 0x0040F020 (kind 0x49).
// Shape: non-const method (the non-const LowerBound instantiation); mask word +0x28 bit 9 tested as `test ch,2`.
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
struct CTCFriendFlags_Retail {
    unsigned char  _pad00[0x0c];
    bool           m_friendsWithEverything;        // +0x0c
};
struct CGameScriptThing {
    void*                   m_pVtbl;               // +0x00
    unsigned char           _pad04[8];
    CBaseIntelligentPointer PThing;                // +0x0c CIntelligentPointer<CThing>
    void SetFriendsWithEverythingFlag(bool flag);
};

void CGameScriptThing::SetFriendsWithEverythingFlag(bool flag)
{
    if (PThing.GetPItem() && !PThing.GetPItem()->m_dead) {
        CTCFriendFlags_Retail* friends;
        if (PThing.GetPItem()->GetInterface(&friends, 0x49))
            friends->m_friendsWithEverything = flag;
    }
}
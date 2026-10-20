// CPlayerGui::DrawSaveScreen(long arg) -- when the GUI tracks a hero-centre thing, find it by UID and forward the call to its component 0x6A (virtual slot 11) if it has one
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the map Find written as the ternary entry pick (`((it == last || kind < it->kind) ? last : it)->tc`, the landed 0x00584157 idiom): the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438FF0, length 116 bytes, `ret 4`.
// Callees: the world's GetThingManager (virtual slot 3, +0x0c), CThingManager::GetThingWithUID(__int64) @ 0x0051ED80
// (manifest label CStopWatch::GetTicks is propagated), the component map's LowerBound @ 0x0040F020, the component's
// virtual slot 11 (+0x2c).
// Layout: CPlayerGui World +0x04 (engine/CPlayerGui.h), HeroCentreUID retail +0x2a8 (PDB +0x2b8); the thing's component
// mask at +0x20 (kind 0x6a = word 3 bit 10, `test ch,4` on +0x2c) and map at +0x44 (the chunk-141 model).
// Shape: `if (uid > 0)` on the unsigned 64-bit id (`ja`/`jbe` pair); the component comes from the inline accessor that
// leaves its result UNSET when the mask bit is clear (chunk 141), so the absent path reads a dead stack word.
struct CTCBase_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void DrawSaveScreen(long arg);                  // slot 11 (+0x2c)
};
struct STCEntry_Retail {                                    // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                             // +0x20
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                                   // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                                     // deliberately unset when the component is absent
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}

struct CThingManager_Retail {
    CThing_Retail* GetThingWithUID(unsigned __int64 uid);   // 0x0051ED80
};
struct CWorld_Retail {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager_Retail* GetThingManager();        // slot 3 (+0x0c)
};

struct CPlayerGui {
    void*            __vftable;                             // +0x000
    CWorld_Retail*   World;                                 // +0x004
    unsigned char    _pad008[0x2a8 - 0x008];
    unsigned __int64 HeroCentreUID;                         // +0x2a8
    void DrawSaveScreen(long arg);
};

void CPlayerGui::DrawSaveScreen(long arg)
{
    if (HeroCentreUID > 0) {
        CThing_Retail* thing = World->GetThingManager()->GetThingWithUID(HeroCentreUID);
        if (thing) {
            if (CTCBase_Retail* tc = GetTCIfPresent(thing, 0x6a))
                tc->DrawSaveScreen(arg);
        }
    }
}
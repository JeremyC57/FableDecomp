// CGameScriptInterface::void SetLightColour(const CScriptThing& thing, const CRGBColour& colour) const -- when the valid script thing's light component (kind 0x27) is switched on (+0x1d), set its colour (0x00640BB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089DF10, length 96 bytes.
// Callees: the script thing validated through its virtual slot 75 (+0x12c), the thing fetched through slot 11 (+0x2c); the component map (+0x44) LowerBound @ 0x0040F020 with the kind 39 by address (a stack temporary), the inlined `find` tail (`cmp end ; cmp key ; jle`), the light's SetColour @ 0x00640BB0.
// Shape: the map address in esi, the entry's value word at +4; the component-kind mask test folded onto the +0x24 word (bit 7).
// Leaf: ?SetLightColour@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCRGBColour@@@Z
struct CTCBase_Retail;
struct CRGBColour;
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long           m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the second instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key converted into a stack temporary), then end when the key is not there
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
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = the player creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word (a bool return would materialise `shr ; and 1`)
};
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }          // free inline: the pointer comes back in eax
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CTCLight_Retail {                           // the component (no vtable modelled: the bytes before the first used member are padding)
    void SetColour(const CRGBColour& colour);      // 0x00640BB0
    unsigned char _pad[0x1d];
    bool m_on;                                     // +0x1d
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
struct CPlayer_Retail {
    CThing_Retail* GetHeroCreature();              // 0x00487DC0 (the manifest label "_Cons_val<...>" is propagated)
    CThing_Retail* GetControlledCreature();        // 0x00487DD0 (the manifest label "_Cons_val<...>" is propagated)
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetMainPlayer();               // 0x00449970 (labelled "Catch@0040d959", propagated)
};
struct CDisplayEngine_Retail; struct CWorld;
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                 World;                           // +0x04 (engine/CGameScriptInterface.h)
    long                    m_08;
    CDisplayEngine_Retail*  DisplayEngine;                   // +0x0c
    long                    m_10;
    CPlayerManager_Retail*  PlayerManager;                   // +0x14
    unsigned char           _pad18[4];
    bool                    m_handLampLit;                   // +0x1c (read when the hero has no hand-lamp component)
    void SetLightColour(const CScriptThing& thing, const CRGBColour& colour) const;
};

void CGameScriptInterface::SetLightColour(const CScriptThing& thing, const CRGBColour& colour) const
{
    if (thing.IsValid()) {
        CThing_Retail* t = thing.GetThing();
        if (t->HasTC(0x27)) {
            CTCLight_Retail* tc = (CTCLight_Retail*)GetTC(t, 0x27);
            if (tc->m_on)
                tc->SetColour(colour);
        }
    }
}
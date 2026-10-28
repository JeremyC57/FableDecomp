// CGameScriptInterface::SetRegionExitAsActive(const CScriptThing& thing, bool active) const -- when the valid script thing holds a region-exit component (kind 100: +0x2c bit 4), set the active byte (+0xc) of the object at the thing's +0x5c and refresh the player GUI (global 0x013B8790, 0x004378C0 with 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089EEC0, length 64 bytes.
// Callees: the script thing's slots 75 (+0x12c) and 11 (+0x2c), the player-GUI refresh @ 0x004378C0 (on the global object; the label "_Cons_val<...>" is propagated).
// Shape: no map lookup (the object pointer sits at +0x5c), the kind mask test folded to `test byte ptr [eax+2Ch],10h`, `ret 8`.
struct CTCBase_Retail;
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
    unsigned char  _pad50[0xc];
    void*          m_pRegionLink;                  // +0x5c
    unsigned char  _pad60[0x31];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word (a bool return would materialise `shr ; and 1`)
};
class CThingPlayerCreature : public CThing_Retail { };
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }          // free inline: the pointer comes back in eax
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CRegionExit_Retail {
    unsigned char _pad[0xc];
    bool m_active;                                 // +0x0c
};
struct CPlayerGui_Retail {
    void Refresh(long flag);                       // 0x004378C0
};
extern CPlayerGui_Retail* g_pPlayerGui_013b8790;   // 0x013B8790
class CScriptThing { public:
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
struct CPlayer {
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0 (the manifest label "_Cons_val<...>" is propagated)
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0 (the manifest label "_Cons_val<...>" is propagated)
};
struct CPlayerManager_Retail {
    CPlayer* GetMainPlayer();               // 0x00449970 (labelled "Catch@0040d959", propagated)
};
struct CDisplayEngine_Retail; class CWorld;
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                 World;                           // +0x04 (engine/CGameScriptInterface.h)
    long                    Component;
    CDisplayEngine_Retail*  DisplayEngine;                   // +0x0c
    long                    DefinitionManager;
    CPlayerManager_Retail*  PlayerManager;                   // +0x14
    unsigned char           _pad18[4];
    bool                    m_handLampLit;                   // +0x1c (read when the hero has no hand-lamp component)
    void SetRegionExitAsActive(const CScriptThing& thing, bool active) const;
};

void CGameScriptInterface::SetRegionExitAsActive(const CScriptThing& thing, bool active) const
{
    if (thing.IsValid()) {
        CThing_Retail* t = thing.GetThing();
        if (t->HasTC(100)) {
            ((CRegionExit_Retail*)t->m_pRegionLink)->m_active = active;
            g_pPlayerGui_013b8790->Refresh(0);
        }
    }
}
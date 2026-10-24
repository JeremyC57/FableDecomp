// Grade: DIFFER(148v150) -- IMPOSSIBLE: retail rounds `max * fraction` with a bare `fistp dword ptr [esp+4]` after storing the product into the dead float argument slot and reloading it (`fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp`), the engine's inline-assembly ROUND helper (GOTCHAS 211/493, asm_bake family); `(long)x` compiles to __ftol2 and /QIfist to a qword fistp without the reload, so no compiled C++ reproduces it. Everything else (unchecked GetInterface on the hero's morality stats, interface 4) is ordinary.
// CGameScriptInterface::GiveHeroMorality(float fraction) const -- add fraction * the maximum morality to the hero's morality
// Retail 0x00899EF0, 150 bytes. Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @
// 0x00487DC0 (both called twice), the component-map LowerBound @ 0x0040F020 (interface 4, the hero stats; an
// unchecked lookup: retail reads the never-written component slot when the bit is clear), CTCHeroStats::
// GetMoralityMax @ 0x0057A67E, CTCHeroStats::SetMorality @ 0x0057C2E6 (Morality at +0x28).
// Leaf: GiveHeroMorality
// Flags: /QIfist

struct CTCBase;
struct STCEntry { long m_kind; CTCBase* m_pTC; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);    // 0x0040F020
    __forceinline CTCBase* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                     // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                           // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                      // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
struct CTCHeroStats {
    unsigned char _pad[0x28];
    long Morality;                                  // +0x28
    long GetMoralityMax();                          // 0x0057A67E
    void SetMorality(long morality);                // 0x0057C2E6
};
struct CPlayer { CThing* GetHeroCreature(); };       // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); }; // 0x00449970
struct CGameScriptInterface {
    virtual void v000();
    void* World; void* Component; void* DisplayEngine; void* DefinitionManager;
    CPlayerManager* PlayerManager;                  // +0x14
    virtual void GiveHeroMorality(float fraction) const;
};
__forceinline CTCBase* GetTCIfPresent(CThing* thing, long kind)
{
    CTCBase* tc;
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}
void CGameScriptInterface::GiveHeroMorality(float fraction) const
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroCreature();
    if (hero && !hero->m_dead)
    {
        CTCHeroStats* stats = (CTCHeroStats*)GetTCIfPresent(PlayerManager->GetMainPlayer()->GetHeroCreature(), 4);
        float amount = stats->GetMoralityMax() * fraction;
        stats->SetMorality(stats->Morality + (long)amount);
    }
}

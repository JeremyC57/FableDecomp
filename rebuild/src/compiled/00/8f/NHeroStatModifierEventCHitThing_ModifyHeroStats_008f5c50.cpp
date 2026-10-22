// CHitThing::ModifyHeroStats(CTCHeroStats* heroStats) -- credit a hero's hit: add the configured trainable-stat experience to the hero's experience component (kind 0x68) when enabled, and shift the hero's morality by the hit creature's punch or weapon-hit morality
// Leaf: ModifyHeroStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5C50, length 146 bytes.
// Callees: the component map's LowerBound @ 0x0040F020 (inlined `find` of kind 0x68), __ftol2 @ 0x00BFEA70,
// CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory, long) @ 0x006D8300,
// CTCHeroStats::SetMorality(long) @ 0x0057C2E6 (one call per arm).
// Layout = Ego_r.pdb CHitThing (24 bytes): vptr, StatsDefOfHitThing +4, Unarmed +8, StatToIncrement +0xc, DoStatIncrement
// +0x10, IncrementAmount +0x14; CCreatureStatsDef MoralityForPunch +0x2c / MoralityForWeaponHit +0x30; retail CTCHeroStats
// ParentThing +4, Morality +0x28 (PDB +0x30).
// Shape: the experience component comes through the inline accessor with an unset local (the absent path reads the
// dead argument slot); the morality sum is written def-first (`mov eax,[def+2Ch] ; add eax,[stats+28h]`).
struct CTCHeroExperience;
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long               m_kind;
    CTCHeroExperience* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCHeroExperience* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCHeroExperience* GetTCIfPresent(CThing* thing, long kind)
{
    CTCHeroExperience* tc;                         // deliberately unset when the component is absent
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}
struct CTCHeroExperience {
    void AddTrainableStatExperience(long stat, long amount);   // 0x006D8300
};
struct CTCHeroStats {
    void*         __vftable;                       // +0x00
    CThing*       ParentThing;                     // +0x04
    unsigned char _pad08[0x20];
    long          Morality;                        // +0x28
    void SetMorality(long morality);               // 0x0057C2E6
};
struct CCreatureStatsDef {
    unsigned char _pad[0x2c];
    long          MoralityForPunch;                // +0x2c
    long          MoralityForWeaponHit;            // +0x30
};
class CHitThing {
public:
    virtual void v00();
    CCreatureStatsDef* StatsDefOfHitThing;         // +0x04
    bool               Unarmed;                    // +0x08
    long               StatToIncrement;            // +0x0c
    bool               DoStatIncrement;            // +0x10
    float              IncrementAmount;            // +0x14
    void ModifyHeroStats(CTCHeroStats* heroStats);
};

void CHitThing::ModifyHeroStats(CTCHeroStats* heroStats)
{
    CTCHeroExperience* experience = GetTCIfPresent(heroStats->ParentThing, 0x68);
    if (DoStatIncrement)
        experience->AddTrainableStatExperience(StatToIncrement, (long)IncrementAmount);
    if (Unarmed)
        heroStats->SetMorality(StatsDefOfHitThing->MoralityForPunch + heroStats->Morality);
    else
        heroStats->SetMorality(StatsDefOfHitThing->MoralityForWeaponHit + heroStats->Morality);
}
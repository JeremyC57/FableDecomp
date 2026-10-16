// CTCGhostSword::ResumeForCutscene() -- bring every live minion back out of limbo after a cutscene (once)
// Leaf: ResumeForCutscene
//
// Retail 0x007c54f0, 112 bytes (plain `ret`). Ego_r.pdb `?ResumeForCutscene@CTCGhostSword@@QAEXXZ`. Callees:
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times per minion), CThing::SetInLimbo(bool) @
// 0x004C8CF0.
// Layout: Ego_r.pdb CTCGhostSword with retail 12-byte vectors: PDef +0xc, Minions
// (vector<CIntelligentPointer<CThingAICreature>>) +0x10, MinionEffects +0x1c, MinionAliveTimer +0x28,
// FadeInTimer +0x2c, CurFadeInTimer +0x30, SpellLevel +0x3c, PItemRemoved +0x40, CutSceneCancelled +0x48.
// Shape: the landed CTCBoss::ClearEffects loop (0x007AEF60) with a SIGNED index and count
// (`(long)Minions.size()` gives `test eax,0FFFFFFF8h ; jle` and `jl`): operator[] as `*(begin() + i)`,
// the inline IsThingAlive on the element address (esi), the call re-reading begin (`lea ecx,[eax+ebp]`).
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Flags91;                                  // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void SetInLimbo(bool inLimbo);                          // 0x004C8CF0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};

struct CTCGhostSword {
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    void*         PDef;                                     // +0x0c CDefPointer<const CSpecialAbilitiesGhostSwordDef>
    CRetailVector<CBaseIntelligentPointer> Minions;         // +0x10
    CRetailVector<CBaseIntelligentPointer> MinionEffects;   // +0x1c
    long          MinionAliveTimer;                         // +0x28
    long          FadeInTimer;                              // +0x2c
    CRetailVector<long> CurFadeInTimer;                     // +0x30
    long          SpellLevel;                               // +0x3c
    unsigned char PItemRemoved[8];                          // +0x40 CIntelligentPointer<CThing>
    bool          CutSceneCancelled;                        // +0x48
    void ResumeForCutscene();
};

void CTCGhostSword::ResumeForCutscene()
{
    if (CutSceneCancelled)
    {
        CutSceneCancelled = false;
        for (long i = 0; i < (long)Minions.size(); ++i)
        {
            if (IsThingAlive(Minions[i]))
                Minions[i].GetPItem()->SetInLimbo(false);
        }
    }
}
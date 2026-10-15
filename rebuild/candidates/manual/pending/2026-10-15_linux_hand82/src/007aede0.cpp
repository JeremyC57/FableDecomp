// CTCBoss::IsOnFinalPhase() const -- true when the boss has no def or its current phase index has run past the def's phase-health list
//
// Retail 0x007AEDE0, 33 bytes. No callees. PBossDef (+0x0c) is the CDefPointer<const CBossDef>
// object pointer; CBossDef::PhaseHealthPercent is the retail 12-byte vector<long> {first, last, end}
// at +0x28 (Ego_r.pdb puts it at +0x28 too, with a 16-byte vector). currentBossPhase is +0x18.
// Shape: null def -> `mov al,1`; `phase >= size()` is an UNSIGNED compare (`jae`, size() is a
// size_t pointer difference `sub ; sar 2`) that shares the `mov al,1` tail; the in-range case
// falls through to `xor al,al`.

template <class T>
struct CRetailVector                                // retail std::vector: {first, last, end}
{
    T* _First;
    T* _Last;
    T* _End;
    unsigned int size() const { return _Last - _First; }
};

struct CBossDef
{
    unsigned char       _pad_0x00[0x28];
    CRetailVector<long> PhaseHealthPercent;         // +0x28
};

#pragma pack(push, 1)
struct CTCBoss
{
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    const CBossDef* PBossDef;                       // +0x0c CDefPointer<const CBossDef>::Object
    long          GenericCounter;                   // +0x10
    long          StrikesSinceHit;                  // +0x14
    long          currentBossPhase;                 // +0x18

    bool IsOnFinalPhase() const;
};
#pragma pack(pop)

bool CTCBoss::IsOnFinalPhase() const
{
    if (PBossDef != 0)
    {
        if ((unsigned int)currentBossPhase < PBossDef->PhaseHealthPercent.size())
            return false;
    }
    return true;
}

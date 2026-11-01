// CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const -- the tolerance to being hit (the override when set, else the
//   reaction manager's tolerance for this thing plus the personality's) minus the hits counted so far
// Leaf: ?GetToleranceToBeingHitRemaining@CTCCreatureOpinionOfHero@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681980, 65 bytes (padded to 80).
// Identity (naming wave 25): Ego_r.pdb / FableWin.pdb order GetToleranceToBeingHit / GetToleranceToBeingHitRemaining / OnHit, as retail
//   0x00681940 / 0x00681980 / 0x006819D0; the body is GetToleranceToBeingHit (0x00681940) inlined with `fsub [esi+10h]`
//   (NumberOfTimesHit) on both paths. Was CViewBase::CalculateDisplayDistance over a CViewBase / CDisplayEngine model of the same fields
//   (m_pThing +4 = ParentThing, m_nearOffset +0x10 = NumberOfTimesHit, m_distance +0x14 = ToleranceToBeingHitOverride, m_pTarget +0x20 =
//   PPersonalityDef with +0x50 ToleranceToBeingHit) and 0x00686A80 as CViewBase::GetDisplayEngine: it is
//   ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (DECISIONS_W24_RESOLVED item 7) and the manager is CWorld +0x74 POpinionReactionManager.
// Callees: PeekWorld 0x00686A80, COpinionReactionManager::GetToleranceToBeingHit(const CThing*) const 0x00701940.
// Shape: as 0x00681940 -- ParentThing is a named local read before the PeekWorld call (edi), the manager a local (its call precedes the
//   push); the personality's tolerance is added only when the def is present.
class CThing;
class COpinionReactionManager { public:
    float GetToleranceToBeingHit(const CThing* thing) const;        // 0x00701940
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74 CCountedPointer<COpinionReactionManager> data
};
class COpinionPersonalityDef { public:
    unsigned char _pad00[0x50];
    float ToleranceToBeingHit;                     // +0x50
};
class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCCreatureOpinionOfHero : public CTCBase { public:
    void*   __vftable;                             // +0x00 (CTCBase)
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    long    LastOpinionReactionFrame;              // +0x0c
    float   NumberOfTimesHit;                      // +0x10
    float   ToleranceToBeingHitOverride;           // +0x14
    long    FrameToDecayNumberOfTimesHit;          // +0x18
    void*   PDef;                                  // +0x1c CDefPointer<COpinionOfHeroDef const>
    COpinionPersonalityDef* PPersonalityDef;       // +0x20 CDefPointer<COpinionPersonalityDef const>
    float GetToleranceToBeingHit() const           // 0x00681940 (landed; inline here)
    {
        if (ToleranceToBeingHitOverride >= 0.0f)
            return ToleranceToBeingHitOverride;
        CThing* thing = ParentThing;
        COpinionReactionManager* manager = PeekWorld().POpinionReactionManager;
        float tolerance = manager->GetToleranceToBeingHit(thing);
        if (PPersonalityDef)
            tolerance += PPersonalityDef->ToleranceToBeingHit;
        return tolerance;
    }
    float GetToleranceToBeingHitRemaining() const;
};
float CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const
{
    return GetToleranceToBeingHit() - NumberOfTimesHit;
}

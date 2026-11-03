p='hdr/engine/CTCWife.h'; s=open(p).read()
old="""    bool IsAttractedToHero() const;                          // 0x006CFE80 ?IsAttractedToHero@CTCWife@@QBE_NXZ
"""
new=old+"""    // Chunk 728 round 2: the non-virtual members GlobalUpdate / FrameUpdate call, Ego_r.pdb spellings (FableWin.pdb agrees where
    // it lists them), retail addresses from the manifest's CTCWife run (same source order as Ego_r 0x00703AA0..0x00707290).
    void SendPauseEvent(bool pause);                         // 0x006CEBF0 ?SendPauseEvent@CTCWife@@QAEX_N@Z
    void InitWifeAtEndOfMarriageFrescoStartFadeIn();         // 0x006CF3B0 ?InitWifeAtEndOfMarriageFrescoStartFadeIn@CTCWife@@QAEXXZ
    void InitWifeAtEndOfMarriageFrescoEndFadeIn();           // 0x006CF480 ?InitWifeAtEndOfMarriageFrescoEndFadeIn@CTCWife@@QAEXXZ
    bool IsCourtableAndNotBlockingIt() const;                // 0x006CF810 ?IsCourtableAndNotBlockingIt@CTCWife@@QBE_NXZ
    bool IsCourtingHero() const;                             // 0x006CF890 ?IsCourtingHero@CTCWife@@QBE_NXZ
    bool IsInLoveWithHero() const;                           // 0x006CF900 ?IsInLoveWithHero@CTCWife@@QBE_NXZ
    void HaveSex();                                          // 0x006CF920 ?HaveSex@CTCWife@@QAEXXZ
    void OnHeroViolence();                                   // 0x006CFDA0 ?OnHeroViolence@CTCWife@@QAEXXZ
    void Divorce();                                          // 0x006D0180 ?Divorce@CTCWife@@QAEXXZ
    void InitWifeNonpersistentState();                       // 0x006D03E0 ?InitWifeNonpersistentState@CTCWife@@QAEXXZ
"""
assert s.count(old)==1; s=s.replace(old,new)
old2="""private:
    static const CCharString CourtingAdviceGroupNames[4];
};"""
new2="""private:
    float GetNumHoursMarriedFor() const;                     // 0x006CEC90 ?GetNumHoursMarriedFor@CTCWife@@ABEMXZ
    void FindAppropriateGift();                              // 0x006CF530 ?FindAppropriateGift@CTCWife@@AAEXXZ
    void SetAttractionIcon(bool show, float value, unsigned char type);   // 0x006CFC90 ?SetAttractionIcon@CTCWife@@AAEX_NME@Z
    void CheckAppearanceChanges(CThingPlayerCreature& hero); // 0x006D1250 ?CheckAppearanceChanges@CTCWife@@AAEXAAVCThingPlayerCreature@@@Z
    static const CCharString CourtingAdviceGroupNames[4];
};"""
assert s.count(old2)==1; s=s.replace(old2,new2)
old3="class CHeroMarriageDef;\nenum EOpinionAttitudeType;"
assert s.count(old3)==1; s=s.replace(old3,"class CHeroMarriageDef;\nclass CThingPlayerCreature;\nenum EOpinionAttitudeType;")
open(p,'w').write(s)

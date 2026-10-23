// CTCCreatureGroupBoastingCrowd::EnterCrowdState(long state) -- leave the current crowd state (pop one pushed hero opinion
// source, remove the pending opinion deed from the hero's log) and enter the new one: push its opinion source on the hero
// and log its opinion deed (state 0 kills the crowd thing); nothing happens when already in the state or in state 10
// Leaf: EnterCrowdState
//
// Retail 0x00784260, 614 bytes (`ret 4`, jump table at 0x007844C8).
// Callees: CTCBase::GetWorld @ 0x00686A70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, COpinionReactionManager::
// ClearOpinionSource @ 0x007008C0 / AddOpinionSource @ 0x00700780, CVectorMap::LowerBound @ 0x0040F020 (the hero's deed
// log 0x51), CTCHeroOpinionDeedLog::RemoveDeed @ 0x006C5920 / AddDeed @ 0x006C6990, CCharString(const char*, long)
// @ 0x0099EBF0 / operator=(const char*) @ 0x0099EFE0 / operator!=(const char*) @ 0x0099E960 / dtor @ 0x0099EAE0,
// CThing::Kill(bool) @ 0x004C9B80, CThing::GetPosition @ 0x004C73D0.
// Layout: as the landed FrameUpdate (0x00784500): PHero +0x24, CrowdState +0x2c, CurrentOpinionDeedHandle +0x38,
// HeroOpinionSourcesPushed +0x3c; CWorld opinion reaction manager +0x74.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const char* text);                // 0x0099EFE0
    bool operator!=(const char* text) const;                 // 0x0099E960
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                                   // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                             // 0x004C9B80
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct COpinionSourceParams {                                // 4 bytes, left uninitialised (retail homes it in the dead lookup-key slot)
    long Reserved;
    COpinionSourceParams() { }
};
struct COpinionReactionManager {
    void ClearOpinionSource(CThing* source, bool immediately);   // 0x007008C0
    bool AddOpinionSource(CThing* source, const CCharString& name, const COpinionSourceParams& params);   // 0x00700780
};
struct CWorld {
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;        // +0x74
};
struct CTCHeroOpinionDeedLog {
    void RemoveDeed(long handle);                            // 0x006C5920
    long AddDeed(long type, const C3DVector& position, long a, long b, long c, long d, long e, long f, long g, long h);   // 0x006C6990
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                           // +0x04
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CTCCreatureGroupBoastingCrowd {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04 (CTCBase)
    unsigned char _pad08[0x24 - 0x08];
    CBaseIntelligentPointer PHero;                           // +0x24
    long CrowdState;                                         // +0x2c
    long CurrentEncouragementState;                          // +0x30
    bool HeroBoasted;                                        // +0x34
    bool HeroMadeGoodBoast;                                  // +0x35
    long CurrentOpinionDeedHandle;                           // +0x38
    long HeroOpinionSourcesPushed;                           // +0x3c
    long FrameToClearRemainingOpinionSources;                // +0x40
    long FrameToLeavePostBoastState;                         // +0x44
    CWorld* GetWorld();                                      // 0x00686A70 (CTCBase)
    void EnterCrowdState(long state);
};

void CTCCreatureGroupBoastingCrowd::EnterCrowdState(long state)
{
    if (CrowdState == state || CrowdState == 10)
        return;
    if (HeroOpinionSourcesPushed > 0) {
        COpinionReactionManager* manager = GetWorld()->POpinionReactionManager;
        manager->ClearOpinionSource(PHero.GetPItem(), true);
        --HeroOpinionSourcesPushed;
    }
    if (CurrentOpinionDeedHandle != -1) {
        CTCHeroOpinionDeedLog* log;
        if (PHero.GetPItem() && !PHero.GetPItem()->IsDead() && PHero.GetPItem()->GetInterface(&log, 0x51))
            log->RemoveDeed(CurrentOpinionDeedHandle);
        CurrentOpinionDeedHandle = -1;
    }
    CCharString sourceName("");
    long deedType = 0;
    switch (state) {
    case 0:
        ParentThing->Kill(true);
        break;
    case 1:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ANTICIPATION";
        deedType = 0x40;
        break;
    case 2:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT";
        deedType = 0x41;
        break;
    case 3:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT";
        deedType = 0x42;
        break;
    case 4:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ENCOURAGEMENT";
        deedType = 0x43;
        break;
    case 5:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_POST_BOAST_GOOD";
        break;
    case 6:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_POST_BOAST_EVIL";
        break;
    case 7:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_WELL_WISHES";
        deedType = 0x44;
        break;
    case 8:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ILL_WISHES";
        break;
    case 9:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_ANNOYED_NO_BOASTING";
        deedType = 0x45;
        break;
    case 10:
        sourceName = "OPINION_SOURCE_HERO_BOASTING_HERO_ATTACKED_CROWD";
        break;
    }
    if (sourceName != "") {
        COpinionReactionManager* manager = GetWorld()->POpinionReactionManager;
        if (manager->AddOpinionSource(PHero.GetPItem(), sourceName, COpinionSourceParams()))
            ++HeroOpinionSourcesPushed;
    }
    if (deedType) {
        CTCHeroOpinionDeedLog* log;
        if (PHero.GetPItem() && !PHero.GetPItem()->IsDead() && PHero.GetPItem()->GetInterface(&log, 0x51))
            CurrentOpinionDeedHandle = log->AddDeed(deedType, PHero.GetPItem()->GetPosition(), 0, 0, 2, 0, 1, 0, 0, 0);
    }
    CrowdState = state;
}

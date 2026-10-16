// CTCQuestCard::CTCQuestCard(CThing* thing) -- CTCBase(thing), null quest-card def, flags cleared, FeatIndex -1, five empty strings, then the def is fetched from the thing's def and the gold/renown rewards copied
// Leaf: ??0CTCQuestCard
// Retail: Fable.exe 0x007026A0, 114 bytes (`ret 4`). Vtable 0x01264C44 = CTCQuestCard (rtti-vtables slot 1 0x00702720).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CCharString::CCharString() @ 0x0099E4B0 (x5),
// CDefClassBase::GetSubDef<CQuestCardDef>(CDefPointer<const CQuestCardDef>*) @ 0x00702930 (manifest label
// "GetSubDef<CAICreatureWillPowerIndicatorDef>" is a byte-twin propagation).
// Layout (engine/CTCQuestCard.h, PDB = retail): PDef +0x0C, Finished +0x10, Replayable +0x11, FeatIndex +0x14,
// QuestName/ObjectiveName/RegionName/TeleporterRegionName/ScriptQuestName +0x18..+0x28, GoldReward +0x2C,
// RenownReward +0x30; the parent thing's def pointer +0x70. CQuestCardDef: GoldReward +0x48, RenownReward +0x44.
// Shape: GetSubDef writes straight into the PDef member; the rewards are read back through PDef.Object.
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
};
struct CDefPointeeBase {
    virtual void Destroy();
    long m_refCount;                                // +0x04
};
template <class T> struct CDefPointer {
    const T* Object;
    CDefPointer() : Object(0) { }
    const T* operator->() const { return Object; }
};
struct CQuestCardDef : CDefPointeeBase {
    unsigned char _pad08[0x44 - 0x08];
    long RenownReward;                              // +0x44
    long GoldReward;                                // +0x48
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CQuestCardDef>* out);   // 0x00702930
};
struct CThingR {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                            // +0x70
};
struct CTCBase {
    virtual void v00();
    CThingR* ParentThing;                           // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
struct CTCQuestCard : CTCBase {                     // vtable 0x01264C44
    CDefPointer<CQuestCardDef> PDef;                // +0x0c
    bool Finished;                                  // +0x10
    bool Replayable;                                // +0x11
    long FeatIndex;                                 // +0x14
    CCharString QuestName;                          // +0x18
    CCharString ObjectiveName;                      // +0x1c
    CCharString RegionName;                         // +0x20
    CCharString TeleporterRegionName;               // +0x24
    CCharString ScriptQuestName;                    // +0x28
    long GoldReward;                                // +0x2c
    long RenownReward;                              // +0x30
    virtual void v00();
    CTCQuestCard(CThing* thing);
};

CTCQuestCard::CTCQuestCard(CThing* thing)
    : CTCBase(thing)
    , Finished(false)
    , Replayable(false)
    , FeatIndex(-1)
{
    ParentThing->PDef->GetSubDef(&PDef);
    GoldReward = PDef->GoldReward;
    RenownReward = PDef->RenownReward;
}
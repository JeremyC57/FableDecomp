// CVillagePeopleDef::operator=(const CVillagePeopleDef&) -- memberwise def copy: CDefClassBase part out of line, Enable, the villager weights and creature-type indices, two vector<long> respawn tables by their operator=, the renown offset; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045473F, length 327 bytes (odd address: size-optimised def TU).
// Pragma: #pragma optimize("s", on).
// Callees: CDefClassBase::operator= @ 0x00431F10 (catalogued "CThingBuildingDef::operator=", propagated),
//          vector<long>::operator= @ 0x00454886 (same TU, right after this body; manifest "CActiveFile::operator=", propagated).
// Identity: the catalog label "CThingBuildingDef::operator=" is propagated. Vtable 0x01232C4C (CVillagePeopleDef, RTTI)
// slot 19 (Copy) is the 5-byte `jmp` thunk at 0x0045473A right before this body.
// Layout: no engine header for CVillagePeopleDef; Ego_r.pdb (struct_layouts_egor.tsv) gives Enable at +0x25 (right after
// the 0x25-byte CDefClassBase) and the scalars +0x28..+0xa4 exactly as copied. The PDB vectors are 16 bytes (+0xa8/+0xb8),
// retail STLport vectors are 12 (+0xa8/+0xb4), which puts GuardRespawnMaxRenownBasedOffset at retail +0xc0 (PDB +0xc8).
#pragma pack(push, 1)
struct CDefClassBase {
    virtual void Copy(const CDefClassBase& other);
    unsigned char _base_0x04[0x18];
    void*         PDefManager;                          // +0x1c
    unsigned long GlobalIndex;                          // +0x20
    unsigned char DefaultValsApplied;                   // +0x24 bitfield group
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)

struct CLongVector {                                    // STLport vector<long> (12 bytes in retail)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CVillagePeopleDef : CDefClassBase {              // vtable 0x01232C4C
    bool        Enable;                                 // +0x25
    float       DesiredMaleWeight;                      // +0x28
    float       DesiredFemaleWeight;                    // +0x2c
    float       DesiredChildWeight;                     // +0x30
    float       ProbabilityOfHomsexualityPerDialogueEnabledMan; // +0x34
    long        MaleTeacher;                            // +0x38
    long        FemaleTeacher;                          // +0x3c
    long        MaleShopKeeper;                         // +0x40
    long        FemaleShopKeeper;                       // +0x44
    long        MaleMarketTrader;                       // +0x48
    long        FemaleMarketTrader;                     // +0x4c
    long        MaleBarman;                             // +0x50
    long        FemaleBarman;                           // +0x54
    long        MaleBarmaid;                            // +0x58
    long        FemaleBarmaid;                          // +0x5c
    long        MaleWarehouseWorker;                    // +0x60
    long        FemaleWarehouseWorker;                  // +0x64
    long        MaleServant;                            // +0x68
    long        FemaleServant;                          // +0x6c
    long        MaleGoodApprentice;                     // +0x70
    long        FemaleGoodApprentice;                   // +0x74
    long        MaleEvilApprentice;                     // +0x78
    long        FemaleEvilApprentice;                   // +0x7c
    long        MaleUnemployed;                         // +0x80
    long        FemaleUnemployed;                       // +0x84
    long        MaleChild;                              // +0x88
    long        FemaleChild;                            // +0x8c
    long        GamesMaster;                            // +0x90
    long        Guard;                                  // +0x94
    long        MaleGhost;                              // +0x98
    long        FemaleGhost;                            // +0x9c
    long        NumGuards;                              // +0xa0
    long        NumGhosts;                              // +0xa4
    CLongVector GuardRespawnType;                       // +0xa8
    CLongVector GuardRespawnQuantity;                   // +0xb4
    long        GuardRespawnMaxRenownBasedOffset;       // +0xc0
    CVillagePeopleDef& operator=(const CVillagePeopleDef& other);
};

CVillagePeopleDef& CVillagePeopleDef::operator=(const CVillagePeopleDef& other)
{
    CDefClassBase::operator=(other);
    Enable = other.Enable;
    DesiredMaleWeight = other.DesiredMaleWeight;
    DesiredFemaleWeight = other.DesiredFemaleWeight;
    DesiredChildWeight = other.DesiredChildWeight;
    ProbabilityOfHomsexualityPerDialogueEnabledMan = other.ProbabilityOfHomsexualityPerDialogueEnabledMan;
    MaleTeacher = other.MaleTeacher;
    FemaleTeacher = other.FemaleTeacher;
    MaleShopKeeper = other.MaleShopKeeper;
    FemaleShopKeeper = other.FemaleShopKeeper;
    MaleMarketTrader = other.MaleMarketTrader;
    FemaleMarketTrader = other.FemaleMarketTrader;
    MaleBarman = other.MaleBarman;
    FemaleBarman = other.FemaleBarman;
    MaleBarmaid = other.MaleBarmaid;
    FemaleBarmaid = other.FemaleBarmaid;
    MaleWarehouseWorker = other.MaleWarehouseWorker;
    FemaleWarehouseWorker = other.FemaleWarehouseWorker;
    MaleServant = other.MaleServant;
    FemaleServant = other.FemaleServant;
    MaleGoodApprentice = other.MaleGoodApprentice;
    FemaleGoodApprentice = other.FemaleGoodApprentice;
    MaleEvilApprentice = other.MaleEvilApprentice;
    FemaleEvilApprentice = other.FemaleEvilApprentice;
    MaleUnemployed = other.MaleUnemployed;
    FemaleUnemployed = other.FemaleUnemployed;
    MaleChild = other.MaleChild;
    FemaleChild = other.FemaleChild;
    GamesMaster = other.GamesMaster;
    Guard = other.Guard;
    MaleGhost = other.MaleGhost;
    FemaleGhost = other.FemaleGhost;
    NumGuards = other.NumGuards;
    NumGhosts = other.NumGhosts;
    GuardRespawnType = other.GuardRespawnType;
    GuardRespawnQuantity = other.GuardRespawnQuantity;
    GuardRespawnMaxRenownBasedOffset = other.GuardRespawnMaxRenownBasedOffset;
    return *this;
}

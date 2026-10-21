// CVillagePeopleDef::Transfer(CPersistContext&) -- persist the village population def fields in declaration order
// Leaf: ?Transfer@CVillagePeopleDef@@UAEXAAVCPersistContext@@@Z
// Retail 0x0046A80E, 448 bytes, size-optimised def TU (#pragma optimize("s")): unaligned address, no padding.
// One CPersistContext::Transfer overload per member type:
//   0x0043314A Transfer(bool&), 0x00431061 Transfer(float&), 0x00431102 Transfer(long&),
//   0x00466A47 Transfer(vector<long>&). (Manifest labels on these callees are propagated and wrong.)
// Layout: Ego_r.pdb names and types. This def's base ends at +0x25 (Enable sits at its PDB offset 0x25), and the
//   two STLport vectors are 12 bytes in retail (PDB 16), so GuardRespawnQuantity is +0xb4 and the last long +0xc0.
//   Local model: no engine/ header exists for CVillagePeopleDef or CPersistContext.
// Shape: VC7.1 forms the last address with `add esi,0C0h ; push esi` because `this` is dead after it.
#pragma optimize("s", on)

template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct CPersistContext {
    void Transfer(bool& v);                 // 0x0043314A
    void Transfer(float& v);                // 0x00431061
    void Transfer(long& v);                 // 0x00431102
    void Transfer(CStlVector<long>& v);     // 0x00466A47
};

struct CVillagePeopleDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x21];              // CDefClassBase fields (+0x04)
    bool               Enable;                                          // +0x25
    float              DesiredMaleWeight;                               // +0x28
    float              DesiredFemaleWeight;                             // +0x2c
    float              DesiredChildWeight;                              // +0x30
    float              ProbabilityOfHomsexualityPerDialogueEnabledMan;  // +0x34
    long               MaleTeacher;                                     // +0x38
    long               FemaleTeacher;                                   // +0x3c
    long               MaleShopKeeper;                                  // +0x40
    long               FemaleShopKeeper;                                // +0x44
    long               MaleMarketTrader;                                // +0x48
    long               FemaleMarketTrader;                              // +0x4c
    long               MaleBarman;                                      // +0x50
    long               FemaleBarman;                                    // +0x54
    long               MaleBarmaid;                                     // +0x58
    long               FemaleBarmaid;                                   // +0x5c
    long               MaleWarehouseWorker;                             // +0x60
    long               FemaleWarehouseWorker;                           // +0x64
    long               MaleServant;                                     // +0x68
    long               FemaleServant;                                   // +0x6c
    long               MaleGoodApprentice;                              // +0x70
    long               FemaleGoodApprentice;                            // +0x74
    long               MaleEvilApprentice;                              // +0x78
    long               FemaleEvilApprentice;                            // +0x7c
    long               MaleUnemployed;                                  // +0x80
    long               FemaleUnemployed;                                // +0x84
    long               MaleChild;                                       // +0x88
    long               FemaleChild;                                     // +0x8c
    long               GamesMaster;                                     // +0x90
    long               Guard;                                           // +0x94
    long               MaleGhost;                                       // +0x98
    long               FemaleGhost;                                     // +0x9c
    long               NumGuards;                                       // +0xa0
    long               NumGhosts;                                       // +0xa4
    CStlVector<long>   GuardRespawnType;                                // +0xa8
    CStlVector<long>   GuardRespawnQuantity;                            // +0xb4
    long               GuardRespawnMaxRenownBasedOffset;                // +0xc0
};

void CVillagePeopleDef::Transfer(CPersistContext& context)
{
    context.Transfer(Enable);
    context.Transfer(DesiredMaleWeight);
    context.Transfer(DesiredFemaleWeight);
    context.Transfer(DesiredChildWeight);
    context.Transfer(ProbabilityOfHomsexualityPerDialogueEnabledMan);
    context.Transfer(MaleTeacher);
    context.Transfer(FemaleTeacher);
    context.Transfer(MaleShopKeeper);
    context.Transfer(FemaleShopKeeper);
    context.Transfer(MaleMarketTrader);
    context.Transfer(FemaleMarketTrader);
    context.Transfer(MaleBarman);
    context.Transfer(FemaleBarman);
    context.Transfer(MaleBarmaid);
    context.Transfer(FemaleBarmaid);
    context.Transfer(MaleWarehouseWorker);
    context.Transfer(FemaleWarehouseWorker);
    context.Transfer(MaleServant);
    context.Transfer(FemaleServant);
    context.Transfer(MaleGoodApprentice);
    context.Transfer(FemaleGoodApprentice);
    context.Transfer(MaleEvilApprentice);
    context.Transfer(FemaleEvilApprentice);
    context.Transfer(MaleUnemployed);
    context.Transfer(FemaleUnemployed);
    context.Transfer(MaleChild);
    context.Transfer(FemaleChild);
    context.Transfer(GamesMaster);
    context.Transfer(Guard);
    context.Transfer(MaleGhost);
    context.Transfer(FemaleGhost);
    context.Transfer(NumGuards);
    context.Transfer(NumGhosts);
    context.Transfer(GuardRespawnType);
    context.Transfer(GuardRespawnQuantity);
    context.Transfer(GuardRespawnMaxRenownBasedOffset);
}
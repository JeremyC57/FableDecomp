#include <cstdio>
// behaviour test: CVillagePeopleDef::Transfer visits every field once, in retail order, through the overload of its type

template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct CPersistContext {
    const void* order[64];
    int kind[64];
    int count;
    void Transfer(CStlVector<long>& v) { kind[count] = 1; order[count++] = &v; }
    void Transfer(bool& v) { kind[count] = 2; order[count++] = &v; }
    void Transfer(float& v) { kind[count] = 3; order[count++] = &v; }
    void Transfer(long& v) { kind[count] = 4; order[count++] = &v; }
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

int main()
{
    static CVillagePeopleDef def;
    CPersistContext ctx; ctx.count = 0;
    def.Transfer(ctx);
    const void* expect[36] = { &def.Enable, &def.DesiredMaleWeight, &def.DesiredFemaleWeight, &def.DesiredChildWeight, &def.ProbabilityOfHomsexualityPerDialogueEnabledMan, &def.MaleTeacher, &def.FemaleTeacher, &def.MaleShopKeeper, &def.FemaleShopKeeper, &def.MaleMarketTrader, &def.FemaleMarketTrader, &def.MaleBarman, &def.FemaleBarman, &def.MaleBarmaid, &def.FemaleBarmaid, &def.MaleWarehouseWorker, &def.FemaleWarehouseWorker, &def.MaleServant, &def.FemaleServant, &def.MaleGoodApprentice, &def.FemaleGoodApprentice, &def.MaleEvilApprentice, &def.FemaleEvilApprentice, &def.MaleUnemployed, &def.FemaleUnemployed, &def.MaleChild, &def.FemaleChild, &def.GamesMaster, &def.Guard, &def.MaleGhost, &def.FemaleGhost, &def.NumGuards, &def.NumGhosts, &def.GuardRespawnType, &def.GuardRespawnQuantity, &def.GuardRespawnMaxRenownBasedOffset };
    const int kinds[36] = { 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 4 };
    bool ok = ctx.count == 36;
    for (int i = 0; ok && i < 36; ++i)
        ok = ctx.order[i] == expect[i] && ctx.kind[i] == kinds[i];
    if (!ok) { std::printf("0046a80e_TEST FAIL\n"); return 1; }
    std::printf("0046a80e_TEST PASS\n");
    return 0;
}

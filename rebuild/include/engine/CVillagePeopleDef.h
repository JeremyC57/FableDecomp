#ifndef FABLE_ENGINE_CVILLAGEPEOPLEDEF_H
#define FABLE_ENGINE_CVILLAGEPEOPLEDEF_H
#ifdef FABLE_RETAIL_CREATE_VILLAGER_PASS27
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
class CVillagePeopleDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName(){return CCharString("CVillagePeopleDef",-1);}
    bool Enable;
    float DesiredMaleWeight;
    float DesiredFemaleWeight;
    float DesiredChildWeight;
    float ProbabilityOfHomsexualityPerDialogueEnabledMan;
    long MaleTeacher;
    long FemaleTeacher;
    long MaleShopKeeper;
    long FemaleShopKeeper;
    long MaleMarketTrader;
    long FemaleMarketTrader;
    long MaleBarman;
    long FemaleBarman;
    long MaleBarmaid;
    long FemaleBarmaid;
    long MaleWarehouseWorker;
    long FemaleWarehouseWorker;
    long MaleServant;
    long FemaleServant;
    long MaleGoodApprentice;
    long FemaleGoodApprentice;
    long MaleEvilApprentice;
    long FemaleEvilApprentice;
    long MaleUnemployed;
    long FemaleUnemployed;
    long MaleChild;
    long FemaleChild;
    long GamesMaster;
    long Guard;
    long MaleGhost;
    long FemaleGhost;
    long NumGuards;
    long NumGhosts;
    std::vector<long> GuardRespawnType;
    std::vector<long> GuardRespawnQuantity;
    long GuardRespawnMaxRenownBasedOffset;
};
FABLE_STATIC_ASSERT(offsetof(CVillagePeopleDef,ProbabilityOfHomsexualityPerDialogueEnabledMan)==0x34);
FABLE_STATIC_ASSERT(sizeof(CVillagePeopleDef)==0xc4);
#endif
#endif

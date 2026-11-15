#ifndef FABLE_ENGINE_CCREATUREACTION_SHOPKEEPERPUTAWAY_H
#define FABLE_ENGINE_CCREATUREACTION_SHOPKEEPERPUTAWAY_H
// Both original complete classes and constructor PUBLICs; native base0xa8, Goods+0xa8.
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"
class CCreatureAction_ShopkeeperPutAway : public CCreatureActionBase {
public:
    CCreatureAction_ShopkeeperPutAway(CThingCreatureBase&,CThing*);
    virtual ~CCreatureAction_ShopkeeperPutAway();
    virtual void PerformAction();
    virtual CCreatureActionBase* Clone() const;
private:
    CIntelligentPointer<CThing> Goods;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_ShopkeeperPutAway)==0xb0);
#endif

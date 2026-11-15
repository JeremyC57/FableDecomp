#ifndef FABLE_ENGINE_CCREATUREACTION_SHOPKEEPERGETOUT_H
#define FABLE_ENGINE_CCREATUREACTION_SHOPKEEPERGETOUT_H
// Both original complete records; native base0xa8 then TCShop/Goods/Pos/RHSet, size0xd4.
#include "engine/CCreatureActionBase.h"
#include "engine/CRightHandedSet.h"
class CTCShop;
class CCreatureAction_ShopkeeperGetOut : public CCreatureActionBase {
public:
    CCreatureAction_ShopkeeperGetOut(CThingCreatureBase&,CTCShop*,const C3DVector&,const CRightHandedSet&,long);
    virtual void PerformAction();
    virtual CCreatureActionBase* Clone() const;
private:
    CTCShop* TCShop;
    long Goods;
    C3DVector Pos;
    CRightHandedSet RHSet;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_ShopkeeperGetOut)==0xd4);
#endif

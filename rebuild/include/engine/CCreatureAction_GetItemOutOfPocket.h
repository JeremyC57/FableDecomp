#ifndef FABLE_ENGINE_CCREATUREACTION_GETITEMOUTOFPOCKET_H
#define FABLE_ENGINE_CCREATUREACTION_GETITEMOUTOFPOCKET_H
#include "engine/CCreatureActionBase.h"
class CCreatureAction_GetItemOutOfPocket : public CCreatureActionBase {
public:
    CCreatureAction_GetItemOutOfPocket(CThingCreatureBase&,long);
    virtual void PerformAction();
    virtual CCreatureActionBase* Clone() const;
private:
    long ItemDefIndex;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_GetItemOutOfPocket)==0xac);
#endif

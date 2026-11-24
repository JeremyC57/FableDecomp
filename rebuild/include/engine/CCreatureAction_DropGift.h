#ifndef FABLE_ENGINE_CCREATUREACTION_DROPGIFT_H
#define FABLE_ENGINE_CCREATUREACTION_DROPGIFT_H
#ifdef FABLE_RETAIL_CCREATUREACTION_DROPGIFT_PASS27
#include "engine/CCreatureActionBase.h"
// Original FableWin and Ego_r both give this public virtual override.
class CCreatureAction_DropGift : public CCreatureActionBase {
public:
    virtual void PerformAction();
};
#endif
#endif

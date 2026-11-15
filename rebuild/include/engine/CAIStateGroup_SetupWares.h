#ifndef FABLE_ENGINE_CAISTATEGROUP_SETUPWARES_H
#define FABLE_ENGINE_CAISTATEGROUP_SETUPWARES_H
// Original complete Ego_r/FableWin records: empty derived class, native size0x14.
#include "engine/CAIStateGroupBase.h"
class CAIStateGroup_SetupWares : public CAIStateGroupBase {
public:
    CAIStateGroup_SetupWares();
    virtual bool IsRunnable();
    virtual void Update();
    virtual bool InitCreaturePosition();
    static CAIStateGroupBase* FABLE_FASTCALL Alloc();
    static CCharString FABLE_FASTCALL GetName();
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_SetupWares)==0x14);
#endif

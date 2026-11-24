#ifndef FABLE_ENGINE_CAISTATEGROUP_WORKINWAREHOUSE_H
#define FABLE_ENGINE_CAISTATEGROUP_WORKINWAREHOUSE_H
#include "engine/CAIStateGroup_MoveGoodsBase.h"
#include "engine/CVillageTaskCarryCrate.h"

// Ego_r RTTI and layout identify the MoveGoods base and PTask at +0x1c.
// Ego_r and FableWin PDBs agree on the public virtuals and private MoveCrates.
class CAIStateGroup_WorkInWarehouse : public CAIStateGroup_MoveGoodsBase {
public:
    virtual bool InitCreaturePosition();
    virtual void Cleanup();
    virtual bool IsRunnable();
    virtual void Update();
private:
    bool MoveCrates();
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CAIStateGroup_WorkInWarehouse, PTask) == 0x1c);
    }
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_WorkInWarehouse) == 0x24);
#endif

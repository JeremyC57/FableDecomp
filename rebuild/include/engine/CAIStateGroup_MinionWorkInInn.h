#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONWORKININN_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONWORKININN_H
#include "engine/CAIStateGroupBase.h"
// Both PDBs: original class has no own data, size20; native vtable overrides base slots11/12.
class CThing;
class CAIStateGroup_MinionWorkInInn : public CAIStateGroupBase {
public:
    virtual void Update();
    virtual bool IsRunnable();
    static CAIStateGroupBase* FABLE_FASTCALL Alloc();
    static CCharString FABLE_FASTCALL GetName();
private:
    void CleanBench();
    CThing* GetFreeBench();
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionWorkInInn)==0x14);
#endif

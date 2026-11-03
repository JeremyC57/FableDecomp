#ifndef FABLE_ENGINE_CENGINEPRIMITIVEHANDLE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEHANDLE_H
// Retail declaration (Codex pass 11): Ego_r TPI0xb241, public CBaseClass
// +0 and private CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4,
// sizeof8; native global objects at013baf44/013baf4c and vtable0122f598.
#include "engine/CBaseClass.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointer.h"
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle() { }
    virtual ~CEnginePrimitiveHandle() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
    }
    void SetData(CEnginePersistentPrimitiveBase* data);
    void Clear() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Release();
    }
private:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle) == 8);
#endif

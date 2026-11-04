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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
    // Retail opt-in (chunk 737, CPSCLight::PreparePrimitives 0x00B19C60; the landed CPSCLight::Kill 0x00B19C20 model): FableWin.pdb
    // ?Clear@CEnginePrimitiveHandle@@QAEXXZ, inline in retail: the last handle notifies the primitive (slot 3), then `Data = 0`
    // through the counted pointer's Reset (needs FABLE_RETAIL_CIVCOUNTEDPOINTER: release inside the null test, then the second store).
    void Clear() {
        if (Data.GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    // Chunk 738 (CPSCRenderSprite::PreparePrimitives 0x00AFCC30, the inline CParticle::SetPrimitiveInfo): FableWin.pdb
    // ??4CEnginePrimitiveHandle@@QAEABV0@ABV0@@Z (returns const&), inline in retail: Clear() (the slot-3 notify when last, the
    // release and the two `mov [esi+4],0` stores at 0x00AFDA8C..0x00AFDAB6), then the counted-pointer assignment.
    __forceinline const CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other) {
        Clear();
        Data = other.Data;
        return *this;
    }
#else
    void Clear() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Release();
    }
#endif
private:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle) == 8);
#endif

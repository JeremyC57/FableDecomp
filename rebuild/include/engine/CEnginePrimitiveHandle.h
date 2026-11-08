#ifndef FABLE_ENGINE_CENGINEPRIMITIVEHANDLE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEHANDLE_H
#include "fable_compiler.h"
// Retail declaration (Codex pass 11): Ego_r TPI0xb241, public CBaseClass
// +0 and private CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4,
// sizeof8; native global objects at013baf44/013baf4c and vtable0122f598.
#include "engine/CBaseClass.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointer.h"
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle() { }
#if defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE) && defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR)
    // Nested opt-in (chunk 763, row 0x00441EE0): retail's inline destructor is Clear() then the member's own release
    // (0x0041C500 ??_G: notify at count 1, Reset(0), then ~CIVCountedPointer releases again).
    FABLE_FORCEINLINE virtual ~CEnginePrimitiveHandle() { Clear(); }
#else
    virtual ~CEnginePrimitiveHandle() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
    }
#endif
    void SetData(CEnginePersistentPrimitiveBase* data);
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
    // Chunk 740 (0x00B03CA0): FableWin.pdb ??4CEnginePrimitiveHandle@@QAEABV0@ABV0@@Z (inline in retail): the counted Data assignment.
    const CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other) { Data = other.Data; return *this; }
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
    // Retail opt-in (chunk 737, CPSCLight::PreparePrimitives 0x00B19C60; the landed CPSCLight::Kill 0x00B19C20 model): FableWin.pdb
    // ?Clear@CEnginePrimitiveHandle@@QAEXXZ, inline in retail: the last handle notifies the primitive (slot 3), then `Data = 0`
    // through the counted pointer's Reset (needs FABLE_RETAIL_CIVCOUNTEDPOINTER: release inside the null test, then the second store).
    void Clear() {
        if (Data.GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Reset(0);
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

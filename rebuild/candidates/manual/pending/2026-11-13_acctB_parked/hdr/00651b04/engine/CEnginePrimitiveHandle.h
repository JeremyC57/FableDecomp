// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
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
#if defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DTOR)
    // Original FableWin01909120: Clear then implicit member and base destructors.
    virtual ~CEnginePrimitiveHandle() { Clear(); }
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE) && defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR)
    // Nested opt-in (chunk 763, row 0x00441EE0): retail's inline destructor is Clear() then the member's own release
    // (0x0041C500 ??_G: notify at count 1, Reset(0), then ~CIVCountedPointer releases again).
    FABLE_FORCEINLINE virtual ~CEnginePrimitiveHandle() { Clear(); }
#else
    virtual ~CEnginePrimitiveHandle() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
    }
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
    void SetData(CEnginePersistentPrimitiveBase* data){Clear();Data.Reset(data);}
    bool IsNull()const{return Data.IsNull();}
    long GetType()const{return Data->GetType();}
    CEnginePersistentPrimitiveBase& GetData()const{return *Data.operator->();}
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA)
    bool IsNull()const{return Data.IsNull();}
    CEnginePersistentPrimitiveBase& GetData()const{return *Data;}
    const CEnginePersistentPrimitiveBase& PeekData()const{return *Data;}
    void SetData(CEnginePersistentPrimitiveBase* data){Clear();Data.Reset(data);}
#else
    void SetData(CEnginePersistentPrimitiveBase* data);
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
    // Chunk 740 (0x00B03CA0): FableWin.pdb ??4CEnginePrimitiveHandle@@QAEABV0@ABV0@@Z (inline in retail): the counted Data assignment.
    const CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other) {
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
        Clear();
#endif
        Data=other.Data;return *this;
    }
#endif
#if defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE) && defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE)
    // Retail opt-in (bwf1, row 0x00651B04 NPlayerGui::CDrawAbilityButtons::Draw): the same retail body, __forceinline for the size TU
    // that expands it in place (0x00651B64..0x00651B99; out of line elsewhere at 0x0041BE70), as the landed 0x00644FFC models it.
    FABLE_FORCEINLINE void Clear() {
        if (Data.GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Reset(0);
    }
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE)
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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_UPDATE
    unsigned long GetReferenceCount()const{return Data.GetRefCount();}
#endif
private:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle) == 8);
#endif

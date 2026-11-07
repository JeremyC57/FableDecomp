#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTER_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTER_H
// Retail declaration (Codex pass 11): Ego_r TPI0x33e8, class template V,
// one private CIVCountedPointeeBase* Object at +0. Native release is inlined.
#include "engine/CIVCountedPointeeBase.h"
template<class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
    // Retail opt-in (chunk 734, row 0x004A1CB0): the counted copy constructor (FableWin.pdb ??0?$CIVCountedPointer@..@@QAE@ABV0@@Z,
    // inline in retail: `mov ecx,[eax+4] ; test ecx,ecx ; je ; inc dword ptr [ecx+4]` at 0x004A2084). Without the opt-in the
    // implicit memberwise copy of the landed includers is unchanged.
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
#endif
    ~CIVCountedPointer() { Release(); }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
    // Chunk 737 (CPSCLight::PreparePrimitives 0x00B19C60, as the landed CPSCLight::Kill 0x00B19C20 / ~CPSCLight 0x00B1AEB0): FableWin.pdb
    // ?Reset@?$CIVCountedPointer@..@@QAEXPAV1@@Z and ?Release@?$CIVCountedPointer@..@@QAEXXZ, inline in retail. Release clears the
    // pointer inside its null test and Reset stores the new one after it: CEnginePrimitiveHandle::Clear's `Data.Reset(0)` gives
    // retail's `mov [esi+4],0` on the release path plus the unconditional second store (0x00B1A623 / 0x00B1A626).
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
    // Nested opt-in (chunk 850, row 0x00AC5770 C3DMeshFileEngineMaterialChunk::ReadFromFile): Ego_r.pdb
    // ?Reset@?$CIVCountedPointer@VCMapFilenames@C3DMeshFileEngineMaterialChunk@@@@QAEXPAV..@@Z adopts the pointer: the new
    // CMapFilenames (IVRefCount 1 from its constructor) is stored after the release with no AddRef (0x00AC5A86 / 0x00AC5F16:
    // `mov [edi+0x34],ebp` right after the release block, no `inc [ebp+4]`). The AddRef of the branch below is unproven
    // (its only landed users pass 0).
    void Reset(T* object) { Release(); Object = object; }
#else
    void Reset(T* object) { Release(); Object = object; if (Object) Object->AddRef(); }
#endif
#else
    void Reset(T* object);
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
    // Chunk 740 (CPSCRenderMesh::PreparePrimitives 0x00B03CA0, CParticle::SetPrimitiveInfo expanded at 0x00B04F2C / 0x00B0555A):
    // FableWin.pdb ??4?$CIVCountedPointer@VCEnginePersistentPrimitiveBase@@@@QAEAAV0@ABV0@@Z, inline in retail: `cmp ecx,[src]; je`,
    // the release (with its `mov [dst],0`), then the new pointer stored and `inc [eax+4]` when non-null. Needs FABLE_RETAIL_CIVCOUNTEDPOINTER.
    CIVCountedPointer& operator=(const CIVCountedPointer& other) {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
#endif
    T* operator->() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
    void Release() {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0) object->Delete();
            Object = 0;
        }
    }
#else
    void Release() {
        CIVCountedPointeeBase* object = Object;
        if (object && object->Release() == 0) object->Delete();
        Object = 0;
    }
#endif
private:
    CIVCountedPointeeBase* Object;
};
#endif

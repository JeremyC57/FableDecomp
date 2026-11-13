#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTER_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTER_H
// Retail declaration (Codex pass 11): Ego_r TPI0x33e8, class template V,
// one private CIVCountedPointeeBase* Object at +0. Native release is inlined.
#include "engine/CIVCountedPointeeBase.h"
#include "fable_compiler.h"
template<class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
    // Nested opt-in (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): Ego_r.pdb / FableWin.pdb
    // ??0?$CIVCountedPointer@VCAnimComponentBase@@@@QAE@PAVCAnimComponentBase@@@Z, inline in retail. It adopts the pointee: the new
    // component's IVRefCount is 1 from CIVCountedPointeeBase's constructor (FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR; 0x005DD691
    // `mov [eax+4],edi(1)`) and no AddRef follows; the local's one release at the end of each registration block
    // (0x005DD820 `dec [esi+4]`) balances it. As Reset's adopting form (FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT).
    CIVCountedPointer(T* object) : Object(object) { }
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
    // Retail opt-in (chunk 734, row 0x004A1CB0): the counted copy constructor (FableWin.pdb ??0?$CIVCountedPointer@..@@QAE@ABV0@@Z,
    // inline in retail: `mov ecx,[eax+4] ; test ecx,ecx ; je ; inc dword ptr [ecx+4]` at 0x004A2084). Without the opt-in the
    // implicit memberwise copy of the landed includers is unchanged.
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
    // Nested opt-in (chunk 867, row 0x005A8C66 CTCInventoryTrade::ConstructActualSpecialInventoryPC, a `#pragma optimize("s")` TU):
    // the destructor spelled in one level, the pointee's release and conditional Delete written here instead of through
    // Release(), so that VC7.1's size-mode inliner expands it in place as retail does for the CIVCountedPointer<const
    // NGameText::CTextEntry> temporaries (0x005A8F05: `cmp eax,ebx ; je ; mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],ebx ;
    // jne ; mov eax,[ecx] ; call [eax+4]`, no null store: a dying temporary). Through Release() the two-level body is emitted out of
    // line (?Release@?$CIVCountedPointer@..@@QAEXXZ as a COMDAT, 0x005A8C66 graded 22 bytes short). The pointer local is declared
    // inside the null test: declared before it (`object && object->Release() == 0`) the copy into ecx is scheduled above the `je`.
    // Same semantics as the default destructor.
    ~CIVCountedPointer() {
        if (Object) {
            CIVCountedPointeeBase* object = Object;
            if (object->Release() == 0) object->Delete();
        }
    }
#else
    ~CIVCountedPointer() { Release(); }
#endif
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
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
    // Nested opt-in (chunk Q31, row 0x00A69B70 CTextureManager::AllocateTexture): the same adopting body forced inline. Retail
    // expands it there (0x00A69BDB..0x00A69BF3: release, then the store); VC7.1's inline budget leaves the plain body a call in that
    // caller (a toy with the same body inlines it, the AllocateTexture shape does not). The caller AddRefs through operator->.
    FABLE_FORCEINLINE void Reset(T* object) { Release(); Object = object; }
#else
    void Reset(T* object) { Release(); Object = object; }
#endif
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
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
    FABLE_FORCEINLINE
#endif
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
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
    // Opt-in (chunk 898, row 0x00A3BF10 C3DAnimationBlendState::CreateInterpolate): Ego_r.pdb's converting constructor
    // (??$?0VC3DAnimationTransitionState@@@?$CIVCountedPointer@VC3DAnimationStateBase@@@@QAE@ABV?$CIVCountedPointer@V...@@@@@Z shape:
    // take the other pointer's object and AddRef it; `return trans;` from a CIVCountedPointer<derived>), InternalGetObject and
    // operator* (both inline in retail).
    template<class U> CIVCountedPointer(const CIVCountedPointer<U>& other) : Object(other.InternalGetObject()) { if (Object) Object->AddRef(); }
    CIVCountedPointeeBase* InternalGetObject() const { return Object; }
    T& operator*() const { return *static_cast<T*>(Object); }
#endif
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
    // Nested opt-in (chunk 851, row 0x00A9BBE0 C3DMeshFileLoader ctor, the C3DMeshFileEngineMaterialChunk temporary's MapFilenames
    // at 0x00A9C4BF): the pointee's own Release deletes it (FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES); retail loads the
    // count before it copies the pointer into ecx (`mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi`), which the local-copy
    // form with the Delete in the caller schedules the other way.
    void Release() {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
#elif defined(FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER)
    // Nested opt-in (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): the pointer copied inside the null test, so the
    // copy into ecx follows the `je` (0x005DD707: `cmp eax,ebx ; je ; mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx`), then the
    // zero store on the release path (0x005DD71D). Same semantics as the branch below.
    void Release() {
        if (Object) {
            CIVCountedPointeeBase* object = Object;
            if (object->Release() == 0) object->Delete();
            Object = 0;
        }
    }
#else
    void Release() {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0) object->Delete();
            Object = 0;
        }
    }
#endif
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

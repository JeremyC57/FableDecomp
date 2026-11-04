// CEnginePrimitiveTypeInfo::`vector deleting destructor'(unsigned int) `adjustor{12}' -- CBaseClassNonCopyable-subobject vtable entry: this -= 0xc, jump to the deleting destructor 0x00B8FB60
// Leaf: ??_ECEnginePrimitiveTypeInfo@@WM@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8FB50, 3+5 bytes: `sub ecx,0xc ; jmp 0x00B8FB60`. Compiler-generated adjustor thunk.
// Identity (two sources): (1) slot 0 of the CBaseClassNonCopyable (+0xC) vtable 0x012A3D4C (RTTI complete-object locator offset 0xc, class
//   .?AVCEnginePrimitiveTypeInfo@@; the catalog's "vfunc_0"); (2) its jump target 0x00B8FB60 is slot 0 of the primary vtable 0x012A3D58, the class's
//   ??_G. VC7.1 names the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the adjustor in the
//   thunk name (??_ECEnginePrimitiveTypeInfo@@WM@AEPAXI@Z); objdump -t of this object shows the leaf.
// TU: the thunk is emitted with the vtable in any TU that defines the class's constructor or destructor (chunk 765 shape). This
//   source reuses the landed definer CEnginePrimitiveTypeInfo::`scalar deleting destructor' 0x00B8FB60 (landed, verbatim) as that TU;
//   only the thunk is graded here. Landed definer: rebuild/src/compiled/00/b8/CEnginePrimitiveTypeInfo_scalardeletingdestructor_00b8fb60.cpp
// Callees: ??_GCEnginePrimitiveTypeInfo@@UAEPAXI@Z 0x00B8FB60 (bound as its ??_E alias).
// ---- definer TU (verbatim from the landed row, minus its Leaf/Flags lines) ----
// CEnginePrimitiveTypeInfo::`scalar deleting destructor'(unsigned int) -- the implicit destructor (~CBaseClassNonCopyable on the base at +0x0C, then ~CEngineComponent), then free the object on flag bit 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8FB60, 48 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of the primary vtable 0x012A3D14 (RTTI .?AVCEnginePrimitiveTypeInfo@@); Ego_r.pdb
//   ??_GCEnginePrimitiveTypeInfo@@UAEPAXI@Z (donor right after RegisterPrimitiveManager). The catalog label
//   "CEngineSubPrimitiveText `scalar_deleting_destructor'" is propagated (wrong class). Slot 0 of the secondary table
//   0x012A3D4C is 0x00B8FB50, its this-adjusting thunk.
// Callees: CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (protected virtual, Ego_r.pdb MAE), CEngineComponent::~CEngineComponent
//   0x00B59730, operator delete 0x00BFE9BC.
// Shape: no user destructor (no vptr store); the second base's destructor runs on `this ? this+0x0C : 0` (retail's null-
//   tested adjust); bases are destroyed in reverse order. The vtable (and this ??_G) is emitted by the class's real
//   constructor, the byte-exact 0x00B8FAA0 body.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CEngineComponent {
public:
    CEngineComponent();                              // 0x00B59710
    virtual ~CEngineComponent();                     // 0x00B59730
    void** GlobalPointerAddress;                     // +0x04
    bool RenderEnabled;                              // +0x08
    bool KeepAlive;                                  // +0x09
};
void __cdecl operator delete(void* p);                       // 0x00BFE9BC
class CEnginePrimitiveManager;

class CEnginePrimitiveTypeInfo : public CEngineComponent, public CBaseClassNonCopyable {
public:
    CEnginePrimitiveTypeInfo();
    CEnginePrimitiveManager* PrimitiveManagers[47];  // +0x10
};

CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo()
{
    for (long i = 0; i < 47; ++i)
        PrimitiveManagers[i] = 0;
}
// C3DAnimationBlendState::~C3DAnimationBlendState() -- implicit destructor: release the shared blend data (CIVCountedPointer Data), then ~CBaseClass
// Leaf: ??1C3DAnimationBlendState@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C2DF0, 35 bytes, thiscall: Data.Release() inline (null test, `dec [ecx+4]`,
//   virtual Delete (slot 1) at zero, `Data.Object = 0`), then `jmp ~CBaseClass` 0x0099A2E0.
// Identity: the three callers destroy a stack C3DAnimationBlendState built by the constructor 0x004C2DD0, which stores the
//   C3DAnimationBlendState vtable 0x01239838 (rtti-vtables.tsv) and zeroes +4: NUISystem::CMesh 0x005506C8 (after
//   SetBaseAnimation 0x00A3C260 on it), CTCInventoryClothing 0x005B9BE1, CDrawReceivedItems::Draw 0x0064ECC0. The vtable's
//   slot 0 (??_G 0x004C3CD0) inlines this same body before its conditional free. Ego_r.pdb: ??1C3DAnimationBlendState@@UAE@XZ
//   (donor 0x004C5E70, next to ??0 0x004C5E50); layout 8 bytes = vptr + CIVCountedPointer<C3DAnimationBlendState::CSharedData>
//   Data +4; FableWin.pdb: ??1 / Release of CIVCountedPointer<CSharedData> (inline in retail). The base is CBaseClass (the
//   ctor calls CBaseClass() 0x0099A2D0, the dtor tail-jumps to ~CBaseClass 0x0099A2E0).
// Shape: the destructor is compiler-generated (no vtable re-store, see GOTCHAS "VC7.1 drops the vtable reset in a
//   compiler-GENERATED destructor"); an emission helper forces the ??1 COMDAT. The twin 0x005E0AB0 is a different function.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"              // vptr, slot 1 Delete() const, inline Release() (returns the new count)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { Release(); }                 // FableWin.pdb ??1?$CIVCountedPointer@VCSharedData@C3DAnimationBlendState@@@@QAE@XZ
    void Release()                                      // FableWin.pdb ?Release@?$CIVCountedPointer@...@@QAEXXZ
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                      // Ego_r.pdb: CIVCountedPointeeBase* Object
};

class C3DAnimationBlendState : public CBaseClass {     // vtable 0x01239838, 8 bytes
public:
    class CSharedData;
    CIVCountedPointer<CSharedData> Data;                // +0x04
};

// Emission helper (not a retail symbol): forces the implicit virtual destructor out of line as its own COMDAT.
void EmitBlendStateDestructor(C3DAnimationBlendState* state)
{
    state->C3DAnimationBlendState::~C3DAnimationBlendState();
}
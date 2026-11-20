// Naming (NF160, 2026-11-20, q6213): the local CRetailHandleVector stand-in replaced by the shared STLport 4.6 std::vector<CEnginePrimitiveHandle>
//   (stlport/fable_containers.h, FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL; the landed 0x0064E1E4 / 0x0064ED06 recipe), so its Resize call binds
//   0x00443F90 as ?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z (STLport one-argument
//   resize; the self row and 29 landed callers bind it).
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
#pragma optimize("s", on)
// NPlayerGui::CDrawScreenFade::CDrawScreenFade(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, the full-fade-drawn flag cleared, one primitive handle reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E8DC, length 42 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (pass-through pushes `push [esp+0Ch]` twice: a size TU), vector<CEnginePrimitiveHandle>::Resize(long)
// @ 0x00443F90 (on PrimitiveHandles +0x1c, count 1). Vtable 0x0125A640 (NPlayerGui::CDrawScreenFade: the chunk label, consistent with the PDB layout).
// PDB CDrawScreenFade (48 bytes): DrawnFullFadeOut at +0x2c, retail +0x28 (the CDrawBase vector is 12 bytes in the shipped build).
// Shape: the vtable store and the byte clear sit between the count push / `lea ecx` and the Resize call.
class CPlayerGui;

namespace NPlayerGui {


class CDrawBase { public:
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

class CDrawScreenFade : public CDrawBase { public:               // vtable 0x0125A640
    virtual void v00();
    bool DrawnFullFadeOut;                         // +0x28
    CDrawScreenFade(CPlayerGui* gui, void* drawList);
};

CDrawScreenFade::CDrawScreenFade(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), DrawnFullFadeOut(false)
{
    PrimitiveHandles.resize(1);
}

}
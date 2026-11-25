// Name fix (agentNF27, 2026-11-12, FAM10 NAME_FIXES): MouseCursor is the shared CCountedPointer<CSurface> and the call is its out-of-line
//   Reset 0x00AB5E80, FableWin.pdb ?Reset@?$CCountedPointer@VCSurface@@@@QAEXPAVCSurface@@@Z (stores DeleteData<CSurface> 0x00AB5DF0),
//   was the stand-in ?Reset@CCountedSurfacePointer@@; the cursor primitive gets the new surface's PD3DSurface (CSurface +4).
//   Header opt-in FABLE_RETAIL_CMOUSEDX_MOUSECURSOR (candidates/mouse_dx_accessors_compiled.h).
#define FABLE_RETAIL_CMOUSEDX_MOUSECURSOR
#include <new>
#include "candidates/mouse_dx_accessors_compiled.h"

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009A4EC0 ?GFGetSystemManager@@YIPAVCSystemManager@@XZ (was ?ResolveMousePrimitiveHierarchy@@YAPAUCMousePrimitiveHierarchy@@XZ)
class CSystemManager;
CSystemManager* __fastcall GFGetSystemManager();
void CMouseDX::SetMouseCursor(const CSurface& surface)
{
    CSurface* copiedSurface = new CSurface(surface);
    MouseCursor.Reset(copiedSurface);

    if (DisableCursorWarp != 0)
    {
        CMousePrimitiveHierarchy* hierarchy = reinterpret_cast<CMousePrimitiveHierarchy*>(GFGetSystemManager());
        CMouseEnginePrimitive* primitive = hierarchy->Owner->Primitive;
        primitive->Dispatch->SetCursorSurface(
            primitive, 0, 0, MouseCursor->PD3DSurface);

        hierarchy = reinterpret_cast<CMousePrimitiveHierarchy*>(GFGetSystemManager());
        primitive = hierarchy->Owner->Primitive;
        primitive->Dispatch->SetCursorEnabled(primitive, 1);
    }
}

// NGraphicAppearance::CTCGraphicAppearance::GetFocalPosIfLoaded(C3DVector& position, float time) const -- the focal helper point (the
// global focal-point name, index 0, flags 2) if the mesh is loaded
// Leaf: ?GetFocalPosIfLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C11D0, length 32 bytes. Callees: CTCGraphicAppearance::GetHelperPoint @ 0x004C0560
// (name, index, flags, out position, time; `this` passed through in ecx). The name is the global MAIN_FOCAL_POSITION_NAME (0x013BAC48).
// Shape: the two arguments re-pushed in reverse (the float as a raw dword through eax), the constants, the global's address;
// the callee's byte result normalised to bool (`test al,al ; setne al`).
// Naming (NF108, 2026-11-19, q5150): Ego_r.pdb 0x004C2870 + FableWin.pdb spell it const with the position by reference; the landed caller
//   0x00753380 binds that spelling. Re-authored on the shared engine/NGraphicAppearance_CTCGraphicAppearance.h (FOCAL: GetHelperPoint;
//   overlay opt-in GETFOCALPOSIFLOADED), replacing the local CTCGraphicAppearance model and its pointer / non-const spelling.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_GETFOCALPOSIFLOADED
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

extern const CCharString MAIN_FOCAL_POSITION_NAME;        // 0x013BAC48 ?MAIN_FOCAL_POSITION_NAME@@3VCCharString@@B

bool NGraphicAppearance::CTCGraphicAppearance::GetFocalPosIfLoaded(C3DVector& position, float time) const
{
    return GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 2, position, time) ? true : false;
}

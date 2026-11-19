// NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPos() const -- the owner's static focal point: the "focus_eye_01" helper position when the mesh has it (caching its offset from the render position), else the render position itself (caching a zero offset)
// Leaf: ?GetStaticFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C10E0, length 227 bytes, `ret 4` (hidden C3DVector result).
// Name: Ego_r.pdb tc_graphic_appearance.obj #44 ?GetStaticFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@XZ between
//       GetFocalPos (#43 = retail 0x004C1060) and GetStaticFocalPosIfLoaded (#46 = retail 0x004C11F0); FableWin.pdb debug\tc_graphic_appearance.obj
//       #176, same neighbours. The catalog "CTCGraphicAppearance_UpdateHelperPointOffset" is behavioural.
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&, float) const @ 0x004C0560 (flags 4 =
//          ignore animation), the file-static __fastcall GetRenderPositionForThing(const CThing&, float) @ 0x004BBF00; the global
//          CCharString MAIN_FOCAL_POSITION_NAME "focus_eye_01" @ 0x013BAC48. Ego_d.pdb locals: this, pos (C3DVector).
// Shape: two early-return arms, each setting the cached flag first (retail stores bit 5 of +0x59 once, before the branch,
//        0x004C10FE..0x004C110B); the zero offset is a C3DVector(0,0,0) temporary copied member-wise (0x004C1155). The if/else form
//        with one shared `return pos` gives a different register choice in the zero arm (eax v ecx for &CachedStaticFocalPosOffset).
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_PCMOUSELOOK_PASS24
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
extern const CCharString MAIN_FOCAL_POSITION_NAME;
#include "engine/NGraphicAppearance_CTCGraphicAppearance_HelperPoint.h"
namespace NGraphicAppearance {
C3DVector CTCGraphicAppearance::GetStaticFocalPos() const
{
    C3DVector pos;
    if (GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 4, pos, 1.0f)) {
        const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPos = true;
        const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPosOffset = pos - GetRenderPositionForThing(GetParentThing(), 1.0f);
        return pos;
    }
    const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPos = true;
    const_cast<CTCGraphicAppearance*>(this)->CachedStaticFocalPosOffset = C3DVector(0.0f, 0.0f, 0.0f);
    pos = GetRenderPositionForThing(GetParentThing(), 1.0f);
    return pos;
}
}
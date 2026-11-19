// NUISystem::CList::CanMove(const C2DVector& where) -- the position the list may move to: `where`, with Y replaced by the target Y when the (relative, converted) position would leave the definition's view area scaled to the UI (bottom edge when scrolling up, top edge otherwise)
// Leaf: ?CanMove@CList@NUISystem@@UAE?AVC2DVector@@ABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00537776, 480 bytes, thiscall, `ret 8` (hidden C2DVector result); size TU with a frame pointer.
// Source: FableWin.pdb debug body 0x0219F54D (ui_list.obj) with Ego_d.pdb's locals (where; result, view_area, scroll_up,
//   view_area_offset, testing_view_area, pdefinition; block: size_y): LoadAndReturnDef (slot 108), a C2DBoxI copy of the def's
//   ViewArea with BRX / BRY rescaled by GetUIScale, IsUsingRelativePosition (slot 117) / PInitialPositionDummy's
//   GetRelativeRenderPosition (slot 118) + CUIScreenUnitConverter::ConvertVector(where - Position), view_area_offset.Y from
//   ConvertY(TLY) - TLY scaled, GetDynamicSize (slot 111), TargetPosition.Y.
// Headers: the C2DVector sum is the existing shared opt-in FABLE_RETAIL_C2DVECTOR_ADD_COPY_RHS_X_PASS23 (landed 0x0064A639; X adds the
//   right operand first, Y the left: retail 0x0053782E..0x0053783E and 0x005378B4..0x005378C1); the plain sum swaps the X addends.
// Inline-budget: the view-area box copy uses the new overlay opt-in FABLE_RETAIL_C2DBOXI_COPY_FORCEINLINE. The user copy constructor is
//   sourced (FableWin.pdb ??0C2DBoxI@@QAE@ABV0@@Z 0x018B1BF0, memberwise), but forcing it inline is fitted: in this size TU c2 calls
//   the plain inline out of line, while retail copies only the live TLY / BRY words in place (0x005377B8 / 0x005377BE); the implicit
//   copy is a movsd block copy. Coordinator to decide.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ADD_COPY_RHS_X_PASS23
#define FABLE_RETAIL_C2DBOXI_ACCESSORS
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_SETBR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY_FORCEINLINE
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_CONVERTXY_INLINE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
#include "engine/CDefPointer.h"
#include "engine/C2DVector.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
using namespace NUISystem;

C2DVector CList::CanMove(const C2DVector& where)
{
    C2DVector result(where);
    C2DVector testing_view_area(where);
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    bool scroll_up = where.Y < Position.Y;
    C2DVector view_area_offset(0.0f, 0.0f);
    C2DBoxI view_area(pdefinition->ViewArea);
    view_area.SetBRX((long)(view_area.GetTLX() + view_area.GetWidth() * CManager::GetInstance()->GetUIScale().X));
    view_area.SetBRY((long)(view_area.GetTLY() + view_area.GetHeight() * CManager::GetInstance()->GetUIScale().Y));
    if (IsUsingRelativePosition())
        testing_view_area = RelativeRenderPosition + CUIScreenUnitConverter::ConvertVector(where - Position);
    view_area_offset.Y = CUIScreenUnitConverter::ConvertY((float)view_area.GetTLY()) - view_area.GetTLY();
    view_area_offset.Y *= CManager::GetInstance()->GetUIScale().Y;
    if (PInitialPositionDummy)
        testing_view_area = PInitialPositionDummy->GetRelativeRenderPosition() + CUIScreenUnitConverter::ConvertVector(where - Position);
    if (scroll_up) {
        float size_y = GetDynamicSize().Y * CManager::GetInstance()->GetUIScale().Y;
        if (testing_view_area.Y + size_y < view_area.GetBRY() + view_area_offset.Y)
            result.Y = TargetPosition.Y;
    } else {
        if (view_area.GetTLY() + view_area_offset.Y < testing_view_area.Y)
            result.Y = TargetPosition.Y;
    }
    return result;
}
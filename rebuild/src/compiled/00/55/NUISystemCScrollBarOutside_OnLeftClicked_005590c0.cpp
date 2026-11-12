// NUISystem::CScrollBarOutside::OnLeftClicked() -- a click on the scroll bar's trough: runs the "under" or "above" actions by the side of the thumb the cursor is on
// Leaf: ?OnLeftClicked@CScrollBarOutside@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005590C0, 365 bytes of code; slot 146 of the primary vtable 0x0124B54C. Flags: none (base /O2).
// Callees: CManager::GetInstance 0x0041E5F2 (three times), CManager::GetUIScale 0x0041CF47 (twice), __ftol2 0x00BFEA70 (the (long)
//          casts); virtual: LoadAndReturnDef (slot 108, +0x1B0), ChangeState (slot 48, +0xC0; 0x0052CF40), the cursor's GetPosition
//          (slot 16, +0x40), the bar's GetTargetRelativeRenderPosition / GetTargetRelativeRenderZoom (slots 122 / 123, +0x1E8 / +0x1EC)
//          and GetSize (slot 23, +0x5C, twice), ExecuteActions (slot 131, +0x20C; 0x005309B0), CObserver::ObserveEvent (slot 3 of the
//          +4 subobject; 0x0052DA20), and the def's Delete (slot 1) when the LoadAndReturnDef temporary drops the last reference.
// Shape: the first two statements are CClickable::OnLeftClicked's (0x0055AF60) without its ExecuteActions. The rest builds the bar's
//          screen box as a C2DBoxI (the 16 frame bytes at esp+0x34..0x44: only TLY at +0x38 is ever stored; the TLX / BRX stores are
//          dead, which is why the first GetUIScale / GetSize pair has no arithmetic left, and BRY is forwarded). Each bottom-right
//          coordinate is `(GetSize() * GetUIScale().<axis>).<axis> * zoom + pos`: C2DVector::operator*(float) takes the scale by
//          value, so its Y is copied to a frame word (integer moves) before the second GetSize call. The two tests read the corners
//          through C2DBoxI::GetBRPos() / GetTLPos() (the C2DCoordI temporary holds the bottom at esp+8 for the fild), and the
//          else-if is tail-duplicated by the compiler into the two ObserveEvent / epilogue copies.
// Headers: overlay opt-ins FABLE_RETAIL_CSCROLLBAROUTSIDE_ONLEFTCLICKED (the override's declaration) and
//          FABLE_RETAIL_CMANAGER_GETMOUSECURSOR (FableWin.pdb ?GetMouseCursor@CManager@NUISystem@@QAEPAVCComponent@2@XZ, inline).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_GETMOUSECURSOR
#define FABLE_RETAIL_CSCROLLBAROUTSIDE_ONLEFTCLICKED
#include "engine/NUISystem_CScrollBarOutside.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CDefPointer.h"
#include "engine/C2DBoxI.h"
namespace NUISystem {
void CScrollBarOutside::OnLeftClicked()
{
    StateBeforeLeftClicked = TargetState;
    ChangeState(LoadAndReturnDef()->LeftClickedState);
    CComponent* pmouse_cursor = CManager::GetInstance()->GetMouseCursor();
    if (PScrollBar && pmouse_cursor) {
        C2DVector mouse_position = pmouse_cursor->GetPosition();
        C2DVector position = PScrollBar->GetTargetRelativeRenderPosition();
        C2DVector zoom = PScrollBar->GetTargetRelativeRenderZoom();
        C2DBoxI box;
        box.TLX = (long)position.X;
        box.TLY = (long)position.Y;
        box.BRX = (long)((PScrollBar->GetSize() * CManager::GetInstance()->GetUIScale().X).X * zoom.X + position.X);
        box.BRY = (long)((PScrollBar->GetSize() * CManager::GetInstance()->GetUIScale().Y).Y * zoom.Y + position.Y);
        if (box.GetBRPos().Y < mouse_position.Y)
            ExecuteActions(PActionsOnLeftClickedUnder);
        else if (box.GetTLPos().Y > mouse_position.Y)
            ExecuteActions(PActionsOnLeftClickedAbove);
    }
    ObserveEvent(UI_EVENT_LEFT_MOUSE_RELEASE);
}
}
// CTCPhotojournalCapture::TakeTheBloodyScreenshot() -- takes the quick screenshot, then reopens the player GUI
// Leaf: ?TakeTheBloodyScreenshot@CTCPhotojournalCapture@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D4C0, 16 bytes: call 0x0055D180 (TakeQuickScreenshot) ; mov ecx,[0x013B8790] (the
//   CPlayerGui singleton) ; jmp 0x004374B0 (CPlayerGui::Open).
// Name: Ego_r.pdb 0x005D67F0 + FableWin.pdb 0x023E2F90 ?TakeTheBloodyScreenshot@CTCPhotojournalCapture@@QAEXXZ, next to TakeQuickScreenshot
//   (Ego_r 0x005D64A0, retail 0x0055D180) and GlobalUpdate (retail 0x0055D4D0); its one retail reference 0x0062B599 is a call from a
//   row not landed yet. Callee CPlayerGui::Open Ego_r 0x0044A140 (engine/CPlayerGui.h _OPEN). CTCPhotojournalCapture has no shared
//   header; only the method is modelled. NF33 2026-11-16: was the stand-in free Dest_val (a propagated std::_Dest_val label).
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_OPEN
#include "engine/CPlayerGui.h"
class CTCPhotojournalCapture {
public:
    void TakeQuickScreenshot();                 // 0x0055D180
    void TakeTheBloodyScreenshot();
};
void CTCPhotojournalCapture::TakeTheBloodyScreenshot()
{
    TakeQuickScreenshot();
    CPlayerGui::Get().Open();
}

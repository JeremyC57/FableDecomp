// InGameMenu_ToggleLiveGui_0047d478() -- descriptive: forwards to CPlayerGui::ToggleLiveGui on the global player GUI
// Leaf: ?InGameMenu_ToggleLiveGui_0047d478@@YAXXZ
// Retail: Fable.exe @ 0x0047D478, 11 bytes: `mov ecx,[0x013B8790] ; jmp 0x00438310` (no frame, tail jump).
// Identity (descriptive): an unreferenced non-COMDAT function of the packed /Os CTCInGameMenu TU (between
// PCSetQuitToFrontEnd 0x0047D296 and IsAnySliderReceivingInput 0x0047D483); no caller, vtable slot or data pointer names it.
// Any incoming ecx is ignored, so it is modelled as a free function. The PDB name set has unassigned CTCInGameMenu
// members with this signature (OpenInGameMenu, ActivateBackOption, ...), but nothing ties one of them to this address.
// The same `mov ecx,[0x013B8790] ; call 0x00438310` tail ends SavedConfirmed 0x0047DB73.
// Callee: CPlayerGui::ToggleLiveGui() @ 0x00438310 on g_pPlayerGui @ 0x013B8790.
#pragma optimize("s", on)
#define CPlayerGui DonorCPlayerGui
#include "engine/CPlayerGui.h"
#undef CPlayerGui
class CPlayerGui : public DonorCPlayerGui {                // engine/CPlayerGui.h
public:
    void ToggleLiveGui();                                  // 0x00438310
};
extern CPlayerGui* g_pPlayerGui;                           // 0x013B8790

void InGameMenu_ToggleLiveGui_0047d478()
{
    g_pPlayerGui->ToggleLiveGui();
}
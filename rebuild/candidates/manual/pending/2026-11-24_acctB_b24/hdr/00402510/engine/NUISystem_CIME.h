#ifndef FABLE_ENGINE_NUISYSTEM_CIME_H
#define FABLE_ENGINE_NUISYSTEM_CIME_H
// New header (B24C4, row 0x00402510 GFMain): NUISystem::CIME, Ego_r.pdb layout (84 bytes: EnableIME +0, WindowHandle +4,
// IsInitialised +8, Appearance +0xC (IMEUI_APPEARANCE, 64 bytes), PEditBox +0x4C, CancelOnOKClick +0x50) and the publics GFMain
// uses: ?Get@CIME@NUISystem@@SIAAV12@XZ 0x00404A80, ?LoadIMESettings@CIME@NUISystem@@SIXAAVCPersistContext@@@Z 0x00405350,
// ?Initialise@CIME@NUISystem@@QAEXPAUHWND__@@@Z 0x00405650, ?ProcessWinMessage@CIME@NUISystem@@SI_NPAUHWND__@@IIJAAJ@Z 0x00404C70,
// ?FilterMessage@CIME@NUISystem@@SI_NPAUtagMSG@@@Z 0x00404CE0. Out of line in retail. Declarations only.
#include "rebuild_abi.h"
#include "engine/Win32Types.h"
#include "engine/ImeUi.h"
class CPersistContext;
struct tagMSG;
namespace NUISystem {
class CEditBox;
class CIME {
public:
    static CIME& __fastcall Get();                                                              // 0x00404A80
    static void __fastcall LoadIMESettings(CPersistContext& context);                          // 0x00405350
    void Initialise(HWND window_handle);                                                        // 0x00405650
    static bool __fastcall ProcessWinMessage(HWND, unsigned int, unsigned int, long, long&);   // 0x00404C70
    static bool __fastcall FilterMessage(tagMSG* message);                                     // 0x00404CE0
private:
    bool EnableIME;                                         // +0x00
    HWND WindowHandle;                                      // +0x04
    bool IsInitialised;                                     // +0x08
    IMEUI_APPEARANCE Appearance;                            // +0x0c
    CEditBox* PEditBox;                                     // +0x4c
    bool CancelOnOKClick;                                   // +0x50
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CIME) == 84);
#endif

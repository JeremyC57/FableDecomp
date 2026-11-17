// CTCQuickAccessMenu::Deactivate() -- while the menu is active, flag it as quitting (Quitting +0x1C; Active +0x0C)
// Leaf: ?Deactivate@CTCQuickAccessMenu@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00625920, 12 bytes.
// NF64 (round 7, q10651): re-authored on engine/CTCQuickAccessMenu.h (FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE declares it: Ego_r.pdb
//   0x0066A140 / FableWin.pdb 0x02119DD0 ?Deactivate@CTCQuickAccessMenu@@QAEXXZ, out of line here). The body reads the header's Active
//   (+0x0C) and stores Quitting (+0x1C), the offsets its constructor confirms. Every landed caller is a quick-access-menu site (TC 0xF6:
//   0x0041E6D3, 0x00439070, 0x0056AD40, 0x00584A73, 0x005EC0F0, 0x00690FD0). Was catalogued CTCCreditsUI::Deactivate (a local
//   CTCCreditsUI model with the same two offsets; Ego_r.pdb 0x007DFEA0 / FableWin.pdb 0x027994C0 name that member, but no landed
//   caller reaches 0x00625920 as a credits-UI call).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
class CEnginePrimitivePrepareInfo;
#include "engine/CCountedPointer.h"
#include "engine/CTCQuickAccessMenu.h"

void CTCQuickAccessMenu::Deactivate()
{
    if (Active)
        Quitting = true;
}

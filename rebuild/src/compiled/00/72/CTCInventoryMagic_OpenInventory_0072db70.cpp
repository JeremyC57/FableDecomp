// CTCInventoryMagic::OpenInventory() -- select the magic screen in the hero information screens manager, then open the base inventory
// Leaf: ?OpenInventory@CTCInventoryMagic@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072DB70, 31 bytes: CPlayerGui's singleton (PClass 0x013B8790) -> GetPHeroInfoScreensManager
//   0x00437CD0 -> SelectScreenOfType(INVENTORY_MAGIC_SCREEN = 10) 0x0066E799 (the argument pushed first), then a tail jump to
//   NInventory::CTCInventoryBase::OpenInventory 0x005BD4EA.
// Identity: Ego_r.pdb + FableWin.pdb ?OpenInventory@CTCInventoryMagic@@UAEXXZ (engine/CTCInventoryMagic.h PASS22_DTOR declares it);
//   callees Ego_r.pdb / FableWin.pdb ?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@NHeroInformationScreens@@XZ and
//   ?SelectScreenOfType@CManager@NHeroInformationScreens@@QAEXW4EScreenType@2@@Z, ?OpenInventory@CTCInventoryBase@NInventory@@UAEXXZ,
//   the same chain as the landed CTCInventoryQuests::OpenInventory 0x00622A00.
// NF79 (round 7, q10959 / q10960): re-authored on the shared headers (macro block of the landed CloseInventory 0x0072DC40 plus
//   CPlayerGui / NHeroInformationScreens::CManager); was a free __fastcall stand-in ?OpenInventory@@YIXPAUSelf@@@Z calling
//   ?Get@GlobalObj@@ / ?Method@MidObj@@ / ?Show@Self@@ through a made-up global.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYMAGIC_PASS22
#define FABLE_RETAIL_CTCINVENTORYMAGIC_PASS22_DTOR
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#include "engine/CTCInventoryMagic.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CPlayerGui.h"

void CTCInventoryMagic::OpenInventory()
{
    CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(NHeroInformationScreens::INVENTORY_MAGIC_SCREEN);
    NInventory::CTCInventoryBase::OpenInventory();
}

#ifndef FABLE_ENGINE_NHEROINFORMATIONSCREENS_CMANAGER_H
#define FABLE_ENGINE_NHEROINFORMATIONSCREENS_CMANAGER_H
#ifndef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#error "NHeroInformationScreens_CManager.h: define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER (retail model only)"
#endif
// NHeroInformationScreens::CManager (chunk 735, row 0x00577AC0 CTCInventoryExperience::OpenInventoryInternalPC): the hero
// information screens manager CPlayerGui owns through scoped_ptr (CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0). Neither
// PDB gives its data layout (not in struct_layouts_egor.tsv), so only the two methods the row calls are declared, with their
// Ego_r.pdb / FableWin.pdb spellings:
//   ?GetCurrentScreen@CManager@NHeroInformationScreens@@QAEPBVCBase@2@XZ, out of line at 0x0066E7BD (landed);
//   ?SelectScreenOfType@CManager@NHeroInformationScreens@@QAEXW4EScreenType@2@@Z, out of line at 0x0066E799: a `ret 4`
//   thiscall that stores its one argument at +0x28; the row passes INVENTORY_EXPERIENCE_SCREEN (2). The landed label
//   "CObservable::AddExclusiveObserver" (propagated from a byte-identical setter) does not fit this class.
#include "engine/NHeroInformationScreens_CBase.h"
namespace NHeroInformationScreens {
class CManager {
public:
    const CBase* GetCurrentScreen();                    // 0x0066E7BD
    void SelectScreenOfType(EScreenType type);          // 0x0066E799
};
}
#endif

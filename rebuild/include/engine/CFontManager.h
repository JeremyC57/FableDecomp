#ifndef FABLE_ENGINE_CFONTMANAGER_H
#define FABLE_ENGINE_CFONTMANAGER_H
// CFontManager (chunk 1011, row 0x005EA650 CTCTavernGame::OnInitialActivate): the system's font-bank manager. Only the method the
// rows call is declared; the members are not modelled (no layout).
//   Ego_r.pdb 0x00B2C2D0 / FableWin.pdb 0x030A3760 ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z,
//   out of line in retail at 0x009E2C80 (hidden CCountedPointer<CFontBank> result, `ret 8`).
// PFontManager 0x013B838C: the global copy of CSystemManager::PFontManager (+0x84) that GFInitialise 0x004022B0 stores (GOTCHAS
// 2026-11-01). No PDB data symbol names it; `PFontManager` is the spelling the landed rows already bind (0x0064560D, 0x0065431D,
// 0x0042DDB3).
#include "engine/CCountedPointer.h"
class CCharString;
class CFontBank;
class CFontManager {
public:
    CCountedPointer<CFontBank> GetFont(const CCharString& name);         // 0x009E2C80
};
extern CFontManager* PFontManager;                                       // 0x013B838C
#endif

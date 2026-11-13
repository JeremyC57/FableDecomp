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
#ifdef FABLE_RETAIL_CFONTMANAGER_VIRTUAL_DTOR
    // Retail opt-in (agentFAM10, 2026-11-12, row 0x009A6C10 CCountedPointer<CFontManager>::DeleteData): the class is polymorphic
    // (RTTI vtable of the ctor 0x009E2E30) and vtable slot 0 is its scalar deleting destructor (the deleter's `push 1; call [eax]`).
    // Declaration only; the members stay unmodelled.
    virtual ~CFontManager();
#endif
    CCountedPointer<CFontBank> GetFont(const CCharString& name);         // 0x009E2C80
#ifdef FABLE_RETAIL_CFONTMANAGER_ASYNC
    // Opt-in (chunk 2051, row 0x0042EC7C CNewFrontendGameComponent::Run): Ego_r.pdb ?SetLoadFontGlyphsAsynchronously@CFontManager@@QAEX_N@Z, inline in retail (Run 0x0042EFB6 stores the
    // byte +0x38 in place: Ego_r LoadFontGlyphsAsynchronously +0x3C, the CArray 4 bytes smaller in retail).
    void SetLoadFontGlyphsAsynchronously(bool async) { LoadFontGlyphsAsynchronously = async; }
private:
    unsigned char _pad_0x00[0x38];                                       // +0x00 vptr .. NumberAsyncGlyphsPending
    bool LoadFontGlyphsAsynchronously;                                   // +0x38
#endif
};
extern CFontManager* PFontManager;                                       // 0x013B838C
#endif

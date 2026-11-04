#ifndef FABLE_ENGINE_NUISYSTEM_CLISTARROW_H
#define FABLE_ENGINE_NUISYSTEM_CLISTARROW_H
// NUISystem::CListArrow (new, chunk 765): RTTI .?AVCListArrow@NUISystem@@, single direct base NUISystem::CSprite at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01248F94 (+0, slot 0 the ??_G 0x0054C360), 0x01248F6C (+4, CObserver) and 0x01248F64 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CListArrow@NUISystem@@QAE@J@Z, ??0CListArrow@NUISystem@@QAE@ABV01@@Z, ??1CListArrow@NUISystem@@UAE@XZ.
// The constructor 0x0054C1D0 is CSprite(long) 0x0041B800 and the three vptrs; no own members (CManager::CreateComponent news
// the CSprite size).
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CSprite.h"
namespace NUISystem {
class CListArrow : public CSprite {
public:
    CListArrow(long);
    CListArrow(const CListArrow&);
    virtual ~CListArrow();
};
}
#endif

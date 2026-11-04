#ifndef FABLE_ENGINE_NUISYSTEM_CMOUSECURSOR_H
#define FABLE_ENGINE_NUISYSTEM_CMOUSECURSOR_H
// NUISystem::CMouseCursor (new, chunk 765): RTTI .?AVCMouseCursor@NUISystem@@, single direct base NUISystem::CSprite at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124C22C (+0, slot 0 the ??_G 0x0055C8D0), 0x0124C208 (+4, CObserver) and 0x0124C200 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CMouseCursor@NUISystem@@QAE@J@Z, ??0CMouseCursor@NUISystem@@QAE@ABV01@@Z, ??1CMouseCursor@NUISystem@@UAE@XZ.
// The constructor 0x0055C650 registers the CObserver sub-object with CManager (slot 2 AddObserver), observes event 0x19 and makes
// itself the manager's mouse cursor; the destructor 0x0055C510 removes the observer (slot 5 RemoveObserver), clears the manager's
// cursor and tail-jumps ~CSprite 0x0041AB40. No own members (CManager::CreateComponent news the CSprite size).
// Overrides are not declared (no row calls them).
#include "engine/NUISystem_CSprite.h"
namespace NUISystem {
class CMouseCursor : public CSprite {
public:
    CMouseCursor(long);
    CMouseCursor(const CMouseCursor&);
    virtual ~CMouseCursor();
    void GetDataFromDef();
};
}
#endif

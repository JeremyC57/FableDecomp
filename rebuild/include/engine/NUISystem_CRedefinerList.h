#ifndef FABLE_ENGINE_NUISYSTEM_CREDEFINERLIST_H
#define FABLE_ENGINE_NUISYSTEM_CREDEFINERLIST_H
// NUISystem::CRedefinerList (new, chunk 765): RTTI .?AVCRedefinerList@NUISystem@@, single direct base NUISystem::CScrollableList at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124A994 (+0, slot 0 the ??_G 0x00557360), 0x0124A970 (+4, CObserver) and 0x0124A968 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CRedefinerList@NUISystem@@QAE@J@Z, ??0CRedefinerList@NUISystem@@QAE@ABV01@@Z, ??1CRedefinerList@NUISystem@@UAE@XZ.
// Ego_r.pdb members (448 bytes) IsActivated +428, RefreshAfterFade +429, FadeOutTime +432, HaveUndefinedControls +436,
// PUndefinedWarning +440, PAcceptButton +444 sit 0x30 lower in retail (on the 0x17C-byte retail CScrollableList): sizeof 0x190.
// Overrides are not declared (no row calls them).
#include "engine/NUISystem_CScrollableList.h"
namespace NUISystem {
class CRedefinerList : public CScrollableList {
public:
    CRedefinerList(long);
    CRedefinerList(const CRedefinerList&);
    virtual ~CRedefinerList();
#ifdef FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON
    // Opt-in (chunk 880, row 0x0058AF10): ?SetAcceptButton@CRedefinerList@NUISystem@@QAEXPAVCComponent@2@@Z (Ego_r.pdb, public,
    // non-virtual), out of line in retail at 0x00556570 (`mov eax,[esp+4] ; mov [ecx+18Ch],eax ; ret 4`: PAcceptButton +0x18C).
    void SetAcceptButton(CComponent* button);                       // 0x00556570
#endif
protected:
    bool IsActivated;                                               // +0x17c
    bool RefreshAfterFade;                                          // +0x17d
    float FadeOutTime;                                              // +0x180
    bool HaveUndefinedControls;                                     // +0x184
    CComponent* PUndefinedWarning;                                  // +0x188
    CComponent* PAcceptButton;                                      // +0x18c
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CRedefinerList) == 0x190);
#endif

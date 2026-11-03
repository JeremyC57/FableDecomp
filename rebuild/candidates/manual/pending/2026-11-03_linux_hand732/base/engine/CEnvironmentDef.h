#ifndef FABLE_ENGINE_CENVIRONMENTDEF_H
#define FABLE_ENGINE_CENVIRONMENTDEF_H
// New header (chunk 731, 2026-11-03; one model, no class macro): the def class CEnvironmentDef : CDefClassBase (Ego_r.pdb / FableWin.pdb
// ??0/??_E/Alloc/Copy/GetSizeofClass spellings). sizeof 0x134: retail ?GetSizeofClass@CEnvironmentDef@@UAEKXZ 0x004558D9 returns it
// (`mov eax,0x134 ; ret`); scalar deleting destructor 0x00455C1D, Copy 0x0045BE26, constructor 0x004558BF. CEngine::Initialise 0x00B26360 holds it through
// CDefPointer<const CEnvironmentDef> and publishes the raw pointer. Own members are not modelled yet (one opaque block after the packed
// 0x25-byte CDefClassBase; the quarantined generated model engine/_quarantine/CEnvironmentDef.h lists the Ego_r names).
#include "rebuild_abi.h"
#include "engine/CDefClassBase.h"
class CEnvironmentDef : public CDefClassBase {
private:
    unsigned char _pad_0x25[0x10f];                                       // +0x25 members not modelled yet
};
FABLE_STATIC_ASSERT(sizeof(CEnvironmentDef) == 0x134);
#endif // FABLE_ENGINE_CENVIRONMENTDEF_H

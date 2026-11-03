#ifndef FABLE_ENGINE_CENGINEVIDEOOPTIONSDEF_H
#define FABLE_ENGINE_CENGINEVIDEOOPTIONSDEF_H
// New header (chunk 731, 2026-11-03; one model, no class macro): the def class CEngineVideoOptionsDef : CDefClassBase (Ego_r.pdb / FableWin.pdb
// ??0/??_E/Alloc/Copy/GetSizeofClass spellings). sizeof 0x78: retail ?GetSizeofClass@CEngineVideoOptionsDef@@UAEKXZ 0x00434110 returns it
// (`mov eax,0x78 ; ret`); scalar deleting destructor 0x00434220, Copy 0x00434440. CEngine::Initialise 0x00B26360 holds it through
// CDefPointer<const CEngineVideoOptionsDef> and publishes the raw pointer. Own members are not modelled yet (one opaque block after the packed
// 0x25-byte CDefClassBase; the quarantined generated model engine/_quarantine/CEngineVideoOptionsDef.h lists the Ego_r names).
#include "rebuild_abi.h"
#include "engine/CDefClassBase.h"
class CEngineVideoOptionsDef : public CDefClassBase {
private:
    unsigned char _pad_0x25[0x53];                                       // +0x25 members not modelled yet
};
FABLE_STATIC_ASSERT(sizeof(CEngineVideoOptionsDef) == 0x78);
#endif // FABLE_ENGINE_CENGINEVIDEOOPTIONSDEF_H

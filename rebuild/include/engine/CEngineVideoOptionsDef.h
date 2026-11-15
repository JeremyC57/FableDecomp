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
#ifdef FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_HIRESTEXTUREMEMORY
    // Retail opt-in (wfE7, row 0x00B3CB30): Ego_r.pdb member HiresTextureMemory (long; Ego_r +0x40, retail +0x3C: the CEngineResourceManager
    // ctor reads PeekVideoOptionsDef(..).HiresTextureMemory at +0x3C, 0x00B3D823, as the landed CEngine::SetVideoOptions 0x00B24690 does).
private:
    unsigned char _pad_0x25[0x3c - 0x25];
public:
    long HiresTextureMemory;                                             // +0x3C
private:
    unsigned char _pad_0x40[0x78 - 0x40];
#else
private:
    unsigned char _pad_0x25[0x53];                                       // +0x25 members not modelled yet
#endif
#ifdef FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CEngineVideoOptionsDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(14, sizeof(CEngineVideoOptionsDef) == 0x78);
#endif // FABLE_ENGINE_CENGINEVIDEOOPTIONSDEF_H

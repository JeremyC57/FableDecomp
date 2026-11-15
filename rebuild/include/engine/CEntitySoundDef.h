#ifndef FABLE_ENGINE_CENTITYSOUNDDEF_H
#define FABLE_ENGINE_CENTITYSOUNDDEF_H
// Retail model (new shared header, chunk 1021, row 0x0066BC10 CTCSoundPlayer::GetPotentialCriteriaStrings; the generated flat model
// is engine/_quarantine/CEntitySoundDef.h). RTTI .?AVCEntitySoundDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv). Ego_r.pdb
// members: SoundMap (CSoundMap), SoundIdentifier (CCharString), VoicePitchOverride, AnimCriteriaClipDistance (Ego_r sizeof 0x5C).
// Retail offsets: SoundMap +0x28 (4-aligned after the packed 0x25-byte base), SoundIdentifier +0x48 (0x0066BC44 `lea edx,[edi+48h]`;
// the landed PlayAttachedSoundEvent 0x0066B1D0 reads +0x48 too): retail's CSoundMap is 0x20 bytes (Ego_r 0x28), kept opaque here
// (as 4-byte words, for its alignment) until its members are proven. StatGetTypeName is the inline
// ?StatGetTypeName@CEntitySoundDef@@SI?AVCCharString@@XZ (FableWin.pdb), the class-name key GetSubDef<CEntitySoundDef> looks up.
// Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#ifdef FABLE_RETAIL_CENTITYSOUNDDEF_PASS22_SOUNDMAP
#include "engine/CSoundMap.h"
#endif
class CEntitySoundDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CEntitySoundDef", -1); }
#ifdef FABLE_RETAIL_CENTITYSOUNDDEF_PASS22_SOUNDMAP
    // Retail opt-in (Codex pass22): original CSoundMap type, native+0x28, no layout change.
    CSoundMap SoundMap;
#else
    unsigned long SoundMap[0x20 / 4];                                    // +0x28 CSoundMap (retail 0x20 bytes, opaque)
#endif
    CCharString SoundIdentifier;                                         // +0x48
    float VoicePitchOverride;                                            // +0x4C
    long AnimCriteriaClipDistance;                                       // +0x50
#ifdef FABLE_RETAIL_CENTITYSOUNDDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CEntitySoundDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
#endif

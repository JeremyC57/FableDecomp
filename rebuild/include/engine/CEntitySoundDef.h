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
#ifdef FABLE_RETAIL_CENTITYSOUNDDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004564B1: ?GetSizeofClass@CEntitySoundDef@@UAEKXZ (FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
#ifdef FABLE_RETAIL_CENTITYSOUNDDEF_COPY_DECLS
    // Opt-in (NF337, 2026-11-24): declarations only, no layout change, for CEntitySoundDef::Copy 0x00463B9D, which tail-calls the out-of-line
    // operator= (retail `jmp` 0x00463BA2). ?Copy@CEntitySoundDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at 0x00463B9D (slot 19; it overrides CDefClassBase::Copy: clang override probe, made-up-name
    // control fails); ??4CEntitySoundDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb) is the operator= at 0x00463BA2.
public:
    virtual void Copy(const CDefClassBase*);
    CEntitySoundDef& operator=(const CEntitySoundDef&);
#endif
};
#endif

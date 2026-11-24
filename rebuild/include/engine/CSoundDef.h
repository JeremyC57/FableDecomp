#ifndef FABLE_ENGINE_CSOUNDDEF_H
#define FABLE_ENGINE_CSOUNDDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
// PDB class keys/member names; native00417a58 strides20 and8, vectors3c/48.
class CSoundBankEntry {
public: unsigned long ID; CDefString Filename,SymbolFilename,LipSyncBankHandle; bool DVD;
};
class CAtmosBankEntry { public: unsigned long ID; CDefString Filename; };
#ifdef FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES
// Retail opt-in (chunk 1026, row 0x006BD9B0 CMusicManager::InternalUpdate): MusicSetEntries typed as the Ego_r.pdb
// vector<CMusicSetEntry> (12-byte element: NormalMusicID +0, DangerMusicID +4, LoopCount +8; struct_layouts_egor.tsv). Retail
// reads its STLport start / finish at +0x60 / +0x64 and divides the byte span by 12 (`imul 2AAAAAABh ; sar edx,1`). Same offsets.
class CMusicSetEntry { public: long NormalMusicID; long DangerMusicID; long LoopCount; };
#endif
class CSoundDef : public CParentDefClassBase {
public:
    std::vector<CSoundBankEntry> SoundBankEntries;
    std::vector<CAtmosBankEntry> AtmosBankEntries;
#ifdef FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES
    unsigned char MusicEntries[12];
    std::vector<CMusicSetEntry> MusicSetEntries;                    // +0x60
    unsigned char ReverbEnvironmentEntries[12];
#else
    unsigned char MusicEntries[12],MusicSetEntries[12],ReverbEnvironmentEntries[12];
#endif
    float AtmosListenerDampingMin,AtmosListenerDampingRange,DistanceModelCurve;
#ifdef FABLE_RETAIL_CSOUNDDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CSoundDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
#ifdef FABLE_RETAIL_CSOUNDDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x00458454: ?GetSizeofClass@CSoundDef@@UAEKXZ (Ego_r.pdb + FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
#ifdef FABLE_RETAIL_CSOUNDDEF_COPY_DECLS
    // Opt-in (NF386, 2026-11-24; NF337 form): declarations only, no layout change, for CSoundDef::Copy 0x00460A5C, which tail-calls the
    // out-of-line operator= (retail `jmp` 0x00460A61). ?Copy@CSoundDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable
    // placement (ghidra_out/labels_rtti_port.tsv) names at 0x00460A5C (it overrides CDefClassBase::Copy: clang override probe,
    // made-up-name control fails); ??4CSoundDef@@QAEAAV0@ABV0@@Z (PDB name sets) is the landed operator= row at 0x00460A61.
public:
    virtual void Copy(const CDefClassBase*);
    CSoundDef& operator=(const CSoundDef&);
#endif
};
FABLE_STATIC_ASSERT_AT(30, sizeof(CSoundBankEntry)==20);
FABLE_STATIC_ASSERT_AT(31, sizeof(CAtmosBankEntry)==8);
FABLE_STATIC_ASSERT_AT(32, offsetof(CSoundDef,SoundBankEntries)==0x3c);
FABLE_STATIC_ASSERT_AT(33, offsetof(CSoundDef,AtmosBankEntries)==0x48);
FABLE_STATIC_ASSERT_AT(34, sizeof(CSoundDef)==0x84);
#ifdef FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES
FABLE_STATIC_ASSERT_AT(36, sizeof(CMusicSetEntry)==12);
FABLE_STATIC_ASSERT_AT(37, offsetof(CSoundDef,MusicSetEntries)==0x60);
#endif
#endif

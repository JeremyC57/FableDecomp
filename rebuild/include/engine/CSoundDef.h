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
};
FABLE_STATIC_ASSERT(sizeof(CSoundBankEntry)==20);
FABLE_STATIC_ASSERT(sizeof(CAtmosBankEntry)==8);
FABLE_STATIC_ASSERT(offsetof(CSoundDef,SoundBankEntries)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CSoundDef,AtmosBankEntries)==0x48);
FABLE_STATIC_ASSERT(sizeof(CSoundDef)==0x84);
#ifdef FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES
FABLE_STATIC_ASSERT(sizeof(CMusicSetEntry)==12);
FABLE_STATIC_ASSERT(offsetof(CSoundDef,MusicSetEntries)==0x60);
#endif
#endif

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
class CSoundDef : public CParentDefClassBase {
public:
    std::vector<CSoundBankEntry> SoundBankEntries;
    std::vector<CAtmosBankEntry> AtmosBankEntries;
    unsigned char MusicEntries[12],MusicSetEntries[12],ReverbEnvironmentEntries[12];
    float AtmosListenerDampingMin,AtmosListenerDampingRange,DistanceModelCurve;
};
FABLE_STATIC_ASSERT(sizeof(CSoundBankEntry)==20);
FABLE_STATIC_ASSERT(sizeof(CAtmosBankEntry)==8);
FABLE_STATIC_ASSERT(offsetof(CSoundDef,SoundBankEntries)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CSoundDef,AtmosBankEntries)==0x48);
FABLE_STATIC_ASSERT(sizeof(CSoundDef)==0x84);
#endif

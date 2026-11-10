#ifndef FABLE_ENGINE_CSOUNDMAP_H
#define FABLE_ENGINE_CSOUNDMAP_H
// CSoundMap / CSoundPair (chunk 1020, row 0x0047042A CSoundMap::TransferBinaryIn). Retail model, all under FABLE_RETAIL_CSOUNDMAP.
// Ego_r.pdb (struct_layouts_egor.tsv): CSoundPair { long FirstSound +0; long LastSound +4 } (8 bytes; FableWin.pdb user ctors
// ??0CSoundPair@@QAE@XZ and ??0CSoundPair@@QAE@J@Z -- the default one stores nothing: the resize default pair<unsigned long,CSoundPair>()
// zeroes the key only, 0x00470495 `mov [ebp-1Ch],edi`);
// CSoundMap { CVectorMap<unsigned long,CSoundPair,CKeyPairCompareLess<..> > SoundMap +0;
// CVectorMap<unsigned long,CDefString,CKeyPairCompareLess<..> > CriteriaMap } -- 40 bytes in Ego_r (20-byte vector maps), 0x20 in
// retail (the 16-byte TLC CVectorMap: 12-byte STLport vector, Compare +0xC, Dirty +0xD), so CriteriaMap is at +0x10
// (0x0047064D `add esi,10h`). Methods (Ego_r / FableWin): AddEntry, AddCriteria, GetPSoundPair, GetPCriteriaString,
// TransferBinaryIn(CMemoryDataInputStream&) 0x0047042A, TransferBinaryOut(CDataOutputStream&) const 0x0047034B.
// Needs FABLE_RETAIL_CDEFSTRINGTABLE (engine/CDefString.h) and FABLE_RETAIL_STLP_SORT_FASTCALL (CVectorMap::Sort).
#ifdef FABLE_RETAIL_CSOUNDMAP
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
class CMemoryDataInputStream;
class CDataOutputStream;
class CCharString;
class CSoundPair {
public:
    CSoundPair() { }                                         // FableWin.pdb ??0CSoundPair@@QAE@XZ: leaves both sounds unset
    CSoundPair(long sound);                                  // FableWin.pdb ??0CSoundPair@@QAE@J@Z (declaration only)
    long FirstSound;                                         // +0x00
    long LastSound;                                          // +0x04
};
class CSoundMap {
public:
    void AddEntry(const CCharString& name,const CSoundPair& sounds);
    void AddCriteria(const CCharString& name,const CCharString& criteria);
    const CSoundPair* GetPSoundPair(const CCharString& name) const;
    const CDefString* GetPCriteriaString(const CCharString& name) const;
    void TransferBinaryIn(CMemoryDataInputStream& stream);  // 0x0047042A
    void TransferBinaryOut(CDataOutputStream& stream) const; // 0x0047034B
    CVectorMap<unsigned long,CSoundPair> SoundMap;           // +0x00
    CVectorMap<unsigned long,CDefString> CriteriaMap;        // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CSoundMap)==0x20);
#endif
#endif

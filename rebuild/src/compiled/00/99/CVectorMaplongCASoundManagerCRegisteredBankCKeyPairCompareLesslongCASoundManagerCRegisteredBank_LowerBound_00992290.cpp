// CVectorMap<long, CASoundManager::CRegisteredBank, CKeyPairCompareLess<long, CASoundManager::CRegisteredBank> >::LowerBound(const long& key) const -- binary search of the sorted 36-byte registered-bank records for the first whose id is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@JVCRegisteredBank@CASoundManager@@V?$CKeyPairCompareLess@JVCRegisteredBank@CASoundManager@@@@@@QBEPBU?$pair@JVCRegisteredBank@CASoundManager@@@std@@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00992290, 83 bytes, thiscall `ret 4`; the STLport iterator is a plain entry pointer in eax.
// Name (agentNF3, 2026-11-10): the Fable Anniversary 360 PDB publics of lib_sound_manager_base.cpp list
//   ?LowerBound@?$CVectorMap@JVCRegisteredBank@CASoundManager@@V?$CKeyPairCompareLess@JVCRegisteredBank@CASoundManager@@@@@@QBA..ABJ@Z
//   (the Dinkumware const_iterator there; STLport's is a plain pointer); TLC: its callers are CASoundManager::GetPSoundBank(long)
//   const 0x00991840 (the Find on the registered-bank map at +0x30) and 0x00992590, over 36-byte pair<long, CRegisteredBank>
//   entries (`0x38e38e39 / sar 3`). Was landed with a descriptive CUnprovenValue_00992290 value type.
// Body: engine/CVectorMap.h's const LowerBound (count = (last - first) / 36; halving search, signed key compare `jge`, the key read
//   once before the loop, `count -= half + 1` as `or -1 ; sub ; add`), explicitly instantiated.
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS          // hdr overlay: CRegisteredBank records at +0x30
#include "engine/CASoundManager.h"

template CVectorMap<long, CASoundManager::CRegisteredBank>::const_iterator
    CVectorMap<long, CASoundManager::CRegisteredBank>::LowerBound(const long& key) const;

// CASoundManager::GetPSoundBank(long bankID) const -- the sound bank registered under that id (the registered-bank CVectorMap's Find), or null
// Leaf: ?GetPSoundBank@CASoundManager@@QBEPAVCASoundBank@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991840, 53 bytes, `ret 4`.
// Name (agentNF3, 2026-11-10): Fable Anniversary 360 PDB public ?GetPSoundBank@CASoundManager@@QBAPAVCASoundBank@@J@Z
//   (lib_sound_manager_base.cpp, between UnloadAllBanks and ~CASoundManager in the same TU order as TLC) + the TLC body (lookup by
//   key); engine/CASoundManager.h FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP declares it. Its 12 callers (e.g. the landed CSubtitledMessage
//   ctors 0x004CDB70 / 0x004CDBD0) call it on PSoundManager 0x013B8394 with a bank id. Was landed as the descriptive non-const
//   CASoundManager::GetBank(long) over a local layout.
// Callees: CVectorMap<long, CASoundManager::CRegisteredBank>::LowerBound(const long&) const @ 0x00992290 (out of line; the Anniversary
//   PDB lists ?LowerBound@?$CVectorMap@JVCRegisteredBank@CASoundManager@@V?$CKeyPairCompareLess@JVCRegisteredBank@CASoundManager@@@@@@
//   for the same map; the STLport const_iterator is a plain pointer). Was bound as ?LowerBound@?$CVectorMap@JPAVCASoundBank@@@@..SEntry.
// Shape: the inline CVectorMap find (lower bound, then `it == end || id < it->first` -> end) and `if (it != end()) return
//   it->second...;` (found arm first) that re-reads end (`cmp eax,[edi+34h]`); the key is the argument's own slot (`lea eax,[esp+0Ch]`).
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS          // hdr overlay: CRegisteredBank records at +0x30
#include "engine/CASoundManager.h"

CASoundBank* CASoundManager::GetPSoundBank(long bankID) const
{
    CVectorMap<long, CRegisteredBank>::const_iterator it = RegisteredBanks.Find(bankID);
    if (it != RegisteredBanks.end())
        return it->second.PSoundBank;
    return 0;
}

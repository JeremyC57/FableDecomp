// CASoundManager::GetPSoundBank(const CLipSyncDataBank& lipSyncBank) const -- the sound bank of the first registered bank record whose lip-sync bank is the argument, or null
// Leaf: ?GetPSoundBank@CASoundManager@@QBEPAVCASoundBank@@ABVCLipSyncDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991690, length 39 bytes. No callees.
// Name (agentNF3, 2026-11-10): descriptive -- no Anniversary counterpart; the reverse of GetPLipSyncBank 0x009916F0 in the CASoundManager
//   bank-lookup TU (between Update and GetPLipSyncBank), spelled as a third GetPSoundBank overload as engine/CASoundManager.h
//   FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP declares it (the landed callers bind it). Was landed as a descriptive
//   CSymbolMap::FindFirstSymbolWithVal(long) const over a local record layout.
// Shape: begin/end loaded into eax/ecx, an entry `cmp ; je` (the for-loop's top test), the argument loaded once into edx before the
//   aligned loop head (`mov edi,edi` filler), `add eax,24h` stride, `xor eax,eax ; ret 4` for the not-found tail and the found
//   tail placed after it.
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS          // hdr overlay: CRegisteredBank records at +0x30
#include "engine/CASoundManager.h"

CASoundBank* CASoundManager::GetPSoundBank(const CLipSyncDataBank& lipSyncBank) const
{
    for (CVectorMap<long, CRegisteredBank>::const_iterator it = RegisteredBanks.begin(); it != RegisteredBanks.end(); ++it) {
        if (it->second.PLipSyncBank == &lipSyncBank)
            return it->second.PSoundBank;
    }
    return 0;
}

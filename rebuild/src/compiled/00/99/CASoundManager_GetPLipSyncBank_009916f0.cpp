// CASoundManager::GetPLipSyncBank(const CASoundBank& soundBank) const -- the lip-sync bank of the first registered bank record whose sound bank is the argument, or null
// Leaf: ?GetPLipSyncBank@CASoundManager@@QBEPBVCLipSyncDataBank@@ABVCASoundBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009916F0, length 39 bytes. No callees.
// Name (agentNF3, 2026-11-10): Fable Anniversary 360 PDB publics of lib_sound_manager_base.cpp (same TU order: Update,
//   GetPSoundBank(const CCharString&), GetPLipSyncBank, GetAnyPLipSyncBank, UnloadAllBanks, GetPSoundBank(long), CloseAllBanks) + the
//   TLC body; engine/CASoundManager.h FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP declares it (the landed callers bind it).
//   Was landed as a descriptive CSymbolMap::FindFirstSymbolWithVal(long) const over a local record layout.
// Shape: identical to the 0x00991690 twin with the key/result words swapped (`cmp [eax+4],edx` in the aligned loop,
//   `mov eax,[eax+0Ch]` in the found tail after the `xor eax,eax ; ret 4` exit).
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS          // hdr overlay: CRegisteredBank records at +0x30
#include "engine/CASoundManager.h"

const CLipSyncDataBank* CASoundManager::GetPLipSyncBank(const CASoundBank& soundBank) const
{
    for (CVectorMap<long, CRegisteredBank>::const_iterator it = RegisteredBanks.begin(); it != RegisteredBanks.end(); ++it) {
        if (it->second.PSoundBank == &soundBank)
            return it->second.PLipSyncBank;
    }
    return 0;
}

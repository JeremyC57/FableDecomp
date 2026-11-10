// CASoundManager::GetAnyPLipSyncBank() const -- the lip-sync bank of the first registered bank record that has one, or null
// Leaf: ?GetAnyPLipSyncBank@CASoundManager@@QBEPBVCLipSyncDataBank@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991720, length 37 bytes. No callees.
// Name (agentNF3, 2026-11-10): Fable Anniversary 360 PDB publics of lib_sound_manager_base.cpp (same TU order: Update,
//   GetPSoundBank(const CCharString&), GetPLipSyncBank, GetAnyPLipSyncBank, UnloadAllBanks, GetPSoundBank(long), CloseAllBanks) + the
//   TLC body; engine/CASoundManager.h FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP declares it (the landed callers bind it).
//   Was landed as a descriptive CSymbolMap::FindFirstSymbolWithVal() const (catalog _global::ElemLoop0x24_00991720).
// Shape: no argument (`ret`), begin/end into eax/ecx, the top test, the six-byte `lea ebx,[ebx]` loop-alignment filler,
//   `mov edx,[eax+0Ch] ; test edx,edx ; jne` and a SECOND load of the same word in the found tail (VC7.1 does not forward the
//   tested register across the loop exit).
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS          // hdr overlay: CRegisteredBank records at +0x30
#include "engine/CASoundManager.h"

const CLipSyncDataBank* CASoundManager::GetAnyPLipSyncBank() const
{
    for (CVectorMap<long, CRegisteredBank>::const_iterator it = RegisteredBanks.begin(); it != RegisteredBanks.end(); ++it) {
        if (it->second.PLipSyncBank != 0)
            return it->second.PLipSyncBank;
    }
    return 0;
}

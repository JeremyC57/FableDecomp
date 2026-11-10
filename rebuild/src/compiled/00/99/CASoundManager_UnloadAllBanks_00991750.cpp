// CASoundManager::UnloadAllBanks() -- switch the sound off (clear the flag and stop every sound when it was on), then unload the compiled bank of every registered bank record
// Leaf: ?UnloadAllBanks@CASoundManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991750, 53 bytes. Callees: virtual slot 16 StopAllSounds (+0x40) on `this`, CASoundBank slot 2
//   UnloadCompiledBank (+0x08) on each record's sound bank. Called directly from 0x00417614 and 0x0042EC1A (landed binders).
// Name (agentNF3, 2026-11-10): Fable Anniversary 360 PDB publics of lib_sound_manager_base.cpp (same TU order: Update,
//   GetPSoundBank(const CCharString&), GetPLipSyncBank, GetAnyPLipSyncBank, UnloadAllBanks, GetPSoundBank(long), CloseAllBanks) + the
//   TLC body; engine/CASoundManager.h FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP declares it (the landed callers bind it).
//   The catalog's "CThing::OnDie" was a propagated name (landed as a local CThing model with placeholder virtuals).
// Shape: the flag is cleared between the vtable fetch and the call (`mov eax,[edi] ; mov byte ptr [edi+24h],0`); the record loop
//   re-reads the end word after every call (aligned loop head), stride 0x24.
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS          // hdr overlay: CRegisteredBank records at +0x30
#include "engine/CASoundManager.h"
#include "engine/CASoundBank.h"

void CASoundManager::UnloadAllBanks()
{
    if (SoundEnabled) {
        SoundEnabled = false;
        StopAllSounds();
    }
    for (CVectorMap<long, CRegisteredBank>::iterator it = RegisteredBanks.begin(); it != RegisteredBanks.end(); ++it)
        it->second.PSoundBank->UnloadCompiledBank();
}

// CASoundManager::GetPSoundBank(const CCharString& symbolName) const -- the sound bank of the first registered bank record whose symbol name equals the argument (case-insensitive), or null
// Leaf: ?GetPSoundBank@CASoundManager@@QBEPAVCASoundBank@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009917E0, length 87 bytes.
// Callees: _stricmp @ 0x00BFEBA8 (`__cdecl`, `add esp,8`); CCharString::NullChar @ 0x0129AAF4 (the inline GetAsCharArray).
// Name (agentNF3, 2026-11-10): Fable Anniversary 360 PDB publics of lib_sound_manager_base.cpp (same TU order: Update,
//   GetPSoundBank(const CCharString&), GetPLipSyncBank, GetAnyPLipSyncBank, UnloadAllBanks, GetPSoundBank(long), CloseAllBanks) + the
//   TLC body; engine/CASoundManager.h FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP declares it (the landed callers bind it).
//   Was landed as a descriptive CSymbolMap::FindFirstSymbolWithVal(const CCharString&) const over a local record layout.
// Shape: `this` in edi because the end word (+0x34) is re-read after every call; the key reference in ebx and its text re-derived
//   each iteration (the call may change it); VC evaluates the right argument (the key) first.
#define FABLE_RETAIL_CCHARSTRING                            // inline GetAsCharArray (NullChar 0x0129AAF4)
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_REGISTEREDBANKS          // hdr overlay: CRegisteredBank records at +0x30
#include "engine/CASoundManager.h"
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8

CASoundBank* CASoundManager::GetPSoundBank(const CCharString& symbolName) const
{
    for (CVectorMap<long, CRegisteredBank>::const_iterator it = RegisteredBanks.begin(); it != RegisteredBanks.end(); ++it) {
        if (_stricmp(it->second.SymbolName.GetAsCharArray(), symbolName.GetAsCharArray()) == 0)
            return it->second.PSoundBank;
    }
    return 0;
}

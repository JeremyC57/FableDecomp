// CDisplayManager::ChangePresentationInterval(bool, long, bool) -- builds a CPresentationIntervalDesc from its three fields and applies it
// Leaf: ?ChangePresentationInterval@CDisplayManager@@QAEX_NJ0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BFF10, length 41 bytes (the 48-byte catalog row ends in int3 padding).
// Callees: CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc&) @ 0x009BFE70.
// Callers: 0x00418A35 and 0x00B2477A (`push 1 ; push 1 ; push <bool> ; call` on the display manager global).
// Label: the manifest's "CTCInventoryAbilities::AddLearnedExpression" is propagated; the callee and the callers
//        identify the class (the field order comes from the PDB CPresentationIntervalDesc: PresentImmediate +0,
//        PresentImmediateIfSyncMissed +1, VSyncCount +4).
// Shape: the descriptor is an 8-byte frame local, aggregate-initialised (no constructor in either PDB; the two bytes stored first,
//        the dword after the address push) and passed by reference.
// NF145 (q11808): on engine/CDisplayManager.h (opt-in _RESETDEVICE). The body now defines the PDB leaf its Leaf line already named
//   (Ego_r.pdb 0x00B24FA0 / FableWin.pdb 0x0301CD60 ?ChangePresentationInterval@CDisplayManager@@QAEX_NJ0@Z; was the stand-in
//   ?SetPresentParameters@CDisplayManager@@QAEX_NJ_N@Z) and binds 0x009BFE70 under its PDB spelling. CPresentationIntervalDesc has no
//   constructor in either PDB: the local is aggregate-initialised (PresentImmediate, PresentImmediateIfSyncMissed, VSyncCount).
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#define FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
#define FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"

void CDisplayManager::ChangePresentationInterval(bool present_immediate, long vsync_count, bool present_immediate_if_sync_missed)
{
    CPresentationIntervalDesc desc = { present_immediate, present_immediate_if_sync_missed, vsync_count };
    ChangePresentationInterval(desc);
}

// CMusicManager::CState::GetActiveSet() const -- the music state's active set: the first non-zero of the UI override, cinematic override, override and map-cell sets, else the region set
// Leaf: ?GetActiveSet@CState@CMusicManager@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BD760, 31 bytes, thiscall, plain `ret`.
// Identity (agentNF13, 2026-11-11; was the free __fastcall CMusicManager_IsDangerMusicEnabled): FableWin.pdb names
//   ?GetActiveSet@CState@CMusicManager@@QBEJXZ; the body reads only Ego_r.pdb CMusicManager::CState members (RegionSet +0x00,
//   OverrideSet +0x04, UIOverrideSet +0x08, CinematicOverrideSet +0x0C, MapCellSet +0x10) and returns the first non-zero one in
//   priority order; CMusicManager::InternalUpdate 0x006BD9B0 calls it on both states (landed binding) and the landed
//   CState::GetDanger 0x006BD780 is its byte-twin caller. IsDangerMusicEnabled is 0x006BD840 (the manager's own accessor).
// Model: engine/CMusicManager.h (the CState class; its inline body is declared out of line here under the opt-in).
// Shape: every test loads into eax, where the result is returned (`mov eax,[ecx+8] ; test eax,eax ; jne`), the last one too.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CMUSICMANAGER_GETACTIVESET_OUTOFLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CMusicManager.h"

long CMusicManager::CState::GetActiveSet() const
{
    if (UIOverrideSet)
        return UIOverrideSet;
    if (CinematicOverrideSet)
        return CinematicOverrideSet;
    if (OverrideSet)
        return OverrideSet;
    if (MapCellSet)
        return MapCellSet;
    return RegionSet;
}

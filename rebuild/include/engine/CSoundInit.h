#ifndef FABLE_ENGINE_CSOUNDINIT_H
#define FABLE_ENGINE_CSOUNDINIT_H
// Retail CSoundInit (agentSND, 2026-11-10): the start parameters every sound-play call takes by `const CSoundInit&`
// (CASoundEmitter::PlaySample / PlaySoundEvent, CTCSoundPlayer::PlayAttachedSample; Ego_r.pdb / FableWin.pdb signatures and the
// Fable Anniversary 360 PDB publics). Layout: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) CSoundInit, 0x20 bytes -- Volume +0,
// Pitch +4, Pan +8 (floats), NoPlays +0xC, PlaybackType +0x10, GainFaderBits +0x14, PitchFaderBits +0x18, OverrideID +0x1C.
// Constructor: Ego_r.pdb ??0CSoundInit@@QAE@XZ, out of line in retail at 0x0041C930 (landed; defaults 1, 1, 0, 1, type 1, 0, 0, -1).
// GainFaderBits selects the CASoundManager gain-fader groups (CASoundManager::SetGainFader, slot 46) the sound follows.
#include "rebuild_abi.h"

enum ESoundPlaybackType { };                    // Ego_r.pdb enum; enumerator names not recovered (the ctor stores 1)

class CSoundInit {
public:
    float              Volume;                  // +0x00
    float              Pitch;                   // +0x04
    float              Pan;                     // +0x08
    long               NoPlays;                 // +0x0C
    ESoundPlaybackType PlaybackType;            // +0x10
    unsigned long      GainFaderBits;           // +0x14
    unsigned long      PitchFaderBits;          // +0x18
    long               OverrideID;              // +0x1C
    CSoundInit();                               // 0x0041C930 ??0CSoundInit@@QAE@XZ
};
FABLE_STATIC_ASSERT(sizeof(CSoundInit) == 0x20);

#endif // FABLE_ENGINE_CSOUNDINIT_H

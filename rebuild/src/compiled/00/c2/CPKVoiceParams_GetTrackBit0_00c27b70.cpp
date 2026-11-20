// CPKVoiceParams::GetTrackBit0() const -- bit 0 of the voice record's byte at +0x44 (CSoundEvent track flag 1)
// Leaf: ?GetTrackBit0@CPKVoiceParams@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27B70: `xor eax,eax ; mov al,[ecx+44h] ; and eax,1 ; ret`.
// Identity (descriptive; libpk has no PDB): its only caller CSoundEventManager::StartEvent 0x00C12940 passes GetTrackBit0/1/2 as
//   CSoundEvent::Initialise's 13th..15th arguments (the +0xAE track flags 1 / 2 / 4) and already binds this spelling; the
//   catalog's propagated physics / vertex labels were wrong. Layout: libpk/CPKVoiceParams.h (0x50 bytes) under the NF165 opt-in.
#define FABLE_RETAIL_CPKVOICEPARAMS_TRACKBITS
#include "libpk/CPKVoiceParams.h"

bool CPKVoiceParams::GetTrackBit0() const
{
    return TrackFlags & 1;
}

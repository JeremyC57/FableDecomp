// CPKVoiceParams::GetTrackBit1() const -- bit 1 of the voice record's byte at +0x44 (CSoundEvent track flag 2)
// Leaf: ?GetTrackBit1@CPKVoiceParams@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27B80: `xor eax,eax ; mov al,[ecx+44h] ; shr eax,1 ; and eax,1 ; ret`.
// Identity (descriptive; libpk has no PDB): its only caller CSoundEventManager::StartEvent 0x00C12940 passes GetTrackBit0/1/2 as
//   CSoundEvent::Initialise's 13th..15th arguments (the +0xAE track flags 1 / 2 / 4) and already binds this spelling; the
//   catalog's propagated physics / vertex labels were wrong. Layout: libpk/CPKVoiceParams.h (0x50 bytes) under the NF165 opt-in.
#define FABLE_RETAIL_CPKVOICEPARAMS_TRACKBITS
#include "libpk/CPKVoiceParams.h"

bool CPKVoiceParams::GetTrackBit1() const
{
    return (TrackFlags >> 1) & 1;
}

#include <stdio.h>
CPKVoiceParams::CPKVoiceParams() { }
int main()
{
    CPKVoiceParams p;
    for (unsigned v = 0; v < 256; ++v) {
        p.TrackFlags = (unsigned char)v;
        bool expect = ((v >> 1) & 1) != 0;
        if (p.GetTrackBit1() != expect) { printf("00c27b80_TEST FAIL\n"); return 1; }
    }
    if (sizeof(void*) == 4 && sizeof(CPKVoiceParams) != 0x50) { printf("00c27b80_TEST FAIL\n"); return 1; }
    printf("00c27b80_TEST PASS\n");
    return 0;
}

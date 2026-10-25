// MixFilteredVoiceFadeIn(const short* buffer, const SFilteredVoice* voice, long* front, long* rear) -- fade a filtered voice in over one block: resample 0x10 quads from `offset` samples back into the front/rear buffers (stereo when the master has 4+ output channels, outputs swapped by the voice flag) with the gain ramping from 0 by 0x100 per sample
// Leaf: ?MixFilteredVoiceFadeIn@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C70C10, 160 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer, the filtered resampling voice effect of vtable 0x012BEDE0 (the 0x2a4-byte-record twin
//   of vtable 0x012BEDCC, whose MixVoiceFadeIn / MixVoiceFadeOut / MixVoice 0x00C70240 / 0x00C702E0 / 0x00C70390 are
//   this file's byte-for-byte shapes with the record fields moved up by 0x180, a 0x10-quad block and the FIR
//   resamplers). Its per-block Process 0x00C70E50 (slot 2) calls these with the current / pending set records.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions; callees are the FIR resamplers
//   (inline-asm bodies; the mono one reads 0x50 bytes of history before the source) 0x00C843F0 / 0x00C7E420 (ramped)
//   and 0x00C812F0 / 0x00C7BAF0 (constant gain), all __cdecl.
// Callers: 0x00C70EE4, 0x00C70F04 (Process 0x00C70E50).
// Shape: the trailing `0, 0x100` arguments are common to all three calls, so VC7.1 hoists their pushes above the channel test
//   and cross-jumps the swapped arm into the shared push tail; the `pan` word is an `unsigned short` argument.
struct CAudioMaster {                                  // libpk's master object (global 0x0143BC2C)
    unsigned char _pad000[0x1bc];
    unsigned long m_outputChannels;                    // +0x1bc (4 and up: the rear pair is mixed too)
};
extern CAudioMaster* g_pAudioMaster;                   // 0x0143BC2C
struct SFilteredVoice {                                // 0x2a4-byte voice record (two per set, sets at effect +0xc4c / +0xc50)
    unsigned char  _pad000[0x290];
    unsigned long  m_phase;                            // +0x290
    unsigned char  _pad294[0x4];
    long           m_offset;                           // +0x298 samples back from the block start
    unsigned long  m_increment;                        // +0x29c
    unsigned char  m_muted;                            // +0x2a0
    unsigned char  m_swapOutputs;                      // +0x2a1
    unsigned short m_pan;                              // +0x2a2
};
void ResampleStereoRampFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                           long* first, long* second, unsigned short pan, long startGain, long gainStep);   // 0x00C843F0
void ResampleMonoRampFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                         long* output, long startGain, long gainStep);                                      // 0x00C7E420
void ResampleStereoFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                       long* first, long* second, unsigned short pan);                                     // 0x00C812F0
void ResampleMonoFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                     long* output);                                                                         // 0x00C7BAF0
void MixFilteredVoiceFadeIn(const short* buffer, const SFilteredVoice* voice, long* front, long* rear)
{
    if (g_pAudioMaster->m_outputChannels >= 4) {
        if (voice->m_swapOutputs)
            ResampleStereoRampFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x10, rear, front, voice->m_pan, 0, 0x100);
        else
            ResampleStereoRampFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x10, front, rear, voice->m_pan, 0, 0x100);
    } else
        ResampleMonoRampFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x10, front, 0, 0x100);
}
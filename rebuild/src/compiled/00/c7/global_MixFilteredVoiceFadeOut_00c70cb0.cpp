// MixFilteredVoiceFadeOut(const short* buffer, const SFilteredVoice* voice, long* front, long* rear) -- fade a filtered voice out over one block: as MixFilteredVoiceFadeIn 0x00C70C10 with the gain ramping down from 0x7fff by 0x100 per sample
// Leaf: ?MixFilteredVoiceFadeOut@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C70CB0, 163 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer, the filtered resampling voice effect of vtable 0x012BEDE0 (the 0x2a4-byte-record twin
//   of vtable 0x012BEDCC, whose MixVoiceFadeIn / MixVoiceFadeOut / MixVoice 0x00C70240 / 0x00C702E0 / 0x00C70390 are
//   this file's byte-for-byte shapes with the record fields moved up by 0x180, a 0x10-quad block and the FIR
//   resamplers). Its per-block Process 0x00C70E50 (slot 2) calls these with the current / pending set records.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions; callees are the FIR resamplers
//   (inline-asm bodies; the mono one reads 0x50 bytes of history before the source) 0x00C843F0 / 0x00C7E420 (ramped)
//   and 0x00C812F0 / 0x00C7BAF0 (constant gain), all __cdecl.
// Callers: 0x00C70E98, 0x00C70EB8 (Process 0x00C70E50).
// Shape: as MixFilteredVoiceFadeIn: the common `0x7fff, -0x100` pushes are hoisted above the channel test.
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
void MixFilteredVoiceFadeOut(const short* buffer, const SFilteredVoice* voice, long* front, long* rear)
{
    if (g_pAudioMaster->m_outputChannels >= 4) {
        if (voice->m_swapOutputs)
            ResampleStereoRampFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x10, rear, front, voice->m_pan, 0x7fff, -0x100);
        else
            ResampleStereoRampFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x10, front, rear, voice->m_pan, 0x7fff, -0x100);
    } else
        ResampleMonoRampFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x10, front, 0x7fff, -0x100);
}
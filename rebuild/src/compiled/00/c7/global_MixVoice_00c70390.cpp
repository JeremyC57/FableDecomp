// MixVoice(const short* buffer, unsigned long count, const SResampleVoice* voice, long* front, long* rear) -- mix an unmuted voice at its constant gain: resample `count` quads into the front/rear buffers (stereo with 4+ output channels, outputs swapped by the voice flag) or into the front buffer alone
// Leaf: ?MixVoice@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C70390, 228 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer, the resampling voice effect of vtable 0x012BEDCC: its per-block Process 0x00C70480
//   (slot 2) copies 128 samples into the history buffer (+0x1b8) and mixes the two 0x124-byte voice records of the
//   current set (+0x64c; record 0 into outputs 0/2, record 1 into outputs 1/3): after a parameter change (+0x24 flag)
//   the previous set fades out (MixVoiceFadeOut 0x00C702E0), the sets swap and the new one fades in (MixVoiceFadeIn
//   0x00C70240), otherwise MixVoice 0x00C70390 runs with count 0x20. With 4+ output channels on the master object
//   (global 0x0143BC2C, +0x1bc) the stereo MMX resampler feeds both the front and the rear buffer (+0x121 swaps them),
//   else the mono one feeds the front buffer only.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions; callees are the MMX resamplers
//   (inline-asm bodies) 0x00C7AB20 / 0x00C79440 (ramped) and 0x00C79FD0 / 0x00C78A10 (constant gain), all __cdecl.
// Callers: 0x00C70555, 0x00C70578 (Process 0x00C70480).
// Shape: the mono call takes five arguments like this function, so VC7.1 turns it into a tail call that rewrites the
//   argument slots in place (`mov [esp+4..14h]` ; `jmp 0x00C78A10`); the two stereo arms stay separate calls.
struct CAudioMaster {                                  // libpk's master object (global 0x0143BC2C)
    unsigned char _pad000[0x1bc];
    unsigned long m_outputChannels;                    // +0x1bc (4 and up: the rear pair is mixed too)
};
extern CAudioMaster* g_pAudioMaster;                   // 0x0143BC2C
struct SResampleVoice {                                // 0x124-byte voice record (two per set, sets at effect +0x64c / +0x650)
    unsigned char  _pad000[0x110];
    unsigned long  m_phase;                            // +0x110
    unsigned char  _pad114[0x4];
    long           m_offset;                           // +0x118 samples back from the block start
    unsigned long  m_increment;                        // +0x11c
    unsigned char  m_muted;                            // +0x120
    unsigned char  m_swapOutputs;                      // +0x121
    unsigned short m_pan;                              // +0x122
};
void ResampleStereoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                           long* first, long* second, unsigned short pan, long startGain, long gainStep);   // 0x00C7AB20
void ResampleMonoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                         long* output, long startGain, long gainStep);                                      // 0x00C79440
void ResampleStereoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                       long* first, long* second, unsigned short pan);                                     // 0x00C79FD0
void ResampleMonoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                     long* output);                                                                         // 0x00C78A10
void MixVoice(const short* buffer, unsigned long count, const SResampleVoice* voice, long* front, long* rear)
{
    if (voice->m_muted)
        return;
    if (g_pAudioMaster->m_outputChannels >= 4) {
        if (voice->m_swapOutputs)
            ResampleStereoMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, rear, front, voice->m_pan);
        else
            ResampleStereoMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, front, rear, voice->m_pan);
    } else
        ResampleMonoMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, front);
}
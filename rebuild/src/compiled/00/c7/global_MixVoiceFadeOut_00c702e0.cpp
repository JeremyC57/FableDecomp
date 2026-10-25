// MixVoiceFadeOut(const short* buffer, const SResampleVoice* voice, long* front, long* rear) -- fade a voice out over one block: as MixVoiceFadeIn 0x00C70240 with the gain ramping down from 0x7fff by 0x100 per sample
// Leaf: ?MixVoiceFadeOut@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C702E0, 163 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer, the resampling voice effect of vtable 0x012BEDCC: its per-block Process 0x00C70480
//   (slot 2) copies 128 samples into the history buffer (+0x1b8) and mixes the two 0x124-byte voice records of the
//   current set (+0x64c; record 0 into outputs 0/2, record 1 into outputs 1/3): after a parameter change (+0x24 flag)
//   the previous set fades out (MixVoiceFadeOut 0x00C702E0), the sets swap and the new one fades in (MixVoiceFadeIn
//   0x00C70240), otherwise MixVoice 0x00C70390 runs with count 0x20. With 4+ output channels on the master object
//   (global 0x0143BC2C, +0x1bc) the stereo MMX resampler feeds both the front and the rear buffer (+0x121 swaps them),
//   else the mono one feeds the front buffer only.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions; callees are the MMX resamplers
//   (inline-asm bodies) 0x00C7AB20 / 0x00C79440 (ramped) and 0x00C79FD0 / 0x00C78A10 (constant gain), all __cdecl.
// Callers: 0x00C704C8, 0x00C704E8 (Process 0x00C70480).
// Shape: the twin of MixVoiceFadeIn 0x00C70240 (hoisted `push 0FFFFFF00h ; push 7FFFh`, cross-jumped swapped arm).
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
void MixVoiceFadeOut(const short* buffer, const SResampleVoice* voice, long* front, long* rear)
{
    if (g_pAudioMaster->m_outputChannels >= 4) {
        if (voice->m_swapOutputs)
            ResampleStereoRampMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x20, rear, front, voice->m_pan, 0x7fff, -0x100);
        else
            ResampleStereoRampMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x20, front, rear, voice->m_pan, 0x7fff, -0x100);
    } else
        ResampleMonoRampMMX(buffer - voice->m_offset, voice->m_phase, voice->m_increment, 0x20, front, 0x7fff, -0x100);
}
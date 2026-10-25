// MixFilteredVoice(const short* buffer, unsigned long count, const SFilteredVoice* voice, long* front, long* rear) -- mix an unmuted filtered voice at its constant gain: resample `count` quads into the front/rear buffers (stereo with 4+ output channels, outputs swapped by the voice flag) or into the front buffer alone
// Leaf: ?MixFilteredVoice@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C70D60, 228 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer, the filtered resampling voice effect of vtable 0x012BEDE0 (the 0x2a4-byte-record twin
//   of vtable 0x012BEDCC, whose MixVoiceFadeIn / MixVoiceFadeOut / MixVoice 0x00C70240 / 0x00C702E0 / 0x00C70390 are
//   this file's byte-for-byte shapes with the record fields moved up by 0x180, a 0x10-quad block and the FIR
//   resamplers). Its per-block Process 0x00C70E50 (slot 2) calls these with the current / pending set records.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions; callees are the FIR resamplers
//   (inline-asm bodies; the mono one reads 0x50 bytes of history before the source) 0x00C843F0 / 0x00C7E420 (ramped)
//   and 0x00C812F0 / 0x00C7BAF0 (constant gain), all __cdecl.
// Callers: 0x00C70F25, 0x00C70F48 (Process 0x00C70E50).
// Shape: the mono call takes five arguments like this function, so VC7.1 turns it into a tail call that rewrites the
//   argument slots in place (`mov [esp+4..14h]` ; `jmp 0x00C7BAF0`); the two stereo arms stay separate calls.
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
void MixFilteredVoice(const short* buffer, unsigned long count, const SFilteredVoice* voice, long* front, long* rear)
{
    if (voice->m_muted)
        return;
    if (g_pAudioMaster->m_outputChannels >= 4) {
        if (voice->m_swapOutputs)
            ResampleStereoFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, rear, front, voice->m_pan);
        else
            ResampleStereoFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, front, rear, voice->m_pan);
    } else
        ResampleMonoFIR(buffer - voice->m_offset, voice->m_phase, voice->m_increment, count, front);
}
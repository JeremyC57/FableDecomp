// Grade: DIFFER(240v236) -- each delay line's memset byte count: retail `mov ecx,ebx ; shl ecx,2` before the length/position stores with the buffer re-read after them; ours `lea ecx,[ebx*4]` after an early buffer re-read (4 bytes longer, same instructions otherwise)
// CReverbProcessor_00c6a460::AllocateMemory(const SReverbConfig& config, unsigned char*& memory, unsigned long& size) -- carve the two delay lines and the 128-float work block out of `memory` (when given), zeroing each, and add every block's size to `size`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69A70, 236 bytes (row padded to 240), `ret 0Ch`.
// Label: the manifest's "NScript::CQ_GuildTrainingSkillScript::AllocateMemory" is propagated: the body belongs to the audio
// DSP module at 0x00C6xxxx. Its only caller is the processor's set-up routine @ 0x00C6A460 (`AllocateMemory(config, &memory,
// &size)` with memory and size zeroed first, then four 0x14-byte comb filters at +0x1F8 initialised). Class and member names
// are descriptive.
// Layout: delay line A {buffer, length, position} at +0x54, delay line B at +0x60, the 0x200-byte work block pointer at +0x288;
// the config's lengths at +0x0c (A) and +0x10 / +0x14 (B = their sum).
// Shape: each length is rounded up to a multiple of 4 floats; a delay line's inline Initialise stores buffer, length and a zero
// position, then `memset(buffer, 0, length * 4)` re-reading the stored buffer (`rep stosd ; rep stosb`); the memory cursor
// advances AFTER the memset for the lines but BEFORE it for the work block.
#include <string.h>

struct SReverbConfig {
    unsigned char _pad00[0x0c];
    unsigned long DelayLengthA;                    // +0x0c
    unsigned long DelayLengthB1;                   // +0x10
    unsigned long DelayLengthB2;                   // +0x14
};

struct CDelayLine {
    float*        m_pBuffer;                       // +0x00
    unsigned long m_length;                        // +0x04
    unsigned long m_position;                      // +0x08
    void Initialise(float* buffer, unsigned long length)
    {
        m_pBuffer = buffer;
        m_length = length;
        m_position = 0;
        memset(m_pBuffer, 0, length * sizeof(float));
    }
};

struct CReverbProcessor_00c6a460 {
    unsigned char _pad00[0x54];
    CDelayLine    m_lineA;                         // +0x54
    CDelayLine    m_lineB;                         // +0x60
    unsigned char _pad6c[0x288 - 0x6c];            // filters, combs (+0x1f8)
    float*        m_pWork;                         // +0x288 (128 floats)
    void AllocateMemory(const SReverbConfig& config, unsigned char*& memory, unsigned long& size);
};

void CReverbProcessor_00c6a460::AllocateMemory(const SReverbConfig& config, unsigned char*& memory, unsigned long& size)
{
    unsigned long lengthA = (config.DelayLengthA + 3) & ~3;
    if (memory) {
        m_lineA.Initialise((float*)memory, lengthA);
        memory += lengthA * sizeof(float);
    }
    size += lengthA * sizeof(float);

    unsigned long lengthB = (config.DelayLengthB2 + config.DelayLengthB1 + 3) & ~3;
    if (memory) {
        m_lineB.Initialise((float*)memory, lengthB);
        memory += lengthB * sizeof(float);
    }
    size += lengthB * sizeof(float);

    if (memory) {
        m_pWork = (float*)memory;
        memory += 128 * sizeof(float);
        memset(m_pWork, 0, 128 * sizeof(float));
    }
    size += 128 * sizeof(float);
}

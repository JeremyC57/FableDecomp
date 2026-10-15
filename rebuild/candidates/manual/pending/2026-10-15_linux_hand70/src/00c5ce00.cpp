// CAudioRingBuffer00c5ce00::Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels, unsigned long playPosition, unsigned long queued) -- set up a streaming ring-buffer cursor: store the six parameters and derive the write position (play + queued) modulo the capacity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CE00, length 56 bytes (`ret 18h`, returns nothing). No callees.
// Label correction: the manifest calls this a CReactionMatch_SecondaryAxisCentredMRShifted constructor (a propagated
// name: 9 parameters, floats). The bytes say otherwise -- no vtable store, no `mov eax,ecx` (not a constructor), six
// dword arguments and an UNSIGNED `div`. Callers 0x00C556E0 / 0x00C5C290 pass (16-byte-aligned static buffer, 0x80,
// 2, channels, 0, 0) on the record at +0x1F0 of an audio device; 0x00C5A3A0 passes (base + offset, frames,
// channels * sampleBytes, channels, 0, frames). Its neighbours in the same object (0x00C5CE60: `pos / [+0Ch]`, rebase
// of +0x1C/+0x18 modulo +0x08; 0x00C5CF00: `+0x1C += n ; +0x18 += n`, wrap at +0x08; 0x00C5CF20: `+0x14 += n ;
// +0x1C -= n`, wrap at +0x08) work the same record: +0x04 is the sample memory, +0x08 the ring capacity, +0x0C the
// bytes per frame, +0x10 the channel count, +0x14 the play cursor, +0x18 the write cursor and +0x1C the queued count.
// The class name is descriptive (no RTTI, no PDB match).
// Shape: plain member assignments in offset order with the remainder computed from the stored members (the
// compiler forwards the arguments: `add eax,edx ; xor edx,edx ; div esi`).
// Leaf: Init
struct CAudioRingBuffer00c5ce00 {
    unsigned long  m_unknown00;                         // +0x00 (not touched here)
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08 ring size
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_playPosition;                      // +0x14
    unsigned long  m_writePosition;                     // +0x18
    unsigned long  m_queued;                            // +0x1c
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long playPosition, unsigned long queued);
};

void CAudioRingBuffer00c5ce00::Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
                                    unsigned long playPosition, unsigned long queued)
{
    m_pData = data;
    m_capacity = capacity;
    m_frameBytes = frameBytes;
    m_channels = channels;
    m_playPosition = playPosition;
    m_queued = queued;
    m_writePosition = (m_playPosition + m_queued) % m_capacity;
}

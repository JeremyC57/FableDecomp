// CAudioRingBuffer::CommitRead(unsigned long frames) -- account for `frames` consumed frames: drop the queued count and advance the read cursor; an emptied ring rewinds both cursors to 0, otherwise the read cursor wraps once at the capacity
// Leaf: ?CommitRead@CAudioRingBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5CF20, length 52 bytes; `ret 4`.
// Callers: 0x00C5500B, 0x00C5A280, 0x00C5A69F, 0x00C5B0DD (the voice's resampler 0x00C5B030), 0x00C5B158, 0x00C5B723.
// Label: the manifest's "AudioBuffer_AdjustPlaybackPosition" is propagated.
// Shape: statement order picks the registers: `m_queued -= frames;` BEFORE `m_readPos += frames;` gives retail's
// queued-first load into edx, the cursor in edi, an explicit `test edx,edx` and the zero stores through edx (the
// known-zero queued value); the other order (or a `return` form) loads the cursor first into eax/esi and stores a
// fresh `xor eax,eax` (50 bytes).
// The GameCODA streaming ring-buffer cursor (vtable 0x012B8FC4, base vtable 0x012B5868; ctor 0x00C5D0A0, Init
// 0x00C5CE00 landed as `CAudioRingBuffer00c5ce00`): sample memory +4, capacity in frames +8, bytes per frame +0xC,
// channels +0x10, read cursor +0x14, write cursor +0x18, queued frames +0x1C. One sits at +0xB4 of every software
// voice; the global mix rings at 0x0143BF68 / 0x0143BF88 are planar (channel c at data + c * capacity * 2).
class CAudioRingBuffer {
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;             // +0x04
    unsigned long  m_capacity;          // +0x08 frames
    unsigned long  m_frameBytes;        // +0x0c
    unsigned long  m_channels;          // +0x10
    unsigned long  m_readPos;           // +0x14
    unsigned long  m_writePos;          // +0x18
    unsigned long  m_queued;            // +0x1c
    void SetCapacity(unsigned long bytes, bool keepQueued);
    void CommitWrite(unsigned long frames);
    void CommitRead(unsigned long frames);
    void CopyChannel(unsigned long dstChannel, unsigned long srcChannel);
    void ClearChannel(unsigned long channel, unsigned long frames);
    void CopyFrom(const CAudioRingBuffer* src, unsigned long frames);
};

void CAudioRingBuffer::CommitRead(unsigned long frames)
{
    m_queued -= frames;
    m_readPos += frames;
    if (m_queued == 0) {
        m_readPos = 0;
        m_writePos = 0;
    } else if (m_readPos >= m_capacity)
        m_readPos -= m_capacity;
}
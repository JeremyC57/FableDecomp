// SBufferedStreamState00c8d970::Reset() -- rewind a 0x168-byte buffered stream/codec state: clear its cursors and counters and mark the current entry invalid (-1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D970, length 70 bytes (`ret`, nothing returned). No callees.
// Label correction: the manifest calls this a CRegionMinimap constructor (propagated). It is not a constructor (no
// `mov eax,ecx`, nothing returned) and it writes up to +0x164, far past CRegionMinimap's 0x64 bytes. It belongs to the
// LTCG-compiled C-style module at 0x00C8xxxx: 0x00C8CE20 zero-fills the same 0x168-byte record (`rep stosd`, 0x5A
// dwords), allocates its buffers (+0x00: 0x4000 bytes, size at +0x04; +0x10: 0x1000 bytes; +0x14: 0x400 eight-byte
// entries, count at +0x18) and stores a caller value at +0x150; 0x00C8D9C0 calls this reset and then stores +0x150; the
// caller at 0x00C8AC3F resets a stack instance right after initialising it. The record's purpose is not established,
// so the struct and member names are descriptive (no RTTI, no PDB match).
// Shape: thirteen stores in offset order with eax as the zero register and an immediate -1.
// Leaf: Reset
struct SBufferedStreamState00c8d970 {
    unsigned char* m_pBuffer;                           // +0x000 (0x4000 bytes)
    unsigned long  m_bufferSize;                        // +0x004
    unsigned long  m_readPosition;                      // +0x008
    unsigned long  m_writePosition;                     // +0x00c
    unsigned char* m_pWorkBuffer;                       // +0x010 (0x1000 bytes)
    void*          m_pEntries;                          // +0x014 (eight-byte entries)
    unsigned long  m_entryCapacity;                     // +0x018
    unsigned long  m_entryCount;                        // +0x01c
    unsigned long  m_counter20;                         // +0x020
    unsigned long  m_counter24;                         // +0x024
    unsigned char  _table28[0x11c];                     // +0x028
    unsigned long  m_counter144;                        // +0x144
    unsigned long  m_counter148;                        // +0x148
    unsigned long  m_counter14c;                        // +0x14c
    unsigned long  m_owner;                             // +0x150 (set by the callers)
    long           m_currentEntry;                      // +0x154 (-1 = none)
    unsigned long  m_counter158;                        // +0x158
    unsigned long  m_counter15c;                        // +0x15c
    unsigned long  m_counter160;                        // +0x160
    unsigned long  m_counter164;                        // +0x164
    void Reset();
};

void SBufferedStreamState00c8d970::Reset()
{
    m_readPosition = 0;
    m_writePosition = 0;
    m_entryCount = 0;
    m_counter20 = 0;
    m_counter24 = 0;
    m_counter144 = 0;
    m_counter148 = 0;
    m_counter14c = 0;
    m_currentEntry = -1;
    m_counter158 = 0;
    m_counter15c = 0;
    m_counter160 = 0;
    m_counter164 = 0;
}

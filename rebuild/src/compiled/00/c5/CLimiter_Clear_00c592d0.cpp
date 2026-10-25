// CLimiter::Clear() -- reset the GameCODA output limiter: zero its 88-sample delay line, rewind the write index and return the state machine to pass-through (the peak, envelope and counters are left as they are)
// Leaf: ?Clear@CLimiter@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C592D0, 29 bytes (+ int3 padding), thiscall, plain `ret`.
// Retail bytes:  mov edx,ecx ; push edi ; xor eax,eax ; mov ecx,58h ; lea edi,[edx+4] ; rep stosd ;
//   mov [edx+164h],eax ; mov [edx+174h],eax ; pop edi ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC592D0 "CChannelState::Clear" = CLimiter::Clear"):
//   - the body is the landed CLimiter::CLimiter 0x00C59570 without the vtable store (delay line +4, 0x58 words; write
//     index +0x164; state +0x174);
//   - its caller 0x00C3F000 (slot 1 of the limited mix buffer vtables 0x012B5BE0 / 0x012B5D40 / 0x012B5DE0) runs it over the
//     six 0x17C-byte limiters at +0xC34 that the constructor 0x00C3EFA0 builds with CLimiter::CLimiter.
//   The CChannelState_00c592d0 name is a placeholder; descriptive names (GameCODA: no PDB/RTTI).
#include <string.h>

class CLimiter {                                        // vtable 0x012B8F64, 0x17C bytes
public:
    virtual ~CLimiter();
    long          m_delay[88];                          // +0x004 delay line of mixed samples
    unsigned long m_writeIndex;                         // +0x164
    float         m_peak;                               // +0x168
    float         m_envelope;                           // +0x16c
    long          m_holdCount;                          // +0x170
    long          m_state;                              // +0x174 0 pass-through, 1 attack, 2 release
    long          m_releaseCount;                       // +0x178
    void Clear();
};

void CLimiter::Clear()
{
    memset(m_delay, 0, sizeof(m_delay));
    m_writeIndex = 0;
    m_state = 0;
}

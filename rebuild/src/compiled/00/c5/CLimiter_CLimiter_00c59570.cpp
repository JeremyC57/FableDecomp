// CLimiter::CLimiter() -- constructor of the GameCODA output limiter: vtable, then the 88-sample delay line, its write index and the state machine cleared
// Leaf: ??0CLimiter@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59570, length 37 bytes (+ int3 padding); `ret`, returns `this`.
// Callees: none (the clear is the intrinsic `rep stosd`, 0x58 words). Referenced as the element constructor of the
// six-limiter array in 0x00C3EFA0 (`__ehvec_ctor`, element size 0x17C, destructor 0x00C592C0).
// Label: the manifest's "Ctor_ZeroInit_012b8f64" describes the bytes; the class name is descriptive (GameCODA has no
// PDB/RTTI). The landed Clear 0x00C592D0 (`CChannelState_00c592d0`) is the same three statements without the vtable
// store, i.e. the same class.
// Shape: `this` moves to edx so ecx can be the `rep stosd` count; the zero register then clears the write index and
// the state. The vtable store comes first (no base); the members are assigned in the body (not an init list).
#include <string.h>

// The GameCODA output limiter (vtable 0x012B8F64, no RTTI; 0x17C bytes; six of them at +0xC34 of the output device
// built by 0x00C3EFA0 through `__ehvec_ctor` with this constructor and the destructor 0x00C592C0). Its process loop
// 0x00C593C0 writes each 32-bit mixed sample into an 88-entry delay line, reads the sample 44 entries back, and runs a
// three-state machine (0 pass-through, 1 attack, 2 release) that scales the delayed sample by 32767 / envelope.
class CLimiter {
public:
    virtual ~CLimiter();                // 0x00C592C0 (body) / 0x00C595A0 (scalar deleting)
    long          m_delay[88];          // +0x004 delay line of mixed samples
    unsigned long m_writeIndex;         // +0x164
    float         m_peak;               // +0x168 held peak magnitude of the delayed samples
    float         m_envelope;           // +0x16c gain envelope (attack towards the peak, slow release)
    long          m_holdCount;          // +0x170 samples the peak has been held (re-armed at 44)
    long          m_state;              // +0x174 0 pass-through, 1 attack, 2 release
    long          m_releaseCount;       // +0x178
    CLimiter();
    void UpdateEnvelope();
};

CLimiter::CLimiter()
{
    memset(m_delay, 0, sizeof(m_delay));
    m_writeIndex = 0;
    m_state = 0;
}
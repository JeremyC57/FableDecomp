// CLimitedMixBuffer::CLimitedMixBuffer() -- EH-framed constructor of the GameCODA mix accumulator with a per-channel output limiter: the CMixBuffer base (0x00C591F0, EH state 0), its vtable 0x012B5BE0, and the six 0x17C-byte CLimiters at +0xC34 built through `__ehvec_ctor` (CLimiter::CLimiter 0x00C59570, CLimiter::~CLimiter 0x00C592C0)
// Leaf: ??0CLimitedMixBuffer@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3EFA0, 95 bytes, thiscall, returns `this`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC3EFA0 "CParticleSet" = GameCODA output device (six CLimiters at
//   +0xC34)"):
//   - the base constructor 0x00C591F0 is the landed CMixBuffer::CMixBuffer (vtable 0x012B8F38: six planar accumulators,
//     channel count +0x20, the 0xC10-byte aligned store ending at +0xC34);
//   - the vtable 0x012B5BE0 keeps CMixBuffer's kernels (slots 2..9 identical to 0x012B8F38) and overrides slot 0 (deleting
//     destructor 0x00C3F090 around the destructor 0x00C3F030), slot 1 (0x00C3F000: CLimiter::Clear 0x00C592D0 on each of
//     the six limiters) and slot 10 (0x00C591C0: channel c's output goes through m_limiters[c] (0x00C593C0, the
//     limiter process loop) instead of CMixBuffer's 0x00C590E0);
//   - the six elements are CLimiters (vtable 0x012B8F64, 0x17C bytes); 0x00C3FA44 / 0x00C3FA70 construct this class and
//     then store the derived vtables 0x012B5DE0 / 0x012B5D40 (their destructors 0x00C3F7F0 / 0x00C3F680 tail-jump to
//     0x00C3F030), the product stored at +0x58 of its owner.
//   The CParticleSet name is a propagated manifest label; CLimitedMixBuffer is descriptive (GameCODA: no PDB/RTTI) --
//   the "output device" of the finding is the owner of the +0x58 mixer, not this class.
// Shape: CMixBuffer is a CPKObject (its deleting destructor 0x00C59250 frees through CPKObject::operator delete
//   0x00C3C370); the destructor of this class is compiler-generated (0x00C3F030, no vptr store).
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};

class CMixBuffer : public CPKObject {                   // vtable 0x012B8F38, 0xC34 bytes
public:
    CMixBuffer();                                       // 0x00C591F0
    virtual ~CMixBuffer();                              // 0x00C57EF0
    long*         m_pChannel[6];                        // +0x04 planar 128-sample accumulators
    long          m_active;                             // +0x1c
    unsigned long m_numChannels;                        // +0x20
    unsigned char m_storage[0xc10];                     // +0x24 accumulator store, 16-byte aligned by hand
};

class CLimiter {                                        // vtable 0x012B8F64, 0x17C bytes
public:
    CLimiter();                                         // 0x00C59570
    virtual ~CLimiter();                                // 0x00C592C0
    long          m_delay[88];                          // +0x004
    unsigned long m_writeIndex;                         // +0x164
    float         m_peak;                               // +0x168
    float         m_envelope;                           // +0x16c
    long          m_holdCount;                          // +0x170
    long          m_state;                              // +0x174
    long          m_releaseCount;                       // +0x178
};

class CLimitedMixBuffer : public CMixBuffer {           // vtable 0x012B5BE0
public:
    CLimitedMixBuffer();
    CLimiter m_limiters[6];                             // +0xc34
};

CLimitedMixBuffer::CLimitedMixBuffer()
{
}

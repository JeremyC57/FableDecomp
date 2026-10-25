// CLimitedMixBuffer::~CLimitedMixBuffer() -- the compiler-generated destructor of the GameCODA limited mix buffer: the six CLimiters at +0xC34 destroyed through `__ehvec_dtor` (CLimiter::~CLimiter 0x00C592C0, EH state 0), then the CMixBuffer base destructor (0x00C57EF0) out of line
// Leaf: ??1CLimitedMixBuffer@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F030, 92 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 441 "0xC3EFA0 = GameCODA output device (six CLimiters at +0xC34)"):
//   - the scalar deleting destructor 0x00C3F090 (slot 0 of the CLimitedMixBuffer vtable 0x012B5BE0) calls this and then
//     CPKObject::operator delete 0x00C3C370; the destructors of the two derived mixers (vtables 0x012B5D40 / 0x012B5DE0,
//     0x00C3F680 / 0x00C3F7F0) store their vtable and tail-jump here;
//   - it tears down what the constructor 0x00C3EFA0 builds: the six 0x17C-byte CLimiters at +0xC34 (`__ehvec_dtor` with
//     their destructor 0x00C592C0) and the CMixBuffer base (0x00C57EF0, the base destructor 0x00C59250 also calls).
//   The landed CParticleSet_00c3f030 is a placeholder over a `novtable` class; descriptive names (GameCODA: no PDB/RTTI).
// Shape: no vtable store because the destructor is IMPLICIT (a compiler-generated destructor does not reset the vptr),
//   not because the class is `novtable` -- its constructor 0x00C3EFA0 does store 0x012B5BE0. The factory below emits the
//   vtable and with it the `??_G` that references this COMDAT. CMixBuffer's own destructor is user-declared out of line:
//   0x00C57EF0 is `CMixBuffer::~CMixBuffer() { }`, whose CMixBuffer vptr store VC7.1 drops as dead before the inlined
//   CPKObject store of 0x012B5868.
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

class CLimitedMixBuffer : public CMixBuffer {           // vtable 0x012B5BE0; destructor compiler-generated
public:
    CLimitedMixBuffer() { }                             // out of line at 0x00C3EFA0
    CLimiter m_limiters[6];                             // +0xc34
};

CLimitedMixBuffer* Create_CLimitedMixBuffer()
{
    return new CLimitedMixBuffer;
}

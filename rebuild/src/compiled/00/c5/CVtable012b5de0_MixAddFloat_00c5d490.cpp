// CVtable_012b5de0::MixAddFloat(const float* src, long* dst) -- vtable slot 8 of the SSE2 limited mix buffer: add the 128-sample float block into the long accumulator through the SSE2 kernel 0x00C5D2E0 (cvtps2dq under a round-to-nearest MXCSR)
// Leaf: ?MixAddFloat@CVtable_012b5de0@@UAEXPBMPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D490, 21 bytes (+ int3 padding), thiscall `ret 8` (`this` unused).
// Callee: 0x00C5D2E0 (__cdecl, `add esp,8`; a hand SSE2 loop between stmxcsr/ldmxcsr rounding-mode switches, not landed;
//   descriptive name AddFloat128BlockSSE2).
// Identity: slot 8 of the vtable 0x012B5DE0, a CLimitedMixBuffer-derived table whose kernel slots point at the SSE2
//   copies (slot 5 the landed AddInt128Block_00c5d4b0); the landed CLimitedMixBuffer ctor 0x00C3EFA0 notes that
//   0x00C3FA44 constructs a CLimitedMixBuffer and stores 0x012B5DE0. CMixBuffer's own slot 8 is MixAddFloat 0x00C57F20.
//   No PDB/RTTI: the class is named by its vtable (its ctor 0x00C3F7B0 carries the propagated "C3DAnimationUtil").
// Shape: the two arguments re-pushed (right to left), `add esp,8`, `ret 8`.
void __cdecl AddFloat128BlockSSE2(const float* src, long* dst);   // 0x00C5D2E0

class CMixBuffer {                                  // vtable 0x012B8F38 (the landed CMixBuffer model)
public:
    virtual ~CMixBuffer();                                                            // slot 0
    virtual void Reset();                                                             // slot 1
    virtual void Clear();                                                             // slot 2
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);         // slot 3
    virtual void MixAdd32Gain(const long* src, long* dst, unsigned short gain);      // slot 4
    virtual void MixAdd32(const long* src, long* dst);                                // slot 5
    virtual void MixAddFloatGain(const float* src, long* dst, float gain);           // slot 6
    virtual void MixAddFloatGain16(const float* src, long* dst, unsigned short gain); // slot 7
    virtual void MixAddFloat(const float* src, long* dst);                            // slot 8 (0x00C57F20)
    long*         m_pChannel[6];        // +0x04 planar 128-sample accumulators
    long          m_active;             // +0x1c
    unsigned long m_numChannels;        // +0x20
    unsigned char m_storage[0xc10];     // +0x24
};
class CLimitedMixBuffer : public CMixBuffer {      // vtable 0x012B5BE0 (ctor 0x00C3EFA0)
public:
    unsigned char m_limiters[6 * 0x17c];            // +0xc34 six CLimiters
};
class CVtable_012b5de0 : public CLimitedMixBuffer { // vtable 0x012B5DE0
public:
    virtual void MixAddFloat(const float* src, long* dst);                            // slot 8 (0x00C5D490)
};

void CVtable_012b5de0::MixAddFloat(const float* src, long* dst)
{
    AddFloat128BlockSSE2(src, dst);
}
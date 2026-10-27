// CMixBuffer::MixAddFloatGain(const float* src, long* dst, float gain) -- vtable slot 6: add a 128-sample float block scaled by a float gain into a long accumulator, each product rounded to nearest by libpk's inline-asm float-to-long helper
// Leaf: ?MixAddFloatGain@CMixBuffer@@UAEXPBMPAJM@Z
// Retail-asm: 0x00C57FD1 `fld dword ptr [esp+14h] ; fmul dword ptr [ecx+eax] ; fstp dword ptr [esp+10h] ; fld dword ptr [esp+10h] ; fistp dword ptr [esp+0Ch]` -- libpk's inline-assembly float-to-long helper inlined per sample (the product homed in a dead argument slot and reloaded for a bare dword fistp); VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1), so the fld/fistp pair is the original's `__asm` (same helper as landed 0x00C6B240 FtoL).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57FC0, 57 bytes (+ int3 padding), thiscall `ret 0Ch`; slot 6 of the
//   CMixBuffer vtable 0x012B8F38 (also of the derived vtables 0x012B5BE0 / 0x012B5D40 / 0x012B5DE0).
// Name: the landed CMixBuffer model (0x00C58370 / 0x00C591F0) already names this slot MixAddFloatGain (descriptive).
// Shape: `fld [gain] ; fmul [src]` is `src[i] * gain`; 128 down-counter in edx.
// The GameCODA software mixer's block accumulator (vtable 0x012B8F38, no RTTI; ctor 0x00C591F0): the landed CMixBuffer
// model (0x00C58370 / 0x00C591F0), six planar 128-sample `long` accumulators, the channel count at +0x20.
class CMixBuffer {
public:
    virtual ~CMixBuffer();                                                            // slot 0 (0x00C59250)
    virtual void Reset();                                                             // slot 1 (0x00C59240)
    virtual void Clear();                                                             // slot 2 (0x00C57F00)
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);         // slot 3 (0x00C580C0)
    virtual void MixAdd32Gain(const long* src, long* dst, unsigned short gain);      // slot 4 (0x00C58040)
    virtual void MixAdd32(const long* src, long* dst);                                // slot 5 (0x00C58000)
    virtual void MixAddFloatGain(const float* src, long* dst, float gain);           // slot 6 (0x00C57FC0)
    virtual void MixAddFloatGain16(const float* src, long* dst, unsigned short gain); // slot 7 (0x00C57F60)
    virtual void MixAddFloat(const float* src, long* dst);                            // slot 8 (0x00C57F20)
    virtual void MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step);   // slot 9 (0x00C58140)
    long*         m_pChannel[6];        // +0x04 planar 128-sample accumulators
    long          m_active;             // +0x1c (1 after construction)
    unsigned long m_numChannels;        // +0x20
    unsigned char m_storage[0xc10];     // +0x24 accumulator store, 16-byte aligned by hand
};
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

void CMixBuffer::MixAddFloatGain(const float* src, long* dst, float gain)
{
    for (int i = 0; i < 128; ++i)
        dst[i] += FloatToLong(src[i] * gain);
}
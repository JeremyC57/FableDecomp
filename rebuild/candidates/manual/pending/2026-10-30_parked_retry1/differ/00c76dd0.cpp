// CReverbPreDelay::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index) -- two-pass setup of the reverb's pre-delay line: half a second of samples rounded up to a multiple of four; with memory, take that many floats from the cursor, clear them, reset the read offset and record the line index; always add the line's bytes to the running total
// Grade: IMPROVED -- retry1: DIFFER(182v186) with the landed libpk FloatToLong / FloatToLongTruncate helpers (bare `fistp dword`, retail_asm): the truncation block matches; left is callee-saved colouring around the inline memset (retail keeps pMemory in ebx from before the if and length*4 in ebp; ours reloads pMemory and keeps length*4 in ebx).
// Leaf: ?Allocate@CReverbPreDelay
// Retail-asm: 0x00C76DFB `fistp  DWORD PTR [esp+0x8]` -- libpk's inline-assembly truncation helper (the landed FloatToLong / FloatToLongTruncate of
//   CVoiceEffect::Configure 0x00C6AEF0: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`); VC7.1 C++ `(long)x`
//   calls _ftol2 and /QIfist emits `fistp qword`, so no C/C++ form gives the bare dword fistp.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76DD0, length 186 bytes, ret 10h. No callees (memset intrinsic). The
//   length is the libpk float-to-int truncation of `rate * 0.5f` (0.5f at 0x0122F59C: fild + 2^32 fix-up, fmul,
//   `fsub 0.5f ; fstp ; fld ; fistp dword`, then `if (i < 0) ++i`), a bare fistp that VC7.1 cannot emit from C++.
// Layout: the reverb's +0x1E0 member (vtable 0x012B9580): ring data / size / position +4 / +8 / +0xc, a read offset
//   +0x10 (cleared here), line index +0x14. Names descriptive (libpk, no PDB/RTTI).
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

// libpk's inline-assembly float-to-long helper (the landed CVoiceEffect::Configure 0x00C6AEF0 spelling). Retail homes the value in a
// dead stack slot and rounds it with a bare `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default); VC7.1 expands
// the __forceinline helper in place.
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

struct CReverbPreDelay : CDspObject {      // vtable 0x012B9580
    CRingBuffer m_ring;                    // +0x04
    unsigned int m_readOffset;             // +0x10
    unsigned int m_index;                  // +0x14
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index);
};

void CReverbPreDelay::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    unsigned int length = (FloatToLongTruncate(rate * 0.5f) + 3) & ~3u;
    if (*pMemory) {
        m_ring.m_data = *pMemory;
        m_ring.m_size = length;
        m_ring.m_pos = 0;
        memset(m_ring.m_data, 0, length * sizeof(float));
        m_index = index;
        m_readOffset = 0;
        *pMemory += length;
    }
    *pBytes += length * sizeof(float);
}

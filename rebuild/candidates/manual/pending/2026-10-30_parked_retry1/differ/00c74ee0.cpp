// IsPrime(unsigned int n) -- trial-division primality test used to size the reverb's delay lines: 0..3 count as prime, even numbers do not, otherwise try the odd divisors up to the rounded square root
// Grade: DIFFER(96v95) (retry1, was 101v95) -- IsPrime as a same-TU `static __declspec(noinline)` beside its caller CStereoDelayLine::Allocate takes n in EBX like retail; the bare `fistp dword` bound is a retail_asm RoundToLong (fld/fistp), the float sqrt / 0.5f / unsigned-fild fix-up and the ecx=3 placement now agree. Left: block placement only -- retail lays the shared `mov al,1` return block straight after `cmp ebx,3 ; ja` (the loop exit jumps back to it), VC7.1 here puts it at the end (1 nop of loop alignment).
// Leaf: ?IsPrime@@YA_NI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74EE0, length 95 bytes, plain `ret`; `n` arrives in ebx (an LTCG
//   static helper: the only callers are CStereoDelayLine::Allocate 0x00C75050's two next-prime loops, which keep the
//   candidate in ebx across the calls). The loop bound is re-rounded each pass by an inline-asm float-to-int
//   (`fld [root] ; fistp dword [tmp]` then `if (i < 0) ++i`) of the hoisted `sqrt(n) - 0.5f` (0.5f at 0x0122F59C, the
//   unsigned fild fix-up 2^32 at 0x0122DCB4), which VC7.1 cannot emit from C++ (a C cast calls _ftol).
// Reference body (descriptive name, no PDB for libpk): the C++ shape of the retail control flow.
// CStereoDelayLine::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes) -- two-pass setup of the stereo pair of delay lines: when sizing, make each line the first prime at or above its scaled length (1051 and 337 samples at 48 kHz, made odd); with memory, hand each line its block from the cursor and clear it; always add both lines' bytes to the running total
// Allocate OldGrade: DIFFER(237v232) -- LTCG register argument: retail passes the IsPrime candidate in ebx (no push, no stack cleanup) and keeps it there across the next-prime loops; ours pushes it
// Leaf: ?Allocate@CStereoDelayLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75050, length 232 bytes, ret 0Ch. Callee: IsPrime 0x00C74EE0, an LTCG
//   static helper that takes its argument in ebx (the candidate stays in ebx across the next-prime loops: `or ebx,1 ;
//   call ; test al,al ; jne ; add ebx,2 ; call ...`), which a VC7.1 non-LTCG build cannot reproduce. The divisions by
//   48000 are the magic multiply 0x057619F1 / shr 10.
// Layout: as CStereoDelayLine::Clear 0x00C75010 (two 0x10-byte CDelayLine at +4: vptr, data, size, position).
//   Names descriptive (libpk, no PDB/RTTI).
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)
// Retail-asm: 0x00C74F18 fistp dword ptr [esp+4] (bare float-to-int of the loop bound; a C cast calls _ftol)
inline long RoundToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result < 0 ? result + 1 : result;
}
static __declspec(noinline) bool IsPrime(unsigned int n)   // 0x00C74EE0
{
    if (n <= 3)
        return true;
    if ((n & 1) == 0)
        return false;
    unsigned int divisor = 3;
    float root = (float)sqrt((float)n) - 0.5f;
    while (divisor < (unsigned int)(RoundToLong(root) + 1)) {
        if (n % divisor == 0)
            return false;
        divisor += 2;
    }
    return true;
}

inline unsigned int NextPrime(unsigned int n)
{
    unsigned int candidate = n | 1;
    while (!IsPrime(candidate))
        candidate += 2;
    return candidate;
}

struct CDelayLine {                        // vtable 0x012BF074, 0x10 bytes
    virtual ~CDelayLine();
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08 (floats)
    unsigned int m_pos;                    // +0x0C
    void Clear()
    {
        if (m_data)
            memset(m_data, 0, m_size * sizeof(float));
        m_pos = 0;
    }
};

struct CStereoDelayLine {                  // vtable 0x012BF078
    virtual ~CStereoDelayLine();           // 0x00C75440
    CDelayLine m_lines[2];                 // +0x04
    void Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes);
};

void CStereoDelayLine::Allocate(unsigned int rate, float** pMemory, unsigned int* pBytes)
{
    if (!*pMemory) {
        m_lines[0].m_size = NextPrime(rate * 1051 / 48000);
        m_lines[1].m_size = NextPrime(rate * 337 / 48000);
    } else {
        for (int i = 0; i < 2; ++i) {
            m_lines[i].m_data = *pMemory;
            m_lines[i].Clear();
            *pMemory += m_lines[i].m_size;
        }
    }
    *pBytes += (m_lines[0].m_size + m_lines[1].m_size) * sizeof(float);
}

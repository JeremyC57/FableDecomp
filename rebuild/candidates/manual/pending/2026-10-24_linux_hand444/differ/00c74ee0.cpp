// IsPrime(unsigned int n) -- trial-division primality test used to size the reverb's delay lines: 0..3 count as prime, even numbers do not, otherwise try the odd divisors up to the rounded square root
// Grade: DIFFER(101v95) -- LTCG register argument (n in ebx, no stack argument) and retail_asm candidate (bare `fistp dword` float-to-int in the loop bound); ours takes n on the stack and calls _ftol
// Leaf: ?IsPrime@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74EE0, length 95 bytes, plain `ret`; `n` arrives in ebx (an LTCG
//   static helper: the only callers are CStereoDelayLine::Allocate 0x00C75050's two next-prime loops, which keep the
//   candidate in ebx across the calls). The loop bound is re-rounded each pass by an inline-asm float-to-int
//   (`fld [root] ; fistp dword [tmp]` then `if (i < 0) ++i`) of the hoisted `sqrt(n) - 0.5f` (0.5f at 0x0122F59C, the
//   unsigned fild fix-up 2^32 at 0x0122DCB4), which VC7.1 cannot emit from C++ (a C cast calls _ftol).
// Reference body (descriptive name, no PDB for libpk): the C++ shape of the retail control flow.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

static long RoundToLong(float value)            // retail: inline asm `fld value ; fistp dword result` + the < 0 fix-up
{
    long result = (long)value;
    if (result < 0)
        ++result;
    return result;
}

bool IsPrime(unsigned int n)
{
    if (n <= 3)
        return true;
    if ((n & 1) == 0)
        return false;
    float root = (float)(sqrt((double)n) - 0.5f);
    for (unsigned int divisor = 3; divisor < (unsigned int)(RoundToLong(root) + 1); divisor += 2)
        if (n % divisor == 0)
            return false;
    return true;
}

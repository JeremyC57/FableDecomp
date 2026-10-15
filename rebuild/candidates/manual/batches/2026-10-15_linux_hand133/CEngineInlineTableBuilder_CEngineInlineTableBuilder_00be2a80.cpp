// CEngineInlineTableBuilder::CEngineInlineTableBuilder() -- static-table builder constructor: fill the 256-entry byte table @ 0x0143B990 with floor(log2(i)) (entry 0 = -1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE2A80, length 150 bytes (+ int3 padding).
// Callees: none (the integer log2 helper -- out of line at 0x0099FDB0 -- is inlined). Global: the byte table @ 0x0143B990.
// Shape: `mov eax,ecx` = a constructor returning `this` that never touches its own (empty) object; the loop over a signed index (`jl`)
// is bottom-tested with the helper's zero and top-bit tests inside it; the helper's result lives in ebx and is stored as a byte. The
// helper's `++value` lands as `lea esi,[edi+1]` here because the loop index stays live. The helper must be `__forceinline` (plain
// `inline` leaves it out of line).
extern signed char g_highestSetBitTable[256];   // 0x0143B990

__forceinline long HighestSetBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}

struct CEngineInlineTableBuilder {
    CEngineInlineTableBuilder();
};

CEngineInlineTableBuilder::CEngineInlineTableBuilder()
{
    for (long i = 0; i < 256; ++i)
        g_highestSetBitTable[i] = (signed char)HighestSetBit(i);
}

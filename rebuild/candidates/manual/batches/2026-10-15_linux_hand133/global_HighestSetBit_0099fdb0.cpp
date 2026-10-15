// _global::HighestSetBit(unsigned long value) -- floor(log2(value)): -1 for 0, 31 for values with the top bit set, else a branch-free five-step binary search over (value + 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099FDB0, length 123 bytes.
// Callees: none. Label correction: the manifest's CEngine::GetScratchZBufferD24S8 is a propagated name -- the body takes its only input in
// ecx and never touches memory, so it is a `__fastcall` integer helper (the same search is inlined into the 256-entry byte table builder at
// 0x00BE2A80, "CEngineInlineTableBuilder").
// Shape: `test esi,esi ; jne` + `or eax,-1` (value 0), `cmp esi,80000000h ; jb` + `mov eax,1Fh`, then `inc esi` (the parameter itself incremented: a separate `n = value + 1` gives `lea` and a late esi save) and four steps of
// `((1 << (r + k)) - n) >> 31 & k` added into edx (the first step's `0x10000 - n` is the folded `1 << 16`); the last step adds into eax.
// Leaf: HighestSetBit
long __fastcall HighestSetBit(unsigned long value)
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

// FindFirstSetBit(unsigned long mask, unsigned long numBytes) -- index of the lowest set bit among the first numBytes*8 bits of mask, -1 when none
// Leaf: ?FindFirstSetBit@@
// Retail: Fable.exe @ 0x00A3F860, 40 bytes. Descriptive name (no PDB name found). __fastcall: both callers (0x00AABE0B,
//   0x00AAC03B, a channel-mask walk) load the mask into ecx and the byte width 0x10 into edx, then use the result as a
//   shift count.
// Shape: the bit count is `numBytes << 3` (`shl edx,3`); unsigned index and bound (`jbe` entry test, `jb` back edge);
//   each probe rebuilds `1 << i` (`mov edi,1 ; mov ecx,eax ; shl edi,cl ; test esi,edi`); the miss falls into `or eax,-1`.
int __fastcall FindFirstSetBit(unsigned long mask, unsigned long numBytes)
{
    for (unsigned long i = 0; i < numBytes * 8; ++i)
        if (mask & (1 << i))
            return (int)i;
    return -1;
}
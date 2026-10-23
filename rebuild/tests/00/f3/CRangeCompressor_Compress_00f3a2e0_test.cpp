#include <cstdio>
#include <cstring>

// Model of CRangeCompressor::Compress: per column entry write flags, bit width, then (by flag) the bias, gap start,
// set mask and kept mask in the block size; then each of the column's values, transformed by the entry (bias
// subtract for 0x10, set-mask clear for 8, gap shift for 2 or kept-run shuffle for 4, bias subtract for 1), is
// bit-packed little-endian into dwords (32-bit widths stored directly); 0x80 ends the stream.
static const unsigned long g_masks[5] = { 0, 0xff, 0xffff, 0, 0xffffffffu };
struct CScriptEntry { unsigned long Flags; long BitsNeeded; unsigned long Bias; long GapStart; unsigned long KeptMask; unsigned long SetMask; };

static long GetBlockSize(unsigned long flags) { if (flags & 0x40) return 4; return (flags & 0x20) ? 2 : 1; }
static void WriteValue(unsigned char* d, unsigned long v, long n) { for (long k = 0; k < n; ++k) d[k] = (unsigned char)(v >> (8 * k)); }
static unsigned long ReadValue(const unsigned char* s, long n) { unsigned long v = 0; for (long k = 0; k < n; ++k) v |= (unsigned long)s[k] << (8 * k); return v; }
static void Put32(unsigned char* d, unsigned long v) { WriteValue(d, v, 4); }
static long CalcShuffleOperations(unsigned long mask, unsigned long* runMasks, unsigned long* shifts)
{
    long count = 0, removed = 0;
    for (long bit = 0; bit < 32; ++bit) {
        if (mask & (1ul << bit)) {
            unsigned long run = 0; long end;
            for (end = bit; end < 32 && (mask & (1ul << end)); ++end) run |= 1ul << end;
            runMasks[count] = run; shifts[count] = (unsigned long)(bit - removed); ++count;
            removed += end - bit; bit = end;
        }
    }
    return count;
}
static long Compress(const unsigned char* source, long count, long stride, unsigned char* dest, const CScriptEntry* entries, unsigned long n)
{
    long pos = 0;
    unsigned long runMasks[16], shifts[16];
    for (unsigned long i = 0; i < n; ++i) {
        const CScriptEntry& entry = entries[i];
        dest[pos] = (unsigned char)entry.Flags; dest[pos + 1] = (unsigned char)entry.BitsNeeded; pos += 2;
        long runs = 0;
        long blockSize = GetBlockSize(entry.Flags);
        if (entry.Flags & 0x11) { WriteValue(dest + pos, entry.Bias, blockSize); pos += blockSize; }
        if (entry.Flags & 2) { dest[pos] = (unsigned char)entry.GapStart; ++pos; }
        if (entry.Flags & 8) { WriteValue(dest + pos, entry.SetMask, blockSize); pos += blockSize; }
        if (entry.Flags & 4) { WriteValue(dest + pos, entry.KeptMask, blockSize); runs = CalcShuffleOperations(entry.KeptMask, runMasks, shifts); pos += blockSize; }
        long bits = entry.BitsNeeded;
        if (bits > 0) {
            unsigned long acc = 0; long accBits = 0;
            for (long j = 0; j < count; ++j) {
                unsigned long value = ReadValue(source + j * stride, blockSize);
                unsigned long flags = entry.Flags;
                if (flags & 0x10) value = (value - entry.Bias) & g_masks[blockSize];
                if (flags & 8) value &= ~entry.SetMask;
                if (flags & 2) value >>= entry.GapStart;
                else if (flags & 4) {
                    unsigned long packed = 0;
                    for (long r = 0; r < runs; ++r) packed |= (runMasks[r] & value) >> shifts[r];
                    value = packed;
                }
                if (entry.Flags & 1) value = (value - entry.Bias) & g_masks[blockSize];
                if (bits == 32) { Put32(dest + pos, value); pos += 4; }
                else {
                    acc |= value << accBits;
                    accBits += bits;
                    if (accBits >= 32) { Put32(dest + pos, acc); acc = value >> (bits - accBits + 32); pos += 4; accBits -= 32; }
                }
            }
            if (accBits > 0) { Put32(dest + pos, acc); pos += 4; }
        }
        source += blockSize;
    }
    dest[pos] = 0x80;
    return pos + 1;
}

static bool Check(const unsigned char* got, long n, const unsigned char* want, long m) { return n == m && std::memcmp(got, want, (unsigned int)m) == 0; }

int main()
{
    unsigned char out[128];
    // 1: one byte column, 4 bits: 1,2,3 -> 0x321
    { unsigned char src[3] = { 1, 2, 3 }; CScriptEntry e = { 0, 4, 0, 0, 0, 0 };
      unsigned char want[] = { 0, 4, 0x21, 0x03, 0, 0, 0x80 };
      if (!Check(out, Compress(src, 3, 1, out, &e, 1), want, sizeof(want))) { std::printf("00f3a2e0_TEST FAIL\n"); return 1; } }
    // 2: dword column with a range bias, 32-bit values stored directly
    { unsigned char src[8]; Put32(src, 150); Put32(src + 4, 300); CScriptEntry e = { 0x50, 32, 100, 0, 0, 0 };
      unsigned char want[] = { 0x50, 32, 100, 0, 0, 0, 50, 0, 0, 0, 200, 0, 0, 0, 0x80 };
      if (!Check(out, Compress(src, 2, 4, out, &e, 1), want, sizeof(want))) { std::printf("00f3a2e0_TEST FAIL\n"); return 1; } }
    // 3: two interleaved byte columns: gap strip (set mask 0x80, gap at bit 2) and kept-run shuffle (0b1010)
    { unsigned char src[4] = { 0x80 | 20, 0x0a, 0x80 | 28, 0x08 };
      CScriptEntry e[2] = { { 0x0a, 3, 0, 2, 0, 0x80 }, { 0x04, 2, 0, 0, 0x0a, 0 } };
      unsigned char want[] = { 0x0a, 3, 2, 0x80, 0x05 | (0x07 << 3), 0, 0, 0, 0x04, 2, 0x0a, 0x03 | (0x02 << 2), 0, 0, 0, 0x80 };
      if (!Check(out, Compress(src, 2, 2, out, e, 2), want, sizeof(want))) { std::printf("00f3a2e0_TEST FAIL\n"); return 1; } }
    // 4: 20-bit word values spill across dwords; flag 1 subtracts the bias after the strip
    { unsigned char src[4] = { 0x10, 0x00, 0x20, 0x00 }; CScriptEntry e = { 0x21, 20, 0x8, 0, 0, 0 };
      long n = Compress(src, 2, 2, out, &e, 1);
      unsigned long a = 0x8 | (0x18ul << 20), b = 0x18ul >> 12;
      unsigned char want[16] = { 0x21, 20, 0x08, 0x00 }; Put32(want + 4, a); Put32(want + 8, b); want[12] = 0x80;
      if (!Check(out, n, want, 13)) { std::printf("00f3a2e0_TEST FAIL\n"); return 1; } }
    // 5: no entries -> just the end marker
    if (Compress(0, 0, 0, out, 0, 0) != 1 || out[0] != 0x80) { std::printf("00f3a2e0_TEST FAIL\n"); return 1; }
    std::printf("00f3a2e0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>

// Model of CRangeCompressor::Decompress, checked as the inverse of the sibling Compress 0x00F3A2E0: a leading 0 byte
// means raw data (copied); otherwise each column header is read back and its bit-packed values are unpacked, the
// kept-run shuffle undone, the set mask restored and the bias added, until the 0x80 end marker. The clock is read at
// entry and exit and the produced byte count is accumulated.
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


static double g_clock = 0.0;
static double GetHighResTime() { g_clock += 1.0; return g_clock; }
static double g_RangeDecompressTime = 0.0;
static unsigned long g_RangeDecompressBytes = 0;
static bool IsValidBitCount(long long bits) { return bits >= 0 && bits <= 0x7fffffff; }

static void Decompress(const unsigned char* data, long count, long stride, void* dest)
{
    g_RangeDecompressTime -= GetHighResTime();
    g_RangeDecompressBytes += (unsigned long)(count * stride);
    if (*data == 0) {
        std::memcpy(dest, data + 1, (unsigned int)(count * stride));
        g_RangeDecompressTime += GetHighResTime();
        return;
    }
    ++data;
    unsigned char* column = (unsigned char*)dest;
    unsigned char flags = *data++;
    while (!(flags & 0x80)) {
        long blockSize = GetBlockSize(flags);
        if (!IsValidBitCount(blockSize * 8)) return;
        long bits = *data++;
        unsigned long bias = 0, setMask = 0; long gap = 0, runs = 0;
        unsigned long runMasks[16], shifts[16];
        if (flags & 0x11) { bias = ReadValue(data, blockSize); data += blockSize; }
        if (flags & 2) gap = *data++;
        if (flags & 8) { setMask = ReadValue(data, blockSize); data += blockSize; }
        if (flags & 4) { runs = CalcShuffleOperations(ReadValue(data, blockSize), runMasks, shifts); data += blockSize; }
        unsigned long mask = bits >= 32 ? 0xffffffffu : (1ul << bits) - 1;
        long used = 32;
        if (bits == 0) used = 0;
        unsigned long acc = 0;
        for (long j = 0; j < count; ++j) {
            unsigned long value;
            if (bits == 32) { value = ReadValue(data, 4); data += 4; }
            else {
                if (used == 32) { acc = ReadValue(data, 4); data += 4; used = 0; }
                value = (acc >> used) & mask;
                used += bits;
                if (used > 32) { acc = ReadValue(data, 4); value |= (acc << (bits - used + 32)) & mask; data += 4; used -= 32; }
            }
            if (flags & 1) value = (value + bias) & g_masks[blockSize];
            value <<= gap;
            if (flags & 4) {
                unsigned long unpacked = 0;
                for (long r = 0; r < runs; ++r) unpacked |= (value << shifts[r]) & runMasks[r];
                value = unpacked;
            }
            value |= setMask;
            if (flags & 0x10) value = (value + bias) & g_masks[blockSize];
            WriteValue(column + j * stride, value, blockSize);
        }
        column += blockSize;
        flags = *data++;
    }
    g_RangeDecompressTime += GetHighResTime();
}

static bool RoundTrip(const unsigned char* src, long count, long stride, const CScriptEntry* e, unsigned long n)
{
    unsigned char packed[256], back[256];
    packed[0] = 1;   // the leading byte: non-zero = range-compressed
    Compress(src, count, stride, packed + 1, e, n);
    std::memset(back, 0xcc, sizeof(back));
    Decompress(packed, count, stride, back);
    return std::memcmp(back, src, (unsigned int)(count * stride)) == 0;
}

int main()
{
    // one byte column with a 5-bit width
    { unsigned char src[6] = { 1, 31, 7, 0, 16, 9 }; CScriptEntry e = { 0, 5, 0, 0, 0, 0 };
      if (!RoundTrip(src, 6, 1, &e, 1)) { std::printf("00f39ed0_TEST FAIL\n"); return 1; } }
    // a byte column (gap strip + set mask) next to a word column (range bias, 20 bits spilling across dwords)
    { unsigned char src[12] = { 0x80 | 20, 0x00, 0x10, 0x80 | 28, 0x08, 0x20, 0x80 | 4, 0xff, 0x2f, 0x80 | 0, 0x10, 0x10 };
      CScriptEntry e[2] = { { 0x0a, 3, 0, 2, 0, 0x80 }, { 0x30, 20, 0x1000, 0, 0, 0 } };
      if (!RoundTrip(src, 4, 3, e, 2)) { std::printf("00f39ed0_TEST FAIL\n"); return 1; } }
    // a byte column with the kept-run shuffle, and a dword column stored at 32 bits
    { unsigned char src[20] = { 0x0a, 1, 2, 3, 4, 0x08, 5, 6, 7, 8, 0x02, 9, 10, 11, 12, 0x00, 13, 14, 15, 16 };
      CScriptEntry e[2] = { { 0x04, 2, 0, 0, 0x0a, 0 }, { 0x40, 32, 0, 0, 0, 0 } };
      if (!RoundTrip(src, 4, 5, e, 2)) { std::printf("00f39ed0_TEST FAIL\n"); return 1; } }
    // raw data: leading 0 byte, copied as is; statistics count the bytes and both clock reads
    { unsigned char raw[7] = { 0, 1, 2, 3, 4, 5, 6 }; unsigned char out[6];
      g_RangeDecompressBytes = 0; g_RangeDecompressTime = 0.0; g_clock = 10.0;
      Decompress(raw, 3, 2, out);
      if (std::memcmp(out, raw + 1, 6) != 0 || g_RangeDecompressBytes != 6 || g_RangeDecompressTime != 1.0) { std::printf("00f39ed0_TEST FAIL\n"); return 1; } }
    std::printf("00f39ed0_TEST PASS\n");
    return 0;
}

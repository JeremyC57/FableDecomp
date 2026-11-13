// Behaviour test for CRangeCompressor::CalcBestCompressionForBlock (0x00F3A890): the row's body and its same-TU callees over a
// std::vector-based CArray model; checks the chosen transform, its script entry and the returned size on two hand-worked blocks.
#include <cstdio>
#include <vector>

template<class T> class CArray : public std::vector<T> { };

class CRangeCompressor {
public:
    struct CScriptEntry {
        CScriptEntry() : Flags(0) { }
        unsigned long Flags;
        long BitCount;
        unsigned long RangeBias;
        long RangeShift;
        unsigned long ShuffleMask;
        unsigned long OrMask;
    };
    long GetBlockSizeFromFlags(unsigned long flags);
    long CalcHeaderSizeNeeded(const CScriptEntry& entry);
    long CalcBitsNeeded(const CArray<unsigned long>& values);
    long CalcMemoryNeededBitCompressed(const CArray<unsigned long>& values);
    void TestRangeBias(const CArray<unsigned long>& values, CArray<unsigned long>& work, CScriptEntry& entry, long bytes);
    void TestRedundantBitStrip(const CArray<unsigned long>& values, CArray<unsigned long>& work, CScriptEntry& entry, long bytes);
    long CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry);
};

inline long CRangeCompressor::GetBlockSizeFromFlags(unsigned long flags)
{
    if (flags & 0x40)
        return 4;
    return (flags & 0x20) ? 2 : 1;
}
inline long CRangeCompressor::CalcHeaderSizeNeeded(const CScriptEntry& entry)
{
    long size = 2;
    long blockSize = GetBlockSizeFromFlags(entry.Flags);
    if (entry.Flags & 0x11)
        size += blockSize;
    if (entry.Flags & 2)
        size += 1;
    if (entry.Flags & 4)
        size += blockSize;
    if (entry.Flags & 8)
        size += blockSize;
    return size;
}
inline long CRangeCompressor::CalcBitsNeeded(const CArray<unsigned long>& values)
{
    unsigned long mask = values[0];
    long bits = 0;
    for (unsigned long i = 0; i < values.size(); ++i)
        mask |= values[i];
    for (long bit = 31; bit >= 0; --bit) {
        if (mask & (1 << bit)) {
            bits = bit + 1;
            break;
        }
    }
    return bits;
}

const unsigned long g_RangeCompressorByteMasks[] = {0, 0xff, 0xffff, 0xffffff, 0xffffffff};   // 0x013AC944

long CRangeCompressor::CalcMemoryNeededBitCompressed(const CArray<unsigned long>& values)   // 0x00F3A270 (landed)
{
    unsigned long mask = values[0];
    unsigned long bits = 0;
    for (unsigned long i = 0; i < values.size(); ++i)
        mask |= values[i];
    for (long bit = 31; bit >= 0; --bit) {
        if (mask & (1 << bit)) {
            bits = bit + 1;
            break;
        }
    }
    return (values.size() * bits + 31) / 32 * 4;
}

void CRangeCompressor::TestRangeBias(const CArray<unsigned long>& values, CArray<unsigned long>& work, CScriptEntry& entry, long bytes)   // 0x00F3A5D0 (landed)
{
    long minS = values[0];
    long maxS = minS;
    unsigned long minU = minS;
    unsigned long maxU = minS;
    for (unsigned long i = 0; i < values.size(); ++i) {
        unsigned long value = values[i];
        long extended = (value << ((4 - bytes) * 8)) >> ((4 - bytes) * 8);
        if (minU > value)
            minU = value;
        if (maxU < value)
            maxU = value;
        if (minS > extended)
            minS = extended;
        if (maxS < extended)
            maxS = extended;
    }
    unsigned long bias = minU;
    if (maxU - minU > (unsigned long)(maxS - minS))
        bias = minS & g_RangeCompressorByteMasks[bytes];
    work.resize(values.size());
    for (unsigned long j = 0; j < values.size(); ++j)
        work[j] = (values[j] - bias) & g_RangeCompressorByteMasks[bytes];
    if (!(entry.Flags & 0x10)) {
        entry.RangeBias = bias;
        entry.Flags |= 1;
    }
    else
        entry.RangeBias = bias;
}

void CRangeCompressor::TestRedundantBitStrip(const CArray<unsigned long>& values, CArray<unsigned long>& work, CScriptEntry& entry, long bytes)   // 0x00F3A710 (landed)
{
    unsigned long allClear = g_RangeCompressorByteMasks[bytes];
    unsigned long allSet = allClear;
    work.resize(values.size(), 0);
    for (unsigned long i = 0; i < values.size(); ++i) {
        allClear &= ~values[i];
        allSet &= values[i];
    }
    unsigned long redundant = allSet | allClear;
    for (unsigned long j = 0; j < values.size(); ++j) {
        unsigned long value = values[j];
        unsigned long packed = 0;
        unsigned long outBit = 1;
        unsigned long inBit = 1;
        for (long b = 0; b < bytes * 8; ++b) {
            if (!(redundant & inBit)) {
                if (value & inBit)
                    packed += outBit;
                outBit *= 2;
            }
            inBit *= 2;
        }
        work[j] = packed;
    }
    if (allSet) {
        entry.Flags |= 8;
        entry.OrMask = allSet;
    }
    bool seenClear = false;
    bool setAfterClear = false;
    bool gapRecorded = false;
    bool seenSet = false;
    for (long bit = 0; bit < bytes * 8; ++bit) {
        if (redundant & (1 << bit)) {
            seenSet = true;
            if (seenClear)
                setAfterClear = true;
        } else {
            if (seenSet && !gapRecorded) {
                entry.Flags |= 2;
                entry.RangeShift = bit;
                gapRecorded = true;
            }
            if (setAfterClear) {
                entry.Flags |= 4;
                entry.ShuffleMask = ~redundant & g_RangeCompressorByteMasks[bytes];
                entry.Flags &= ~2;
                return;
            }
            seenClear = true;
        }
    }
}

long CRangeCompressor::CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry)
{
    CScriptEntry entries[5];
    if (bytes == 2)
        entries[0].Flags = 0x20;
    else if (bytes == 4)
        entries[0].Flags = 0x40;
    CArray<unsigned long> arrays[5];
    arrays[0] = values;
    entries[1] = entries[0];
    TestRedundantBitStrip(values, arrays[1], entries[1], bytes);
    entries[2] = entries[1];
    TestRangeBias(arrays[1], arrays[2], entries[2], bytes);
    entries[3] = entries[0];
    entries[3].Flags &= ~1;
    entries[3].Flags |= 0x10;
    TestRangeBias(values, arrays[3], entries[3], bytes);
    entries[4] = entries[3];
    TestRedundantBitStrip(arrays[3], arrays[4], entries[4], bytes);
    long best = 0x7fffffff;
    long bestIndex = 0;
    for (long i = 0; i < 5; ++i) {
        long size = CalcHeaderSizeNeeded(entries[i]) + CalcMemoryNeededBitCompressed(arrays[i]);
        if (size < best) {
            bestIndex = i;
            best = size;
        }
    }
    entry = entries[bestIndex];
    entry.BitCount = CalcBitsNeeded(arrays[bestIndex]);
    return best;
}

int main()
{
    CRangeCompressor rc;
    CArray<unsigned long> a;
    for (int i = 0; i < 4; ++i) a.push_back(5);
    CRangeCompressor::CScriptEntry e;
    long size = rc.CalcBestCompressionForBlock(a, 1, e);
    // constant block: the bit strip wins (header 3, no payload); OrMask records the constant
    if (size != 3 || e.Flags != 8 || e.OrMask != 5 || e.BitCount != 0) { std::printf("00f3a890_TEST FAIL\n"); return 1; }
    CArray<unsigned long> b;
    b.push_back(1); b.push_back(2); b.push_back(3);
    CRangeCompressor::CScriptEntry f;
    size = rc.CalcBestCompressionForBlock(b, 1, f);
    // {1,2,3}: untransformed wins (2 + 4 bytes); width 2 bits
    if (size != 6 || f.Flags != 0 || f.BitCount != 2) { std::printf("00f3a890_TEST FAIL\n"); return 1; }
    CArray<unsigned long> c;
    c.push_back(0x1234); c.push_back(0x1235);
    CRangeCompressor::CScriptEntry g;
    size = rc.CalcBestCompressionForBlock(c, 2, g);
    if ((g.Flags & 0x20) == 0 || size != rc.CalcHeaderSizeNeeded(g) + 4) { std::printf("00f3a890_TEST FAIL\n"); return 1; }
    std::printf("00f3a890_TEST PASS\n");
    return 0;
}
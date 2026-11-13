// CRangeCompressor::CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry) -- try five
// transforms of a block (as is, bit-stripped, stripped then range-biased, range-biased, range-biased then stripped), pick the one
// with the smallest header plus bit-packed size, return its script entry (with its bit width) in `entry`, and return that size.
// Leaf: ?CalcBestCompressionForBlock@CRangeCompressor@@AAEJABV?$CArray@K@@JAAUCScriptEntry@1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3A890, length 857 bytes (ret 0xc); Ego_r.pdb / FableWin.pdb
// ?CalcBestCompressionForBlock@CRangeCompressor@@AAEJABV?$CArray@K@@JAAUCScriptEntry@1@@Z (private, non-virtual).
// Callees: std::vector<unsigned long>::operator= 0x0045BC09, TestRedundantBitStrip 0x00F3A710, TestRangeBias 0x00F3A5D0,
// CalcMemoryNeededBitCompressed 0x00F3A270, free 0x00BFEA14 (the inline ~vector of the five work arrays).
// GetBlockSizeFromFlags / CalcHeaderSizeNeeded / CalcBitsNeeded (Ego_r 0x00BDC090 / 0x00BDC440 / 0x00BDC480) are defined
// earlier in the same source file and fully inlined in TLC (no out-of-line copy), so they are inline definitions here.
// Shape: CalcMemoryNeededBitCompressed, TestRangeBias and TestRedundantBitStrip are defined EARLIER in the same retail TU
// (Ego_r order 0xBDC090..0xBDD210 == TLC 0xF39E50..0xF3AFC0) and called out of line; their REAL landed bodies sit here under
// #pragma auto_inline(off). Seeing them, VC7.1 knows they do not capture the CScriptEntry references, so the local entries[]
// is not globally exposed: entries[0].Flags stays in ebp across the vector::operator= call, entries[1]'s ctor zero-store dies,
// and `this` is spilled (declaration-only Test* callees: Flags reloaded, this in ebp, 844v857). STLport's _vector.c operator=
// body is visible (FABLE_RETAIL_STLP_VECTOR_C) as in retail. CalcBitsNeeded uses CalcMemoryNeededBitCompressed's
// `bits = bit + 1; break;` form (xor esi,esi preset, lea esi,[ecx+1]); that also gives bestIndex edi and i a frame slot.
#define FABLE_RETAIL_STLP_VECTOR_C
#include "engine/CRangeCompressor.h"

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

#pragma auto_inline(off)
extern const unsigned long g_RangeCompressorByteMasks[];   // 0x013AC944 {0, 0xff, 0xffff, 0xffffff, 0xffffffff}

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
#pragma auto_inline(on)

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
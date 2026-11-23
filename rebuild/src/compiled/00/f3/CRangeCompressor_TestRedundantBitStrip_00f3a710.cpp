// CRangeCompressor::TestRedundantBitStrip(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes) -- strips the bits that are constant across every value and records the strip in the entry
// Leaf: ?TestRedundantBitStrip@CRangeCompressor@@AAEXABV?$CArray@K@@AAV2@AAUCScriptEntry@1@J@Z
// Layout: retail CArray<T> is begin/end/capacity (landed 0x00F3A270); CRangeCompressor is empty in the PDB; the
// CScriptEntry fields are named from use only (+0 flags: 8 = constant-set bits, 2 = gap start at +0xC, 4 = gap
// failed with the kept mask at +0x10; +0x14 = bits set in every value). out.resize is 0x00464931 (resize(n, const T&)).
// Shape: the repack loop walks explicit inBit/outBit masks (`*= 2` gives `add r,r`; `1 << b` gives `shl cl`), and the
// four scan bools are declared seenClear, setAfterClear, gapRecorded, seenSet (that order gives the dl/bl split).
// NF48 (round 7): CArray<unsigned long> is the shared engine/CArray.h (a public STLport 4.6.2 std::vector<unsigned long>,
// a local begin/end/capacity model before); resize(n, const unsigned long&) is vector<unsigned long>::resize 0x00464931
// (?resize@?$vector@KV?$allocator@K@std@@@std@@QAEXIABK@Z), kept out of line by FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE.
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/CArray.h"

extern const unsigned long g_RangeCompressorByteMasks[];

struct CRangeCompressor {
    struct CScriptEntry {
        unsigned long Flags;        // +0x00
        long Pad04[2];
        long GapStart;              // +0x0C
        unsigned long KeptMask;     // +0x10
        unsigned long SetMask;      // +0x14
    };
    private:
    void TestRedundantBitStrip(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes);
    public:
};

void CRangeCompressor::TestRedundantBitStrip(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes)
{
    unsigned long allClear = g_RangeCompressorByteMasks[bytes];
    unsigned long allSet = allClear;
    out.resize(in.size(), 0);
    for (unsigned long i = 0; i < in.size(); ++i) {
        allClear &= ~in[i];
        allSet &= in[i];
    }
    unsigned long redundant = allSet | allClear;
    for (unsigned long j = 0; j < in.size(); ++j) {
        unsigned long value = in[j];
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
        out[j] = packed;
    }
    if (allSet) {
        entry.Flags |= 8;
        entry.SetMask = allSet;
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
                entry.GapStart = bit;
                gapRecorded = true;
            }
            if (setAfterClear) {
                entry.Flags |= 4;
                entry.KeptMask = ~redundant & g_RangeCompressorByteMasks[bytes];
                entry.Flags &= ~2;
                return;
            }
            seenClear = true;
        }
    }
}
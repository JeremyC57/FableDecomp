// CRangeCompressor::TestRedundantBitStrip(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes) -- strips the bits that are constant across every value and records the strip in the entry
// Leaf: TestRedundantBitStrip
// Layout: retail CArray<T> is begin/end/capacity (landed 0x00F3A270); CRangeCompressor is empty in the PDB; the
// CScriptEntry fields are named from use only (+0 flags: 8 = constant-set bits, 2 = gap start at +0xC, 4 = gap
// failed with the kept mask at +0x10; +0x14 = bits set in every value). out.resize is 0x00464931 (resize(n, const T&)).
// Shape: the repack loop walks explicit inBit/outBit masks (`*= 2` gives `add r,r`; `1 << b` gives `shl cl`), and the
// four scan bools are declared seenClear, setAfterClear, gapRecorded, seenSet (that order gives the dl/bl split).
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
    T& operator[](unsigned long i) { return m_begin[i]; }
    void resize(unsigned long size, const T& value);
};

extern const unsigned long g_RangeCompressorByteMasks[];

struct CRangeCompressor {
    struct CScriptEntry {
        unsigned long Flags;        // +0x00
        long Pad04[2];
        long GapStart;              // +0x0C
        unsigned long KeptMask;     // +0x10
        unsigned long SetMask;      // +0x14
    };
    void TestRedundantBitStrip(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes);
};

void CRangeCompressor::TestRedundantBitStrip(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes)
{
    unsigned long allClear = g_RangeCompressorByteMasks[bytes];
    unsigned long allSet = allClear;
    out.resize(in.Size(), 0);
    for (unsigned long i = 0; i < in.Size(); ++i) {
        allClear &= ~in[i];
        allSet &= in[i];
    }
    unsigned long redundant = allSet | allClear;
    for (unsigned long j = 0; j < in.Size(); ++j) {
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

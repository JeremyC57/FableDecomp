#include <cstdio>
#include <vector>
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
    T& operator[](unsigned long i) { return m_begin[i]; }
    void resize(unsigned long size, const T& value) {
        T* n = new T[size]; for (unsigned long i = 0; i < size; ++i) n[i] = i < Size() ? m_begin[i] : value;
        m_begin = n; m_end = n + size; m_capacity = m_end;
    }
};
const unsigned long g_RangeCompressorByteMasks[] = { 0, 0xFF, 0xFFFF, 0xFFFFFF, 0xFFFFFFFF };
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

static int fails = 0;
static void check(bool c, const char* m) { if (!c) { printf("FAIL %s\n", m); ++fails; } }
int main()
{
    CRangeCompressor rc;
    // values share bit 0 set and bits 4..7 clear; varying bits 1..3 -> packed contiguously, no gap
    unsigned long v1[] = { 0x03, 0x05, 0x0F, 0x01 };
    CArray<unsigned long> in = { v1, v1 + 4, v1 + 4 };
    CArray<unsigned long> out = { 0, 0, 0 };
    CRangeCompressor::CScriptEntry e = { 0, {0, 0}, 0, 0, 0 };
    rc.TestRedundantBitStrip(in, out, e, 1);
    check(out.Size() == 4, "size");
    check(out[0] == 1 && out[1] == 2 && out[2] == 7 && out[3] == 0, "packed");
    check(e.Flags == (8 | 2) && e.SetMask == 1 && e.GapStart == 1, "flags1");
    // redundant bits 0 and 2 (clear in all), varying 1 and 3: clear,var,clear,var -> fails
    unsigned long v2[] = { 0x02, 0x08 };
    CArray<unsigned long> in2 = { v2, v2 + 2, v2 + 2 };
    CArray<unsigned long> out2 = { 0, 0, 0 };
    CRangeCompressor::CScriptEntry e2 = { 0, {0, 0}, 0, 0, 0 };
    rc.TestRedundantBitStrip(in2, out2, e2, 1);
    check(out2[0] == 1 && out2[1] == 2, "packed2");
    check(e2.Flags == 4 && e2.KeptMask == 0x0A, "flags2");
    if (!fails) printf("00f3a710_TEST PASS\n");
    return fails;
}

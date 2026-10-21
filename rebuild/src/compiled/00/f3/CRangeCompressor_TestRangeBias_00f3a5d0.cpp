// CRangeCompressor::TestRangeBias(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes) -- find the unsigned and the sign-extended (to `bytes` wide) value ranges of the input, pick the bias that gives the smaller range, write the biased, masked values to `out`, and record the bias in the script entry (flagging it as biased unless flag 0x10 is set)
// Leaf: TestRangeBias
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3A5D0, length 320 bytes (`ret 10h`). FableWin.pdb
// ?TestRangeBias@CRangeCompressor@@AAEXABV?$CArray@K@@AAV2@AAUCScriptEntry@1@J@Z; CScriptEntry is 24 bytes
// {Flags, BitCount, RangeBias, RangeShift, ShuffleMask, OrMask} (PDB). CArray<unsigned long> is the 12-byte
// retail STLport vector {first, last, end}; resize() is inlined (erase tail through the memmove copy-down
// helper @ 0x00BFEAE6, grow via _M_fill_insert @ 0x004649A3). The byte masks {0, 0xff, 0xffff, 0xffffff,
// 0xffffffff} live at 0x013AC944.
// Shape: signed-first declarations (minS, then maxS, minU, maxU copied from it; this order gives the register map and
// sets ebx before the maxU spill), the `minU > value` comparison orientation, and the bias store written in both arms of
// the flag test (`if (!(Flags & 0x10)) { RangeBias = bias; Flags |= 1; } else RangeBias = bias;`): VC7.1 tail-merges
// the store above the branch and keeps the callee-saved pops after the join (a single store before the `if` lets it
// hoist the pops between the test and the store).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);

template <class T> class CArray {
public:
    T* first;   // +0
    T* last;    // +4
    T* end_;    // +8
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long i) { return first[i]; }
    const T& operator[](unsigned long i) const { return first[i]; }
    void InsertN(T* where, unsigned long count, const T& value);   // 0x004649A3
    static T* CopyDown(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (unsigned long)((char*)srcEnd - (char*)src);
        return (T*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void resize(unsigned long n, const T& value = T())
    {
        if (n < size())
            last = CopyDown(last, last, first + n);
        else
            InsertN(last, n - size(), value);
    }
};

extern const unsigned long g_RangeCompressorByteMasks[];   // 0x013AC944

class CRangeCompressor {
public:
    struct CScriptEntry {
        unsigned long Flags;        // +0x00
        long          BitCount;     // +0x04
        unsigned long RangeBias;    // +0x08
        long          RangeShift;   // +0x0c
        unsigned long ShuffleMask;  // +0x10
        unsigned long OrMask;       // +0x14
    };
    void TestRangeBias(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes);
};

void CRangeCompressor::TestRangeBias(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes)
{
    long minS = in[0];
    long maxS = minS;
    unsigned long minU = minS;
    unsigned long maxU = minS;
    for (unsigned long i = 0; i < in.size(); ++i) {
        unsigned long value = in[i];
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
    out.resize(in.size());
    for (unsigned long j = 0; j < in.size(); ++j)
        out[j] = (in[j] - bias) & g_RangeCompressorByteMasks[bytes];
    if (!(entry.Flags & 0x10)) {
        entry.RangeBias = bias;
        entry.Flags |= 1;
    }
    else
        entry.RangeBias = bias;
}
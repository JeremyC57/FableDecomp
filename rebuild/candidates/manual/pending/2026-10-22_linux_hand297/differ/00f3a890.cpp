// CRangeCompressor::CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry) -- try five transforms of a block (as is, bit-stripped, stripped then range-biased, range-biased, range-biased then stripped), pick the one with the smallest header plus bit-packed size, return it in the entry with its bit width, and return that size
// Grade: DIFFER(844v857) -- retail keeps entries[0].Flags in ebp across the CArray::operator= call (copies it into entries[1] from the register, drops entries[1]'s ctor zero-store), keeps ebx=0 for the whole body and spills this to frame 0; ours reloads Flags from memory, keeps this in ebp. Tried: flags local, arrays first, copy before arrays (832v857, fewest diff lines), bestIndex/best order.
// Leaf: CalcBestCompressionForBlock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3A890, length 864 bytes (ret 0xc); PDB
// ?CalcBestCompressionForBlock@CRangeCompressor@@AAEJABV?$CArray@K@@JAAUCScriptEntry@1@@Z.
// Callees: CArray<unsigned long>::operator= 0x0045BC09, TestRedundantBitStrip 0x00F3A710, TestRangeBias 0x00F3A5D0,
// CalcMemoryNeededBitCompressed 0x00F3A270, the CRT free thunk 0x00BFEA14 (inline ~CArray); CalcHeaderSizeNeeded,
// GetBlockSizeFromFlags and CalcBitsNeeded (out of line in Ego_r.exe) are inlined.
// Layout: the retail 12-byte CArray {begin, end, capacity} of the landed siblings; CScriptEntry is 24 bytes with Flags at
// +0 (0x20 / 0x40 = 2- / 4-byte blocks, 0x10 = range bias, 1 / 2 / 4 / 8 = the strip records) and the bit width at +4.
extern "C" void __cdecl free(void* block);

template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) {}
    ~CArray() { if (m_begin) free(m_begin); }
    CArray& operator=(const CArray& other);            // 0x0045BC09 (CArray<unsigned long>)
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
};

struct CRangeCompressor {
    struct CScriptEntry {
        unsigned long Flags;        // +0x00
        long BitsNeeded;            // +0x04
        long Pad08;
        long GapStart;              // +0x0C
        unsigned long KeptMask;     // +0x10
        unsigned long SetMask;      // +0x14
        CScriptEntry() : Flags(0) {}
    };
    long GetBlockSizeFromFlags(unsigned long flags)
    {
        if (flags & 0x40)
            return 4;
        return (flags & 0x20) ? 2 : 1;
    }
    long CalcHeaderSizeNeeded(const CScriptEntry& entry)
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
    long CalcBitsNeeded(const CArray<unsigned long>& values)
    {
        unsigned long all = values[0];
        for (unsigned long i = 0; i < values.Size(); ++i)
            all |= values[i];
        for (long bit = 31; bit >= 0; --bit) {
            if (all & (1 << bit))
                return bit + 1;
        }
        return 0;
    }
    void TestRedundantBitStrip(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes);   // 0x00F3A710
    void TestRangeBias(const CArray<unsigned long>& in, CArray<unsigned long>& out, CScriptEntry& entry, long bytes);           // 0x00F3A5D0
    long CalcMemoryNeededBitCompressed(const CArray<unsigned long>& values);                                                    // 0x00F3A270
    long CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry);
};

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
    entry.BitsNeeded = CalcBitsNeeded(arrays[bestIndex]);
    return best;
}

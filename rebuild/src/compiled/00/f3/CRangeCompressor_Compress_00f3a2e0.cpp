// CRangeCompressor::Compress(const unsigned char* source, long count, long stride, unsigned char* dest, const CArray<CScriptEntry>& entries) -- write each column's script entry header (flags, bit width, bias, gap start, set mask, kept mask) and its count values, transformed by the entry and bit-packed into dwords, then the 0x80 end marker; returns the bytes written
// Leaf: Compress
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3A2E0, length 752 bytes (ret 0x14); PDB
// ?Compress@CRangeCompressor@@AAEJPBEJJPAEABV?$CArray@UCScriptEntry@CRangeCompressor@@@@@Z.
// Callees: CalcShuffleOperations 0x00F39E50 (landed); data: the byte-width masks 0x013AC944 (the landed
// TestRedundantBitStrip's g_RangeCompressorByteMasks).
// Layout: retail CArray {begin, end, capacity}; CScriptEntry (24 bytes) Flags +0, BitsNeeded +4, Bias +8 (range bias
// minimum), GapStart +0x0C, KeptMask +0x10, SetMask +0x14 (named from the landed TestRedundantBitStrip /
// TestRangeBias). Column i of the source starts i * blockSize bytes in; its values are `stride` bytes apart.
// Shape: the entry is a `const CScriptEntry&` bound once per column (indexing entries[i] at every use re-reads the
// array's begin after each byte store to dest); the column's values are read at `source + j * stride` (a pointer local
// initialised before the loop is loaded ahead of the count test).
template<class T> class CArray { public:
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
};
extern const unsigned long g_RangeCompressorByteMasks[];      // 0x013AC944

struct CRangeCompressor {
    struct CScriptEntry {
        unsigned long Flags;        // +0x00
        long BitsNeeded;            // +0x04
        unsigned long Bias;         // +0x08
        long GapStart;              // +0x0C
        unsigned long KeptMask;     // +0x10
        unsigned long SetMask;      // +0x14
    };
    long GetBlockSizeFromFlags(unsigned long flags)
    {
        if (flags & 0x40)
            return 4;
        return (flags & 0x20) ? 2 : 1;
    }
    void WriteValue(unsigned char* dest, unsigned long value, long blockSize)
    {
        if (blockSize == 4)
            *(unsigned long*)dest = value;
        else if (blockSize == 2)
            *(unsigned short*)dest = (unsigned short)value;
        else
            *dest = (unsigned char)value;
    }
    unsigned long ReadValue(const unsigned char* source, long blockSize)
    {
        if (blockSize == 4)
            return *(const unsigned long*)source;
        if (blockSize == 2)
            return *(const unsigned short*)source;
        return *source;
    }
    long CalcShuffleOperations(unsigned long mask, unsigned long* runMasks, unsigned long* shifts);   // 0x00F39E50
    long Compress(const unsigned char* source, long count, long stride, unsigned char* dest, const CArray<CScriptEntry>& entries);
};

long CRangeCompressor::Compress(const unsigned char* source, long count, long stride, unsigned char* dest, const CArray<CScriptEntry>& entries)
{
    long pos = 0;
    unsigned long runMasks[16];
    unsigned long shifts[16];
    for (unsigned long i = 0; i < entries.Size(); ++i) {
        const CScriptEntry& entry = entries[i];
        dest[pos] = (unsigned char)entry.Flags;
        dest[pos + 1] = (unsigned char)entry.BitsNeeded;
        pos += 2;
        long runs = 0;
        long blockSize = GetBlockSizeFromFlags(entry.Flags);
        if (entry.Flags & 0x11) {
            WriteValue(dest + pos, entry.Bias, blockSize);
            pos += blockSize;
        }
        if (entry.Flags & 2) {
            dest[pos] = (unsigned char)entry.GapStart;
            ++pos;
        }
        if (entry.Flags & 8) {
            WriteValue(dest + pos, entry.SetMask, blockSize);
            pos += blockSize;
        }
        if (entry.Flags & 4) {
            WriteValue(dest + pos, entry.KeptMask, blockSize);
            runs = CalcShuffleOperations(entry.KeptMask, runMasks, shifts);
            pos += blockSize;
        }
        long bits = entry.BitsNeeded;
        if (bits > 0) {
            unsigned long acc = 0;
            long accBits = 0;
            for (long j = 0; j < count; ++j) {
                unsigned long value = ReadValue(source + j * stride, blockSize);
                unsigned long flags = entry.Flags;
                if (flags & 0x10)
                    value = (value - entry.Bias) & g_RangeCompressorByteMasks[blockSize];
                if (flags & 8)
                    value &= ~entry.SetMask;
                if (flags & 2)
                    value >>= entry.GapStart;
                else if (flags & 4) {
                    unsigned long packed = 0;
                    for (long r = 0; r < runs; ++r)
                        packed |= (runMasks[r] & value) >> shifts[r];
                    value = packed;
                }
                if (entry.Flags & 1)
                    value = (value - entry.Bias) & g_RangeCompressorByteMasks[blockSize];
                if (bits == 32) {
                    *(unsigned long*)(dest + pos) = value;
                    pos += 4;
                } else {
                    acc |= value << accBits;
                    accBits += bits;
                    if (accBits >= 32) {
                        *(unsigned long*)(dest + pos) = acc;
                        acc = value >> (bits - accBits + 32);
                        pos += 4;
                        accBits -= 32;
                    }
                }
            }
            if (accBits > 0) {
                *(unsigned long*)(dest + pos) = acc;
                pos += 4;
            }
        }
        source += blockSize;
    }
    dest[pos] = 0x80;
    return pos + 1;
}
// CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride, long bytes) -- choose the cheapest compression script for `count` values of `bytes` width: wider than 4 bytes in 4-byte columns, 3 as 2+1, otherwise one whole-width block against two half-width scripts
// Grade: DIFFER(605v605) -- register swap only: retail keeps `this` in ebp and the remaining-width counter in ebx, ours the other way round; every instruction otherwise identical.
// Leaf: CalcCompressionScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3ABF0, length 603 bytes (ret 0x14); PDB
// ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z.
// Callees: itself (recursive), CArray<unsigned long>(count) @ 0x00A96100, CalcBestCompressionForBlock @ 0x00F3A890,
// CArray<CScriptEntry>::push_back @ 0x00F3B420, the CRT free thunk @ 0x00BFEA14 (inline ~CArray).
// Layout: the retail 12-byte CArray {first, last, end} of the landed siblings (0x00F3AFC0, 0x00F3A710); CScriptEntry is
// 24 bytes (`/ 24` by 0x2AAAAAAB) and its Flags word starts at 0.
// Shape: the 3-byte case sums the 1-byte column at +2 and the 2-byte column; the whole-width cost lands in the dead
// count slot, the half-width total in the dead stride slot; a cheaper split appends every sub-entry.
extern "C" void __cdecl free(void* block);

template<class T> struct CArray {
    T* m_first;                                                 // +0
    T* m_last;                                                  // +4
    T* m_end;                                                   // +8
    CArray() : m_first(0), m_last(0), m_end(0) {}
    explicit CArray(unsigned long count);                       // 0x00A96100 (CArray<unsigned long>)
    ~CArray() { if (m_first) free(m_first); }
    unsigned long Size() const { return (unsigned long)(m_last - m_first); }
    T& operator[](unsigned long i) { return m_first[i]; }
    void push_back(const T& value);                             // 0x00F3B420 (CArray<CScriptEntry>)
};

struct CRangeCompressor {
    struct CScriptEntry {
        unsigned long Flags;        // +0x00
        long Data[5];
    };
    long CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry);   // 0x00F3A890
    long CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride, long bytes);
};

long CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count,
                                             long stride, long bytes)
{
    long total = 0;
    if (bytes > 4) {
        for (long offset = 0; offset < bytes; offset += 4)
            total += CalcCompressionScript(script, data + offset, count, stride, bytes - offset > 4 ? 4 : bytes - offset);
        return total;
    }
    if (bytes == 3)
        return CalcCompressionScript(script, data + 2, count, stride, 1)
             + CalcCompressionScript(script, data, count, stride, 2);
    CArray<unsigned long> values(count);
    const unsigned char* source = data;
    for (long i = 0; i < count; ++i) {
        unsigned long value;
        if (bytes == 4)
            value = *(const unsigned long*)source;
        else if (bytes == 2)
            value = *(const unsigned short*)source;
        else
            value = *source;
        values[i] = value;
        source += stride;
    }
    CScriptEntry entry;
    entry.Flags = 0;
    long whole = CalcBestCompressionForBlock(values, bytes, entry);
    if (bytes == 1) {
        script.push_back(entry);
        return whole;
    }
    CArray<CScriptEntry> split;
    long half = bytes / 2;
    long splitCost = CalcCompressionScript(split, data, count, stride, half);
    splitCost += CalcCompressionScript(split, data + half, count, stride, half);
    if (whole <= splitCost) {
        script.push_back(entry);
        return whole;
    }
    for (unsigned long j = 0; j < split.Size(); ++j)
        script.push_back(split[j]);
    return splitCost;
}

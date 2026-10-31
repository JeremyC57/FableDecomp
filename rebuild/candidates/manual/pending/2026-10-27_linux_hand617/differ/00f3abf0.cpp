// CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride, long bytes) -- choose the cheapest compression script for `count` values of `bytes` width: wider than 4 bytes in 4-byte columns, 3 as 2+1, otherwise one whole-width block against two half-width scripts
// Grade: DIFFER(605v605) -- one scheduling pair: in the bytes>4 split loop `mov ebx,edi` (remaining = bytes) is placed before the `jle` guard where retail puts it after | tried (chunk 617): explicit guard + do-while value loop (win, sinks the pointer load), explicit guards / IV forms on the split loop (this/counter swap to ebx/ebp), `0 < bytes` guard (fixes loop 1, breaks value-loop guard), `bytes > 0` (607), for-init counter (same) | next: a while(remaining > 0) form with the copy inside the guarded scope
// Leaf: ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z
// Earlier grade: DIFFER(605v605) (retry wave 2, improved) -- a separate remaining-width counter in the split loop gives retail's
//   this-in-ebp / counter-in-ebx allocation; residue: the counter copy (mov ebx,edi) one slot early and the value-copy
//   loop's ecx/edx swap (source pointer vs value). Tried: values[i] stores per branch, counter at function scope
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3ABF0, length 603 bytes (ret 0x14); PDB
// ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z.
// Callees: itself (recursive), CArray<unsigned long>(count) @ 0x00A96100, CalcBestCompressionForBlock @ 0x00F3A890,
// CArray<CScriptEntry>::push_back @ 0x00F3B420, the CRT free thunk @ 0x00BFEA14 (inline ~CArray).
// Layout: the retail 12-byte CArray {first, last, end} of the landed siblings (0x00F3AFC0, 0x00F3A710); CScriptEntry is
// 24 bytes (`/ 24` by 0x2AAAAAAB) and its Flags word starts at 0.
// Shape: the 3-byte case sums the 1-byte column at +2 and the 2-byte column; the whole-width cost lands in the dead
// count slot, the half-width total in the dead stride slot; a cheaper split appends every sub-entry.
extern "C" void __cdecl free(void* block);                       // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
// STLport 4.6 vector (12 bytes): the sized constructor and push_back are out of line (landed bodies).
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                   // +0
    _Tp* _M_finish;                                                  // +4
    _Tp* _M_end_of_storage;                                          // +8
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    explicit vector(size_type __n);                                  // 0x00A96100 (vector<unsigned long>)
    ~vector() { if (_M_start) free(_M_start); }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
    void push_back(const _Tp& __x);                                  // 0x00F3B420 (vector<CRangeCompressor::CScriptEntry>)
};
}
// CArray<T> (Ego_r class template, V): the engine's std::vector wrapper; FableWin ??0?$CArray@K@@QAE@I@Z is the sized ctor.
template <class T> class CArray : public std::vector<T> {
public:
    CArray() {}
    explicit CArray(unsigned int count) : std::vector<T>(count) {}
};

class CRangeCompressor {
public:
    struct CScriptEntry {                                            // Ego_r CRangeCompressor::CScriptEntry (U), 24 bytes
        unsigned long Flags;                                         // +0x00
        long          BitCount;                                      // +0x04
        unsigned long RangeBias;                                     // +0x08
        long          RangeShift;                                    // +0x0C
        unsigned long ShuffleMask;                                   // +0x10
        unsigned long OrMask;                                        // +0x14
    };
private:
    long CalcBestCompressionForBlock(const CArray<unsigned long>& values, long bytes, CScriptEntry& entry);   // 0x00F3A890
    long CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride, long bytes);
};

long CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count,
                                             long stride, long bytes)
{
    long total = 0;
    if (bytes > 4) {
        long remaining = bytes;
        for (long offset = 0; offset < bytes; offset += 4, remaining -= 4)
            total += CalcCompressionScript(script, data + offset, count, stride, remaining > 4 ? 4 : remaining);
        return total;
    }
    if (bytes == 3)
        return CalcCompressionScript(script, data + 2, count, stride, 1)
             + CalcCompressionScript(script, data, count, stride, 2);
    CArray<unsigned long> values(count);
    long i = 0;
    if (i < count) {
        const unsigned char* source = data;
        do {
            unsigned long value;
            if (bytes == 4)
                value = *(const unsigned long*)source;
            else if (bytes == 2)
                value = *(const unsigned short*)source;
            else
                value = *source;
            values[i] = value;
            ++i;
            source += stride;
        } while (i < count);
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
    for (unsigned long j = 0; j < split.size(); ++j)
        script.push_back(split[j]);
    return splitCost;
}

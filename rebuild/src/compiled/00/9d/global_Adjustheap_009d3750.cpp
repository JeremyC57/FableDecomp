// _global::_Adjust_heap(unsigned long*, long, long, unsigned long, CBankFile::CompareDataOffset) -- Dinkumware heap sift-down of a bank's entry indices ordered by each entry's data offset (packed or from the runtime record), then _Push_heap @ 0x009D3040
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009d3750, 186 bytes (`ret 0Ch`).
// Callees: CPackedUIntArray::operator[] 0x00A629F0, _Push_heap 0x009D3040 (manifest:
// _Push_heap<unsigned long*,int,unsigned long,CBankFile::CompareDataOffset>).
// first in ecx, hole in edx (link-time __fastcall); bottom, the index value and the one-pointer
// comparator by value on the stack.
// Layout: the comparator holds the bank; the retail CBankFile layout of the landed sibling
// __unguarded_linear_insert 0x009D2FC0 (same comparator): RuntimeData +0x20 (12-byte records,
// DataOffset +0), PackedDataOffset +0x38 (packed when its Size word, +4, is non-zero).
// engine/CBankFile.h's default (Ego_r) layout puts these at +0x28/+0x48 and disagrees with the
// bytes, so the local retail model of 0x009D2FC0 is reused.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; GetEntryDataOffset inlined twice (right child first, its
// result parked in the comparator's own argument slot `[esp+24h]`), the unsigned `jae`; `top` and
// `hole` live in the two `sub esp,8` frame words.
namespace FableHeap_009d3750 {

template <class T> struct CArray {                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CRuntimeData {                              // CBankFile::CRuntimeData, 12 bytes
    unsigned long DataOffset;                      // +0x00
    unsigned long DataSize;                        // +0x04
    unsigned char DataType;                        // +0x08
    bool          Valid;                           // +0x09
};

struct CPackedUIntArray {                          // 16 bytes
    unsigned long* PackedInts;                     // +0x00
    unsigned long  Size;                           // +0x04 (0: not packed, use RuntimeData)
    unsigned char  Bits;                           // +0x08
    unsigned long  Bias;                           // +0x0C
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return Size != 0; }
};

struct CBankFile_Retail {                          // the retail layout (engine/CBankFile.h disagrees with the bytes)
    void*                     __vftable;           // +0x00
    unsigned long             Size;                // +0x04
    unsigned char             Symbols[0x0c];       // +0x08
    unsigned char             Checksums[0x0c];     // +0x14
    CArray<CRuntimeData>      RuntimeData;         // +0x20
    unsigned char             UpdateData[0x0c];    // +0x2c
    CPackedUIntArray          PackedDataOffset;    // +0x38
    unsigned long GetEntryDataOffset(unsigned long index) const
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
};

struct CompareDataOffset {
    const CBankFile_Retail* m_pBank;
    bool operator()(unsigned long a, unsigned long b) const
    {
        return m_pBank->GetEntryDataOffset(a) < m_pBank->GetEntryDataOffset(b);
    }
};

void __fastcall _Push_heap(unsigned long* first, long hole, long top, unsigned long value, CompareDataOffset pred);   // 0x009D3040

void __fastcall _Adjust_heap(unsigned long* first, long hole, long bottom, unsigned long value, CompareDataOffset pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}
// CBankFile::InternalResizeEntryData(unsigned long index, unsigned long size) -- give an entry room for `size` bytes: a new entry of a solid bank goes at the end of the file; otherwise, when the aligned size outgrows the entry's block, move it to the first free block (and invalidate the block order); the record takes the new size
// Leaf: InternalResizeEntryData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D1260, length 186 bytes (+6 int3), ret 8.
// Callees: CBankFile::GetAllocatedBlockSize @ 0x009D0FC0, CBankFile::FindEmptyBlockOffset @ 0x009D0EF0, the disk file's
// (+0x7c) virtual slot 9 (+0x24, GetLength), memmove @ 0x00BFEAE6 (the inlined BlockOrder.clear()).
// Shape: the record reference (ebp) and the aligned size (inline AlignOffset, edi) are computed first; Solid is cleared on
// the general path; BlockOrder.clear() is the copy-down helper of the landed DeleteEntry 0x009CD3C0; the size store is
// duplicated into every exit.
// Layout: the landed retail CBankFile model (0x009CC240): RuntimeData +0x20, BankFile_Data +0x7c, Solid +0x8f,
// Alignment +0x98, BlockOrder +0xe0.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct CRuntimeData {                              // CBankFile::CRuntimeData, 12 bytes
    unsigned long DataOffset;                      // +0x00
    unsigned long DataSize;                        // +0x04
    unsigned char DataType;                        // +0x08
    bool          Valid;                           // +0x09
    CRuntimeData() : DataOffset(0), DataSize(0), DataType(0), Valid(false) {}
};
template <class T> struct CArray {                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
    static T* CopyDown(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (unsigned long)((char*)srcEnd - (char*)src);
        return (T*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void clear() { m_pLast = CopyDown(m_pLast, m_pLast, m_pFirst); }
};

struct CDiskFileCalls {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual unsigned long GetLength();             // slot 9 (+0x24)
};

class CBankFile {
public:
    void*                     __vftable;            // +0x00
    unsigned long             Size;                 // +0x04
    unsigned char             Symbols[0x0c];        // +0x08 CArray<CCharString>
    unsigned char             Checksums[0x0c];      // +0x14 CArray<unsigned long>
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    unsigned char             _pad2c[0x50];         // +0x2c .. +0x7b (UpdateData, packed arrays, OpenFlags)
    CDiskFileCalls*           BankFile_Data;        // +0x7c
    unsigned char             _pad80[0x0f];         // +0x80 .. +0x8e
    bool                      Solid;                // +0x8f
    unsigned char             _pad90[0x08];         // +0x90 .. +0x97
    unsigned long             Alignment;            // +0x98
    unsigned char             _pad9c[0x44];         // +0x9c .. +0xdf
    CArray<unsigned long>     BlockOrder;           // +0xe0
    unsigned long AlignOffset(unsigned long value) const { return (value + Alignment - 1) / Alignment * Alignment; }
    unsigned long GetAllocatedBlockSize(unsigned long index);   // 0x009D0FC0
    unsigned long FindEmptyBlockOffset(unsigned long size);     // 0x009D0EF0
    void InternalResizeEntryData(unsigned long index, unsigned long size);
};

void CBankFile::InternalResizeEntryData(unsigned long index, unsigned long size)
{
    CRuntimeData& record = RuntimeData[index];
    unsigned long alignedSize = AlignOffset(size);
    unsigned long allocated = GetAllocatedBlockSize(index);
    if (allocated == 0 && Solid) {
        record.DataOffset = BankFile_Data->GetLength();
        record.DataSize = size;
        return;
    }
    Solid = false;
    if (alignedSize > allocated) {
        record.DataOffset = FindEmptyBlockOffset(alignedSize);
        BlockOrder.clear();
    }
    record.DataSize = size;
}
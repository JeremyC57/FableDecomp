// CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream& stream, unsigned long& value) -- binary persist read of one 4-byte value through the inlined, range-checked stream read
// Leaf: ?TransferBinaryIn@?$CPersistTraits@K@@QAEXAAVCMemoryDataInputStream@@AAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F8A0, 129 bytes, thiscall (traits `this` unused), `ret 8`.
// Identity (proven): reloc_bindings binds ?TransferBinaryIn@?$CPersistTraits@K@@QAEXAAVCMemoryDataInputStream@@AAK@Z here; its caller is CPersistContext::Transfer<unsigned long> 0x004106F0 (bound ?Transfer@CPersistContext@@QAEXPBDAAKABK@Z), whose long twin 0x00410BE0 calls the landed CPersistTraits<long> reader 0x0040FE60 instead.
// Callees: CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) 0x00993CA0 when the chunk runs out.
// Shape (family K077, representative 0x0040F8A0): the landed CPersistTraits<long>::TransferBinaryIn 0x0040FE60 verbatim as the
//   template member: a local filled by the inline Read (64-bit position check through an inline bool predicate, memcpy
//   from the chunk pointer +0xC, chunk length +0x14, position += size) and copied to the output.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
class CMemoryDataInputStream {
public:
    void* m_pVtable;                       // +0x00
    unsigned long m_position;              // +0x04
    long StreamSize;                       // +0x08
    const unsigned char* m_pData;          // +0x0C current chunk read pointer
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
template <class T> class CPersistTraits {
public:
    T DefaultVal;                                      // Ego_r.pdb (not touched here)
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value)
    {
        T number;
        stream.Read(&number, sizeof(number));
        value = number;
    }
};
template void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream&, unsigned long&);
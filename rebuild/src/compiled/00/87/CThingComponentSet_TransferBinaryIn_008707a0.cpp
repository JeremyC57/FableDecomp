// CThingComponentSet::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the entry count, resize the entry vector to it (new entries default), read the raw 9-byte entries over it and skip the end marker
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008707A0, length 327 bytes (`ret 4`). The inverse of the landed
//   CThingComponentSet::TransferBinaryOut (0x008708F0: count, raw entries, end marker 0x01AB6CCE).
// Callees: CSmallVector<CThingComponentSet::CEntry, 8>::resize(long, const CEntry&) @ 0x00870370 (as landed 0x00870480;
//   manifest label names the CANavQuadTreeNode instantiation, propagated), CDataInputStream::ReadWithSrcChunkOverflow
//   @ 0x00993CA0 (when the current chunk runs out).
// Layout: Entries.Start +0, Entries.Size (signed char) +4, CEntry 9 bytes packed (Ego_r.pdb, as the landed siblings);
//   CMemoryDataInputStream position +4, chunk read pointer +0xC, bytes left in the chunk +0x14 (as landed 0x007AD7F0).
// Shape: the inlined stream Read (64-bit position range check, inline memcpy or the overflow call) three times; the count
//   is homed in the dead stream-argument slot, the end marker's value is never used (its copy is dropped); the byte count
//   of the entry read is tested `> 0` first; the chunk test is written `size <= m_remaining` (`cmp size,[+14h] ; jg`).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

#pragma pack(push, 1)
class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CEntry {                         // CThingComponentSet::CEntry, 9 bytes (PDB: packed)
    CDefString Name;                    // +0x00
    unsigned long DriverType;           // +0x04
    bool Override;                      // +0x08
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, sizeof(value));
        return value;
    }
};

template<class T, int N> class CSmallVector { public:
    T* Start;                           // +0x00
    signed char Size;                   // +0x04
    void resize(long n, const T& fill);     // 0x00870370
    T* GetData() { return Size > 0 ? Start : (T*)this; }
};

class CThingComponentSet { public:
    CSmallVector<CEntry, 8> Entries;    // +0x00
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CThingComponentSet::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    long count = stream->ReadLong();
    Entries.resize(count, CEntry());
    CEntry* data = Entries.GetData();
    long bytes = Entries.Size * sizeof(CEntry);
    if (bytes > 0)
        stream->Read(data, bytes);
    stream->ReadLong();
}
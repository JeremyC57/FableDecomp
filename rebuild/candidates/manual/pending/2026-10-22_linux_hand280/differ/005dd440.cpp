// CAnimationSet::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the animation count, resize the animation map to it, read each (key, CAnimationEntry) pair, then the default flags / transition time / delay and the default group name
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DD440, length 545 bytes (ret 4).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the slow path of the inlined 4-byte reads, as the
//          landed CPersistTraits<long>::TransferBinaryIn 0x0040FE60), vector<pair<unsigned long, CAnimationEntry> >::resize
//          @ 0x005E39E0 (manifest "vector<CTCInventoryMap::CUIMiniMapMarker>::resize" is a propagated byte twin; 20-byte
//          elements), CAnimationEntry::TransferBinaryIn @ 0x005DD090, CDefString::TransferBinaryIn @ 0x0044FC00.
// Layout: Ego_r.pdb CAnimationSet (36 bytes PDB, 32 retail: the CVectorMap's STLport vector is 12 bytes, so DefaultFlags
//         +0x10, DefaultTransitionTime +0x14, DefaultDelay +0x18, DefaultGroup +0x1c); CAnimationEntry {BankIndex, AnimName,
//         GroupName, Components} 16 bytes (CDefString default -1, CIVCountedPointer default 0, BankIndex uninitialised).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
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
template <class T> __forceinline void PersistTransferBinaryIn(CMemoryDataInputStream* stream, T* out)
{
    T value;
    stream->Read(&value, sizeof(value));
    *out = value;
}

template <class T> __forceinline T PersistRead(CMemoryDataInputStream* stream)
{
    T value;
    stream->Read(&value, sizeof(value));
    return value;
}

struct CDefString {
    long Index;                            // -1 = none
    CDefString() : Index(-1) { }
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};
struct CAnimComponentMap {
    virtual void v00();
    virtual void DeleteThis();             // slot 1
    long RefCount;                         // +4
};
struct CIVCountedPointerAnimComponentMap {
    CAnimComponentMap* P;
    CIVCountedPointerAnimComponentMap() : P(0) { }
    __forceinline ~CIVCountedPointerAnimComponentMap()
    {
        CAnimComponentMap* p = P;
        if (p) {
            if (--p->RefCount == 0)
                p->DeleteThis();
        }
    }
};
struct CAnimationEntry {                   // 16 bytes
    long BankIndex;                        // +0x00
    CDefString AnimName;                   // +0x04
    CDefString GroupName;                  // +0x08
    CIVCountedPointerAnimComponentMap Components;   // +0x0c
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x005DD090
};
struct CAnimPair {                         // pair<unsigned long, CAnimationEntry>, 20 bytes
    unsigned long first;
    CAnimationEntry second;
    CAnimPair() : first(0), second() { }
};
struct CAnimVector {                       // STLport vector, 12 bytes
    CAnimPair* _M_start;
    CAnimPair* _M_finish;
    CAnimPair* _M_end_of_storage;
    void resize(unsigned int n, const CAnimPair& x);   // 0x005E39E0
};
struct CAnimationSet {
    CAnimVector Anims;                     // +0x00 (CVectorMap data)
    long _vectorMapTail;                   // +0x0c
    long DefaultFlags;                     // +0x10
    long DefaultTransitionTime;            // +0x14
    long DefaultDelay;                     // +0x18
    CDefString DefaultGroup;               // +0x1c
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CAnimationSet::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    unsigned long count = PersistRead<unsigned long>(stream);
    Anims.resize(count, CAnimPair());
    for (unsigned long i = 0; i < count; ++i) {
        Anims._M_start[i].first = PersistRead<unsigned long>(stream);
        Anims._M_start[i].second.TransferBinaryIn(stream);
    }
    PersistTransferBinaryIn(stream, &DefaultFlags);
    PersistTransferBinaryIn(stream, &DefaultTransitionTime);
    PersistTransferBinaryIn(stream, &DefaultDelay);
    DefaultGroup.TransferBinaryIn(stream);
}

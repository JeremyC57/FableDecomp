// Grade: DIFFER(716v716) -- one SIB operand order: retail lea ecx,[eax+ecx+4] (CDefString address), ours lea ecx,[ecx+eax+4]
// CSoundMap::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the sound-key/range records and the criteria-key/string records back into both vector maps, re-sorting each if dirty
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047042A, 716 bytes, `ret 4`, size-optimised def TU (#pragma optimize("s")).
// Mirror of the landed CSoundMap::TransferBinaryOut 0x0047034B.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of each inlined Read),
//   CArray<pair<ulong,CSoundPair>>::resize(n, const T&) 0x004706F6 (manifest "push_back", wrong; landed as resize),
//   CArray<pair<ulong,CDefString>>::resize(n, const T&) 0x00470A28 (manifest "push_back", wrong),
//   CDefString::TransferBinaryIn(CMemoryDataInputStream*) 0x0044FC00, the maps' Resort (sort + clear Dirty)
//   0x00470CD8 / 0x0047119A (manifest "CGuiVarTransferBool::TransferToGui", propagated and wrong).
// Layout: Ego_r.pdb CSoundMap { SoundMap +0, CriteriaMap +0x14 } with the retail 16-byte CVectorMap (12-byte vector,
//   comparator +0xC, Dirty +0xD), so CriteriaMap sits at +0x10.
// Shape: each map is read by an inline per-map TransferBinaryIn(stream, count) after the caller reads the count
//   (retail forms &CriteriaMap only at the resize call, after the count read); per-entry data via a specialised
//   ReadData. Every value goes through an inlined __forceinline ReadX() (memcpy -> movsd under /Os); the resize defaults are
//   pair temporaries: the sound pair's CSoundPair has an empty ctor (only the key is zeroed), CDefString() is -1.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
#pragma optimize("s", on)

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
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long v; Read(&v, 4); return v; }
    __forceinline long ReadLong() { long v; Read(&v, 4); return v; }
};

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() { }
};

struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

template <class Value> struct CMapEntry {                    // pair<unsigned long, Value>
    unsigned long Key;
    Value Data;
    CMapEntry() : Key(0), Data() { }
};
template <class Value> struct CSoundVectorMap {              // CVectorMap<unsigned long, Value, CKeyPairCompareLess<...>>
    CMapEntry<Value>* Start;
    CMapEntry<Value>* Finish;
    CMapEntry<Value>* End;
    unsigned char Comparator;                                // +0xC
    bool Dirty;                                              // +0xD
    unsigned long size() const { return Finish - Start; }
    CMapEntry<Value>& operator[](unsigned long index) { return Start[index]; }
    void resize(unsigned long count, const CMapEntry<Value>& value);   // 0x004706F6 / 0x00470A28
    void Resort();                                                     // 0x00470CD8 / 0x0047119A
    void ReadData(CMemoryDataInputStream* stream, unsigned long i);
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, unsigned long count)
    {
        resize(count, CMapEntry<Value>());
        for (unsigned long i = 0; i < size(); ++i) {
            (*this)[i].Key = stream->ReadULong();
            ReadData(stream, i);
        }
        if (Dirty)
            Resort();
    }
};

template <> __forceinline void CSoundVectorMap<CSoundPair>::ReadData(CMemoryDataInputStream* stream, unsigned long i)
{
    (*this)[i].Data.FirstSound = stream->ReadLong();
    (*this)[i].Data.LastSound = stream->ReadLong();
}
template <> __forceinline void CSoundVectorMap<CDefString>::ReadData(CMemoryDataInputStream* stream, unsigned long i)
{
    (*this)[i].Data.TransferBinaryIn(stream);
}

struct CSoundMap {
    CSoundVectorMap<CSoundPair> SoundMap;                    // +0x00
    CSoundVectorMap<CDefString> CriteriaMap;                 // +0x10
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CSoundMap::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    unsigned long count = stream->ReadULong();
    SoundMap.TransferBinaryIn(stream, count);
    count = stream->ReadULong();
    CriteriaMap.TransferBinaryIn(stream, count);
}

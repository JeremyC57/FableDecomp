// CWoundMorphs::TransferBinaryIn(CMemoryDataInputStream* stream) -- reads the entry count, resizes the morph map's vector, reads all the (location, entry) pairs as one block, re-sorts the map if it was dirty, then skips a trailing 4-byte field
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757990, 402 bytes (+ int3 padding), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, vector<pair<EBodyLocation, CEntry>>::resize(n,
//   const T&) 0x007595F0 (manifest label vector<CVectorMap<...>>::resize, propagated), the map's sort 0x007597C0
//   (`__fastcall` first/last, comparator by value; manifest "CSoundPairHeap_MakeHeap").
// Layout: Ego_r.pdb CWoundMorphs { CVectorMap<EBodyLocation, CEntry> Morphs } -- the PDB vector is 16 bytes (Compare
//   +0x10, Dirty +0x11), retail STLport's is 12 (Compare +0xC, Dirty +0xD), as the landed CSkeletalMorphs::
//   TransferBinaryIn 0x0081AED0; pair<EBodyLocation, CEntry> is 0x2C bytes (CEntry: 9 longs + float, as the landed
//   CTCWound::AddWound 0x00757B30).
// Shape: the trailing 4-byte field is read back into `count` (retail reads both into the dead stream-arg slot, frame
//   0x30); CEntry's user ctor zeroes the longs and sets the float to 0.0f (the float is the one `mov [],0` immediate,
//   the rest share the ebx zero); the inline Read tests `size <= m_remaining` (`cmp eax,[ebp+0x14]; jg`).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
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
};

enum EBodyLocation { BODY_LOCATION_NONE };

struct CWoundMorphs {
    struct CEntry {
        EBodyLocation Location;
        long          ReplacingTextureIndex;
        long          ReplacingBumpMapIndex;
        long          WoundTextureIndex;
        long          WoundBumpMapIndex;
        long          ScarTextureIndex;
        long          ScarBumpMapIndex;
        long          FadeToScarDuration;
        long          TotalDuration;
        float         PermanentScarStrength;
        CEntry() : Location(BODY_LOCATION_NONE), ReplacingTextureIndex(0), ReplacingBumpMapIndex(0), WoundTextureIndex(0),
            WoundBumpMapIndex(0), ScarTextureIndex(0), ScarBumpMapIndex(0), FadeToScarDuration(0), TotalDuration(0),
            PermanentScarStrength(0.0f) { }
    };
    struct CEntryPair {                                   // pair<EBodyLocation, CEntry>, 0x2C bytes
        EBodyLocation first;
        CEntry        second;
        CEntryPair() : first(EBodyLocation()), second(CEntry()) { }
    };
    struct CCompare { };                                  // CKeyPairCompareLess<EBodyLocation, CEntry>
    struct CEntryVector {
        CEntryPair* _M_start;
        CEntryPair* _M_finish;
        CEntryPair* _M_end_of_storage;
        unsigned long size() const { return _M_finish - _M_start; }
        CEntryPair* GetData() { return size() != 0 ? _M_start : (CEntryPair*)this; }
        void resize(unsigned long count, const CEntryPair& value);   // 0x007595F0
    };
    CEntryVector Morphs;                                  // +0x00
    CCompare     Compare;                                 // +0x0C
    bool         Dirty;                                   // +0x0D
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};
void __fastcall SortEntries(CWoundMorphs::CEntryPair* first, CWoundMorphs::CEntryPair* last, CWoundMorphs::CCompare compare);   // 0x007597C0

void CWoundMorphs::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    unsigned long count;
    stream->Read(&count, sizeof(count));
    Morphs.resize(count, CEntryPair());
    CEntryPair* data = Morphs.GetData();
    long bytes = Morphs.size() * sizeof(CEntryPair);
    if (bytes > 0)
        stream->Read(data, bytes);
    if (Dirty) {
        SortEntries(Morphs._M_start, Morphs._M_finish, Compare);
        Dirty = false;
    }
    stream->Read(&count, sizeof(count));
}
// CSkeletalMorphs::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the morph count, resize the morph map's vector, then read each (key, name, index, enabled) entry and mark the map clean
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081AED0, length 452 bytes (__fastcall, ret 4).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, vector<pair<ulong,CEntry>>::resize(n, const T&)
// @ 0x0081D9F0 (manifest label vector<CBrainBehaviour>::resize, propagated), CDefString::TransferBinaryIn @ 0x0044FC00
// (manifest label CPersistTraits<float>::TransferBinaryIn, propagated; as the landed 0x00815620).
// Layout: Ego_r.pdb CSkeletalMorphs { CVectorMap<ulong, CEntry> Morphs } -- the PDB view of the vector is 16 bytes
// (Compare +0x10, Dirty +0x11) but retail STLport's is 12, so Dirty is the byte at +0xd (`mov byte [ebp+0xd],0`).
// pair<ulong,CEntry> is 12 bytes (`add ebx,0xc`): key +0, CEntry { CDefString Name +4, short +8, bool +0xa };
// the resize fill value is pair() = {0, CDefString() TablePos -1} built in the frame.
// Shape: inlined CMemoryDataInputStream::Read (as landed 0x00815620). The count and each key come back by value
// from a __forceinline ReadULong (count lives in edi; the fast path loads straight into the register), the short
// and the bool byte through address-taken locals; the loop is strength-reduced (byte offset in ebx, a
// count-down in the frame).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
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
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
};

class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) {}
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

struct CEntry {
    CDefString Name;          // +0
    short m_index;            // +4
    bool m_enabled;           // +6
};
struct CMorphPair {
    unsigned long first;      // +0
    CEntry second;            // +4
    CMorphPair() : first(0) {}
};
struct CMorphVector {
    CMorphPair* _M_start;
    CMorphPair* _M_finish;
    CMorphPair* _M_end_of_storage;
    void resize(unsigned long count, const CMorphPair& value);   // 0x0081D9F0
};
class CSkeletalMorphs { public:
    CMorphVector Morphs;      // +0
    bool Compare;             // +0xc
    bool Dirty;               // +0xd
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CSkeletalMorphs::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    unsigned long count = stream->ReadULong();
    Morphs.resize(count, CMorphPair());
    for (unsigned long i = 0; i < count; ++i) {
        Morphs._M_start[i].first = stream->ReadULong();
        CEntry& entry = Morphs._M_start[i].second;
        entry.Name.TransferBinaryIn(stream);
        short index;
        stream->Read(&index, sizeof(index));
        entry.m_index = index;
        unsigned char enabled;
        stream->Read(&enabled, sizeof(enabled));
        entry.m_enabled = enabled != 0;
    }
    Dirty = false;
}
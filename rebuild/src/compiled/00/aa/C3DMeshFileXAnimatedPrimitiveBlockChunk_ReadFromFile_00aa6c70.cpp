// C3DMeshFileXAnimatedPrimitiveBlockChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the group list, BonesPerVertex, the vertex list and every bone weight, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA6C70, length 928 bytes (virtual, chunk reader slot, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2) x2, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//          CVertexGroupBlock::AddGroup @ 0x00AD91F0, memmove @ 0x00BFEAE6, vector<unsigned long>::_M_fill_insert
//          @ 0x004649A3, vector<C3DIndexedBoneWeight>::_M_fill_insert @ 0x00A97180, _ftol2 @ 0x00BFEA70,
//          CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830.
// Shape: the mirror of the landed WriteToFile 0x00AA5720. start = GetPosition(); a counted loop of AddGroup(ReadULong());
// BonesPerVertex; the vertex count; VertexList.resize(count) (inline STLport resize with its by-value zero fill in
// the dead stream-argument slot: erase through the trivial memmove copy, else _M_fill_insert) and one bulk Read of
// count*4 bytes into GetData(); BoneWeights.resize(BonesPerVertex * count) (non-trivial element copy loop on erase);
// then per weight record the bone index and the weight read as dwords, the weight stored unsigned -> float -> long.
// Layout: retail CArray is 12 bytes (PDB 16): GroupBlock +0x2c, BonesPerVertex +0x44, VertexList +0x48,
// BoneWeights +0x54, as in the landed WriteToFile 0x00AA5720 and dtor 0x00AA7480; no engine header.
// Leaf: ReadFromFile@C3DMeshFileXAnimatedPrimitiveBlockChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
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
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
};

struct C3DIndexedBoneWeight {
    unsigned long BoneIndex;               // +0
    unsigned long Weight;                  // +4
    C3DIndexedBoneWeight() {}              // the resize fill value is left uninitialised
};
struct CULongVector {                      // vector<unsigned long> (trivial element: memmove erase)
    unsigned long* _M_start; unsigned long* _M_finish; unsigned long* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long* GetData() { return size() > 0 ? _M_start : (unsigned long*)this; }
    void _M_fill_insert(unsigned long* pos, unsigned long n, const unsigned long& x);   // 0x004649A3
    void erase(unsigned long* first, unsigned long* last)
    {
        unsigned long* finish = _M_finish;
        unsigned long* result = (last == finish) ? first
            : (unsigned long*)((char*)memmove(first, last, (char*)finish - (char*)last) + ((char*)finish - (char*)last));
        _M_finish = result;
    }
    void resize(unsigned long newSize, unsigned long x = 0)
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), x);
    }
};
struct CBoneWeightVector {                 // vector<C3DIndexedBoneWeight> (element copy loop on erase)
    C3DIndexedBoneWeight* _M_start; C3DIndexedBoneWeight* _M_finish; C3DIndexedBoneWeight* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    C3DIndexedBoneWeight& operator[](unsigned long n) { return *(_M_start + n); }
    void _M_fill_insert(C3DIndexedBoneWeight* pos, unsigned long n, const C3DIndexedBoneWeight& x);   // 0x00A97180
    void erase(C3DIndexedBoneWeight* first, C3DIndexedBoneWeight* last)
    {
        C3DIndexedBoneWeight* result = first;
        for (int n = (int)(_M_finish - last); n > 0; --n) {
            *result = *last;
            ++last;
            ++result;
        }
        _M_finish = result;
    }
    void resize(unsigned long newSize, const C3DIndexedBoneWeight& x)
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), x);
    }
};
struct CVertexGroupBlock {                 // retail 0x18 bytes
    CULongVector Groups;                   // +0x00
    unsigned char GroupToIndexMap[0x0c];   // +0x0c STLport map
    void AddGroup(unsigned long group);    // 0x00AD91F0
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};
struct C3DMeshFileXAnimatedPrimitiveBlockChunk : CChunkedFileSuperChunk {
    CVertexGroupBlock GroupBlock;          // +0x2c
    unsigned long BonesPerVertex;          // +0x44
    CULongVector VertexList;               // +0x48
    CBoneWeightVector BoneWeights;         // +0x54
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileXAnimatedPrimitiveBlockChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    unsigned long groups = in.ReadULong();
    for (unsigned long i = 0; i < groups; ++i)
        GroupBlock.AddGroup(in.ReadULong());
    BonesPerVertex = in.ReadULong();
    unsigned long vertices = in.ReadULong();
    VertexList.resize(vertices);
    in.Read(VertexList.GetData(), vertices * sizeof(unsigned long));
    unsigned long weights = BonesPerVertex * vertices;
    BoneWeights.resize(weights, C3DIndexedBoneWeight());
    for (unsigned long k = 0; k < weights; ++k) {
        unsigned long bone = in.ReadULong();
        unsigned long weight = in.ReadULong();
        BoneWeights[k].BoneIndex = bone;
        BoneWeights[k].Weight = (long)(float)weight;
    }
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
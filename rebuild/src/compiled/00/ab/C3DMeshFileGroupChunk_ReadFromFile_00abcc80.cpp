// C3DMeshFileGroupChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the bone index and the (vertex, weight) blend list, merging repeated vertices
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABCC80, length 560 bytes (virtual, chunk reader slot, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2) x2, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//          vector<C3DVertexBlend2>::_M_insert_overflow @ 0x00ABEB40 (push_back growth path),
//          CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830 (the base reader, given the bytes left).
// Shape: start = GetPosition(); BoneIndex; the list is cleared with the inline STLport erase(begin, end) (copy of
// the empty tail, then finish = first); count; per entry a ushort vertex index and a float weight -- when the
// list's last entry has the same vertex its weight is accumulated, otherwise the blend is push_back'ed
// (placement copy with the null test, or _M_insert_overflow(pos, x, __true_type, 1, true)); then the base reader
// with chunkSize - (GetPosition() - start).
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (PDB 0x34, landed 0x00AA9BA0 / 0x00ABA170 model), so
// BoneIndex +0x2c and VertexList +0x30 as a 12-byte STLport vector (PDB header +0x34/+0x38 with a 16-byte CArray);
// the engine header is not used.
// Leaf: ReadFromFile@C3DMeshFileGroupChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
inline void* operator new(unsigned int, void* where) { return where; }

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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned short ReadUShort() { unsigned short value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

struct C3DVertexBlend2 {
    long VertexIndex;
    float Weight;
};
struct __true_type {};
struct CVertexBlendVector {
    C3DVertexBlend2* _M_start;
    C3DVertexBlend2* _M_finish;
    C3DVertexBlend2* _M_end_of_storage;
    void _M_insert_overflow(C3DVertexBlend2* pos, const C3DVertexBlend2& x, const __true_type&, unsigned int fillLen, bool atEnd);  // 0x00ABEB40
    bool empty() const { return _M_start == _M_finish; }
    C3DVertexBlend2& back() { return *(_M_finish - 1); }
    void erase(C3DVertexBlend2* first, C3DVertexBlend2* last)
    {
        C3DVertexBlend2* dest = first;
        for (int n = (int)(_M_finish - last); n > 0; --n) {
            *dest = *last;
            ++last;
            ++dest;
        }
        _M_finish = dest;
    }
    void clear() { erase(_M_start, _M_finish); }
    void push_back(const C3DVertexBlend2& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DVertexBlend2(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};
struct C3DMeshFileGroupChunk : CChunkedFileSuperChunk {
    long BoneIndex;                        // +0x2c (PDB +0x34)
    CVertexBlendVector VertexList;         // +0x30
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

#pragma auto_inline(off)
// Same-TU stand-in for the growth path (reads the value only through its const reference, never the tag).
void CVertexBlendVector::_M_insert_overflow(C3DVertexBlend2* pos, const C3DVertexBlend2& x, const __true_type&, unsigned int fillLen, bool atEnd)
{
    if (*(void**)&x != pos)
        return;
}
#pragma auto_inline(on)

void C3DMeshFileGroupChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    BoneIndex = in.ReadLong();
    VertexList.clear();
    unsigned long count = in.ReadULong();
    for (unsigned long i = 0; i < count; ++i) {
        C3DVertexBlend2 blend;
        blend.VertexIndex = in.ReadUShort();
        blend.Weight = in.ReadFloat();
        if (!VertexList.empty() && VertexList.back().VertexIndex == blend.VertexIndex)
            VertexList.back().Weight += blend.Weight;
        else
            VertexList.push_back(blend);
    }
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
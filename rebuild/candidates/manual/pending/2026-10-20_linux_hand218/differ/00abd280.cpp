// C3DMeshFileTrianglesChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the triangle count and the raw triangle array, then the super chunk's sub chunks
// Grade: DIFFER(ours v retail) -- frame only: retail sub esp,8 passes the resize default T() temp at the same frame word as the start position (lea edx,[esp+10h]); ours opens 0xC bytes with the temp at its own offset (lea edx,[esp+16h]); everything else identical
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABD280, 308 bytes (virtual, `ret 0Ch`).
// Callees: CDataInputStream::GetPosition (vtable slot 2), ReadWithSrcChunkOverflow 0x00993CA0 (the inlined stream Read's
// slow path), vector<C3DTriangle2>::resize(n, const T&) 0x00A97A50 (manifest "vector<CBrainBehaviour>::resize" is a
// propagated byte twin), CChunkedFileSuperChunk::ReadFromFile 0x00AA8830.
// Shape: start position, a by-value ReadULong() count (homed in the dead `in` slot), TriangleList.resize(count) through
// the one-argument inline `resize(n) { resize(n, T()); }`, then one inlined stream Read of size() * 6 bytes into the
// array data (`size() ? _M_start : (T*)this`), the variable-length memcpy intrinsic (`rep movsd ; rep movsb`) on the
// fast path; the rest of the chunk goes to the base reader (`chunkSize - (GetPosition() - start)`).
// Layout: Ego_r.pdb TriangleList +0x34; retail +0x2C (the super chunk base 8 bytes smaller: 12-byte containers), so a
// local model; C3DTriangle2 is three unsigned shorts.
// Leaf: ReadFromFile@C3DMeshFileTrianglesChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
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

struct C3DTriangle2 {
    unsigned short Index[3];
    C3DTriangle2() {}
};

template <class T> struct CArray {                 // retail STLport vector layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void resize(unsigned long n, const T& x);      // 0x00A97A50
    void resize(unsigned long n) { resize(n, T()); }
    T* GetData() { return size() ? _M_start : (T*)this; }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _body[0x28];             // CChunkedFileChunk (0x10) + the sub chunk containers (retail 0x2C total)
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

struct C3DMeshFileTrianglesChunk : CChunkedFileSuperChunk {
    CArray<C3DTriangle2> TriangleList;     // +0x2C
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileTrianglesChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    unsigned long count = in.ReadULong();
    TriangleList.resize(count);
    in.Read(TriangleList.GetData(), TriangleList.size() * sizeof(C3DTriangle2));
    unsigned long used = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - used, loader);
}

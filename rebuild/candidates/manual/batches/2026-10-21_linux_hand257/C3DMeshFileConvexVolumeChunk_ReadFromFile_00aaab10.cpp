// C3DMeshFileConvexVolumeChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- skip a version word, read Name, resize Planes to the stored count and read each plane
// Leaf: ReadFromFile@C3DMeshFileConvexVolumeChunk
// Retail @ 0x00AAAB10, 448 bytes (virtual, slot 7, ret 0Ch).
// Callees: CDataInputStream::ReadNullTerminatedString (vtable slot 6, hidden CCharString result),
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CDataInputStream::Read3DVector 0x009950F0 (C3DVector by
//   hidden pointer), CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, and the CArray<CPlane> (STLport
//   vector) members erase(first, last) 0x00AAB500 and insert(pos, n, const CPlane&) 0x00AAB570.
// Shape: the first 4-byte read is a by-value ReadULong() whose result is dropped (only the stream advance stays);
// the count is the STLport resize(n, CPlane()) inlined (erase the tail when shrinking, else insert the default
// plane at end()); then one Read3DVector + ReadFloat per plane. No sub-chunk call: the chunk is a leaf.
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (Ego_r.pdb 0x34): Name +0x2c, Planes +0x30 (PDB +8 each).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct C3DVector { float X, Y, Z; };
struct CPlane {
    C3DVector Normal;
    float Distance;
    CPlane() {}
};

struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
    C3DVector Read3DVector();                             // 0x009950F0 (by value through the hidden pointer)
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
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

// CArray<CPlane>: an STLport vector (begin, end, end-of-storage).
struct CPlaneArray {
    CPlane* _M_start;
    CPlane* _M_finish;
    CPlane* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CPlane* begin() { return _M_start; }
    CPlane* end() { return _M_finish; }
    CPlane& operator[](unsigned long n) { return _M_start[n]; }
    CPlane* erase(CPlane* first, CPlane* last);                      // 0x00AAB500
    void insert(CPlane* position, unsigned long n, const CPlane& x); // 0x00AAB570
    void resize(unsigned long newSize, const CPlane& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    void resize(unsigned long newSize) { resize(newSize, CPlane()); }
};

struct C3DMeshFileChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
};

struct C3DMeshFileConvexVolumeChunk : C3DMeshFileChunk {
    CCharString Name;                      // +0x2c (PDB +0x34)
    CPlaneArray Planes;                    // +0x30
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileConvexVolumeChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    in.ReadULong();
    Name = in.ReadNullTerminatedString();
    unsigned long count = in.ReadULong();
    Planes.resize(count);
    for (unsigned long i = 0; i < count; ++i) {
        CPlane& plane = Planes[i];
        plane.Normal = in.Read3DVector();
        plane.Distance = in.ReadFloat();
    }
}

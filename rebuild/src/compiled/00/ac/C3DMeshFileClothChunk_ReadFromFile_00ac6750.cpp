// C3DMeshFileClothChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the cloth name, material, particle code text and optional compiled code bytes, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC6750, length 583 bytes (virtual, chunk reader slot, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2) x2, CDataInputStream::ReadNullTerminatedString (vtable slot 6,
//          hidden CCharString result) x2, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
//          CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, vector<char>::resize(n, const char&) @ 0x004122D0,
//          CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830.
// Shape: start = GetPosition(); an unused version dword; Name; MaterialIndex; ParticleCode; a one-byte "has
// compiled code" flag -- when set, a byte count, CompiledCode resized to it (zero fill char) and one bulk Read
// into GetData() (the array itself when empty); then the base reader with chunkSize - (GetPosition() - start).
// Every helper temporary reuses the dead stream-argument slot.
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (PDB 0x34), so Name +0x2c, MaterialIndex +0x30,
// ParticleCode +0x34, CompiledCode +0x38 (12-byte STLport vector; PDB header has +0x34..+0x40 and a 16-byte CArray);
// the engine header is not used.
// Leaf: ReadFromFile@C3DMeshFileClothChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
template<class T> struct CArray {
    T* Begin; T* End; T* Capacity;
    unsigned long Size() const { return (unsigned long)(End - Begin); }
    T* GetData() { return Size() > 0 ? Begin : (T*)this; }
    void Resize(unsigned long, const T&);                 // 0x004122D0
};

struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { char value; Read(&value, sizeof(value)); return value != 0; }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};
struct C3DMeshFileClothChunk : CChunkedFileSuperChunk {
    CCharString Name;                      // +0x2c (PDB +0x34)
    long MaterialIndex;                    // +0x30
    CCharString ParticleCode;              // +0x34
    CArray<char> CompiledCode;             // +0x38
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileClothChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    in.ReadULong();                        // version (unused)
    Name = in.ReadNullTerminatedString();
    MaterialIndex = in.ReadLong();
    ParticleCode = in.ReadNullTerminatedString();
    if (in.ReadBool()) {
        unsigned long size = in.ReadULong();
        CompiledCode.Resize(size, 0);
        in.Read(CompiledCode.GetData(), size);
    }
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
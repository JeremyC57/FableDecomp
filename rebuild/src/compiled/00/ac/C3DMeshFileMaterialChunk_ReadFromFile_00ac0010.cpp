// C3DMeshFileMaterialChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the material name, index, flags, colours and shininess, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC0010, length 576 bytes (virtual, chunk reader slot, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2) x2, CDataInputStream::ReadNullTerminatedString (vtable slot 6,
//          hidden CCharString result), CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
//          CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CDataInputStream::ReadColour @ 0x009945E0 (x3,
//          CRGBColour by value through the hidden pointer), CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830.
// Shape: start = GetPosition(); the name first (its temporary reuses the dead stream-argument slot, like every
// following by-value helper local); MaterialIndex; a one-byte TwoSided bool; Ambient/Diffuse/Specular colours;
// Shiny/ShinyStrength/Transparency floats; then the base reader with chunkSize - (GetPosition() - start).
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (PDB 0x34, landed 0x00AA9BA0 / 0x00ABA170 model), so every
// member sits 8 bytes below the PDB header (MaterialIndex +0x2c .. Name +0x4c); the engine header is not used.
// Leaf: ReadFromFile@C3DMeshFileMaterialChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CChunkedFileLoader;
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
class CRGBColour { public:
    unsigned long m_value;                 // packed B, G, R, A bytes (dword aligned: Ambient sits at +0x34)
    CRGBColour() {}
    CRGBColour(const CRGBColour& other) : m_value(other.m_value) {}
};

class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
    CRGBColour ReadColour();                              // 0x009945E0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
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
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { char value; Read(&value, sizeof(value)); return value != 0; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

class CChunkedFileSuperChunk { public:
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};
class C3DMeshFileMaterialChunk : public CChunkedFileSuperChunk { public:
    long MaterialIndex;                    // +0x2c (PDB +0x34)
    bool TwoSided;                         // +0x30
    CRGBColour Ambient;                    // +0x34
    CRGBColour Diffuse;                    // +0x38
    CRGBColour Specular;                   // +0x3c
    float Shiny;                           // +0x40
    float ShinyStrength;                   // +0x44
    float Transparency;                    // +0x48
    CCharString Name;                      // +0x4c
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileMaterialChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    Name = in.ReadNullTerminatedString();
    MaterialIndex = in.ReadLong();
    TwoSided = in.ReadBool();
    Ambient = in.ReadColour();
    Diffuse = in.ReadColour();
    Specular = in.ReadColour();
    Shiny = in.ReadFloat();
    ShinyStrength = in.ReadFloat();
    Transparency = in.ReadFloat();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
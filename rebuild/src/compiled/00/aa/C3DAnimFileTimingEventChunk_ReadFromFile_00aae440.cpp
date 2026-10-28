// C3DAnimFileTimingEventChunk::ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader) -- the event name, then the time
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAE440, 144 bytes. Callees: the stream's virtual
// ReadNullTerminatedString (slot 6, +0x18; CCharString returned through a hidden pointer that VC homes in the
// dead `in` argument slot), CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0,
// CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0.
// Layout as the landed C3DAnimFileTimingEventChunk::WriteToFile (0x00AAE220): Name +0x10, Time +0x14.
// The float read is the inlined stream Read(buffer, 4) of the landed TransferBinaryIn family (64-bit
// position range check, direct copy when the chunk holds 4 more bytes, else the overflow reader) through an
// inline ReadFLOAT() returning a local (homed in the same dead slot); the copy into a scalar local is a
// dword move forwarded to the member store. Read/ReadFLOAT must be __forceinline (plain inline stays out of
// line), and the stream/loader are `class` so the mangled leaf (AAVCDataInputStream) matches the catalog.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);                // 0x0099EFB0
};

class CDataInputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual CCharString ReadNullTerminatedString();                  // slot 6 (+0x18)
    unsigned long m_position;              // +0x04
    long StreamSize;
    const unsigned char* m_pData;          // +0x0C current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
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
    __forceinline float ReadFLOAT()
    {
        float value;
        Read(&value, 4);
        return value;
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

class CChunkedFileLoader;

class C3DAnimFileTimingEventChunk { public:
    virtual ~C3DAnimFileTimingEventChunk();
    unsigned char _base_body[0x0C];                 // +0x04 .. +0x0F (CChunkedFileChunk: type, parent)
    CCharString Name;                               // +0x10
    float Time;                                     // +0x14
    virtual void ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader);
};

void C3DAnimFileTimingEventChunk::ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader)
{
    Name = in.ReadNullTerminatedString();
    Time = in.ReadFLOAT();
}
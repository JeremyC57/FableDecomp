// CPatchTesselationEdgeStrip::Load(CMemoryDataInputStream* stream, CMovableResourceHandler* handler) -- read StartPos and Size (words) and the Vertical flag from the stream, then load the four vertex arrays through the LoadVerts<T> member template; false as soon as one fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC600, length 360 bytes (+ int3 padding), `ret 8`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the slow path of the inline reads);
// LoadVerts<CVertex> 0x00BFC880, LoadVerts<CFanBaseVertex> 0x00BFCA20, LoadVerts<CWaterVertex> 0x00BFCBC0,
// LoadVerts<CWaterFanBaseVertex> 0x00BFCD60 (the four instantiations in member order, as the landed Save 0x00BFC770).
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, the landed Save/Reset model); the stream reader is the
// landed CMemoryDataInputStream inline Read (0x00632E00 family: position +4, chunk pointer +0xC, bytes left +0x14).
// Shape: each value is an inline by-value ReadUShort/ReadBool whose local shares one frame slot; the flag is
// `value != 0` (setne); the four loads are early-returning tests with the last one's bool as the result.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CMemoryDataInputStream { public:
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
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned short ReadUShort()
    {
        unsigned short value;
        Read(&value, sizeof(value));
        return value;
    }
    __forceinline bool ReadBool()
    {
        unsigned char value;
        Read(&value, sizeof(value));
        return value != 0;
    }
};

struct CMovableResourceHandler;

template <class T>
class CEdgeStipVertexArray { public:
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
};

class CPatchTesselationEdgeStrip { public:                // vtable 0x012A8318 (CBase-derived), 0x2c bytes
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    virtual void v00();
    unsigned short StartPos;                                            // +0x04
    unsigned short Size;                                                // +0x06
    CEdgeStipVertexArray<CVertex>             Verts;                    // +0x08
    CEdgeStipVertexArray<CFanBaseVertex>      FanBaseVerts;             // +0x10
    CEdgeStipVertexArray<CWaterVertex>        WaterVerts;               // +0x18
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;        // +0x20
    bool Tesselated;                                                    // +0x28
    bool Vertical;                                                      // +0x29
    template <class T> bool LoadVerts(CEdgeStipVertexArray<T>* verts, CMemoryDataInputStream* stream, CMovableResourceHandler* handler);   // 0x00BFC880..0x00BFCD60
    bool Load(CMemoryDataInputStream* stream, CMovableResourceHandler* handler);
};

bool CPatchTesselationEdgeStrip::Load(CMemoryDataInputStream* stream, CMovableResourceHandler* handler)
{
    StartPos = stream->ReadUShort();
    Size = stream->ReadUShort();
    Vertical = stream->ReadBool();
    if (!LoadVerts(&Verts, stream, handler))
        return false;
    if (!LoadVerts(&FanBaseVerts, stream, handler))
        return false;
    if (!LoadVerts(&WaterVerts, stream, handler))
        return false;
    if (!LoadVerts(&WaterFanBaseVerts, stream, handler))
        return false;
    return true;
}
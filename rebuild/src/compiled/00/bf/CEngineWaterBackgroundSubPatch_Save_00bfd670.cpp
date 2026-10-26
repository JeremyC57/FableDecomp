// CEngineWaterBackgroundSubPatch::Save(CDataOutputStream& stream, ...) -- write the sub-patch: vertex and poly counts and water type, then (for the water types that carry geometry) the range-compressed vertex buffer with its stride and size, and the compressed index buffer when there are polygons
// Leaf: Save
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD670, length 336 bytes (body + its 12-byte jump table and 8-byte
//   index table + int3 padding), `ret 14h` (four further arguments unused here; manifest signature).
// Callees: CDataOutputStream::WriteUShort @ 0x00993E80 and WriteLong @ 0x00993EB0 (manifest "WriteCBYTE" is propagated;
//   0x993EB0 is WriteLong per GOTCHAS), the stream's virtual Write(const void*, long) (slot 4, +0x10),
//   CVertexBufferWin32::Lock(ulong, ulong, ulong) @ 0x00AC20A0 / Unlock @ 0x00AC20B0, CIndexBuffer::Lock @ 0x00AC1FB0 /
//   Unlock @ 0x00AC1F00 (labels for 0xAC20A0/0xAC20B0/0xAC1F00 are propagated), CRangeCompressor::Compress(void*, long
//   count, long stride, CArray<unsigned char>*) @ 0x00F3AFC0, free @ 0x00BFEA14.
// Layout: engine/CEngineWaterBackgroundSubPatch.h (IndexBuffer +0x08, VertexBuffer +0x0c, PolyCount +0x30, VertexCount
//   +0x32, WaterType +0x40 as a 4-byte EWaterType blob read through an accessor); the method sits on a derived shell.
// Shape: the stride comes from a switch on WaterType compiled to a byte-indexed jump table (types 1,2,6,8 -> 0x38 bytes,
//   3,4,5 -> 0xc, anything else writes no geometry); the empty compressor object is homed in the dead `stream` slot; the
//   output buffer is a local STLport-style CArray<unsigned char> (GetData() = begin, or the array itself when empty).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
#include "engine/CEngineWaterBackgroundSubPatch.h"

class CDataOutputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size);    // slot 4 (+0x10)
    void WriteUShort(unsigned short value);              // 0x00993E80
    void WriteLong(long value);                          // 0x00993EB0
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    CArray() : First(0), Last(0), End(0) { }
    ~CArray() { if (First) free(First); }
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

class CRangeCompressor {
public:
    long Compress(void* data, long count, long stride, CArray<unsigned char>* output);   // 0x00F3AFC0
};

class CVertexBufferWin32 { public:
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC20A0
    void Unlock();                                       // 0x00AC20B0
};
class CIndexBuffer { public:
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                       // 0x00AC1F00
};

struct CEngineWaterBackgroundSubPatch_Methods : CEngineWaterBackgroundSubPatch {
    long GetWaterType() const { return *(const long*)WaterType; }
    void Save(CDataOutputStream& stream, unsigned short a, unsigned short b, long c, long d);
};

void CEngineWaterBackgroundSubPatch_Methods::Save(CDataOutputStream& stream, unsigned short a, unsigned short b, long c, long d)
{
    stream.WriteUShort(VertexCount);
    stream.WriteUShort(PolyCount);
    stream.WriteLong(GetWaterType());
    long stride;
    switch (GetWaterType()) {
    case 1:
    case 2:
    case 6:
    case 8:
        stride = 0x38;
        break;
    case 3:
    case 4:
    case 5:
        stride = 0xc;
        break;
    default:
        return;
    }
    CRangeCompressor compressor;
    CArray<unsigned char> buffer;
    void* vertices = VertexBuffer->Lock(0, 0, 0);
    long size = compressor.Compress(vertices, VertexCount, stride, &buffer);
    stream.WriteLong(stride);
    stream.WriteLong(size);
    stream.Write(buffer.GetData(), size);
    VertexBuffer->Unlock();
    if (PolyCount != 0) {
        void* indices = IndexBuffer->Lock(0, 0, 0);
        size = compressor.Compress(indices, PolyCount * 3, 2, &buffer);
        stream.WriteLong(size);
        stream.Write(buffer.GetData(), size);
        IndexBuffer->Unlock();
    }
}
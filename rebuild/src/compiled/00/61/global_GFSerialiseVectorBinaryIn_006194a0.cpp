// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CTCInventoryMap::CUIMiniMapMarker>& out) -- read the count, resize the vector, then read every mini-map marker in place
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006194A0, length 184 bytes (__fastcall stream, out).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, vector::resize 0x00619220, marker traits 0x00614AF0,
//   ReadWithSrcChunkOverflow 0x00993CA0.
// Model: engine/CMemoryDataInputStream.h + reader adapter; CUIMiniMapMarker {Region, UIDefName, Position, GraphicIndex, Angle}
//   is local (no engine header). Speed TU, no pragma: the traits' DefaultVal marker is built with `Region(0), UIDefName("", -1)`
//   and a body that zeroes the rest, which gives retail's `push -1 ; push offset ""` string construction.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct C2DVector {
    float X, Y;
};
struct CUIMiniMapMarker {
    long        Region;                                     // +0x00
    CCharString UIDefName;                                  // +0x04
    C2DVector   Position;                                   // +0x08
    long        DefNameLength;                              // +0x10
    long        GraphicIndex;                               // +0x14
    float       Angle;                                      // +0x18
    CUIMiniMapMarker() : Region(0), UIDefName("", -1)
    {
        Position.X = 0;
        Position.Y = 0;
        DefNameLength = 0;
        GraphicIndex = 0;
        Angle = 0;
    }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int newSize);                      // 0x00619220
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x00614AF0
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
template void __fastcall GFSerialiseVectorBinaryIn<CUIMiniMapMarker>(CMemoryDataInputStreamReader& stream, std::vector<CUIMiniMapMarker>& out);
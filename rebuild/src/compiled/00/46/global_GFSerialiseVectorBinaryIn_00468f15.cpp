// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CRGBColour>& out) -- read the element count, resize the colour vector, then read every colour in place
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00468F15, 144 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (slow path of the inlined count Read),
//   vector<CRGBColour>::resize(n, const CRGBColour&) @ 0x00468FA5, CDataInputStream::ReadColour() @ 0x009945E0 (hidden result).
//   Caller: CPersistContext::TransferBinary @ 0x00468EB4 (mode 2, the load stream at +0x24).
// Shape: the generic `GFSerialiseVectorBinaryIn` template (landed 0x005C2AD8 / 0x006194A0): an inlined 4-byte Read of the count
//   (64-bit position range check through an inline predicate, chunk `movsd` fast path, overflow reader), `out.resize(count)`
//   (STLport's one-argument resize inlined: `resize(n, T())` with CRGBColour's empty default constructor, so the temporary is
//   never written), then `traits.TransferBinaryIn(stream, &out[i])` with the CRGBColour traits inlined to
//   `*value = stream.ReadColour()` (`__forceinline`: a plain inline member is called out of line and its empty traits object
//   gets a frame word) -- the element address is formed before the call and the result copied as one dword.
//   The colour temporary and the ReadColour result share one frame word. Size TU with a frame pointer:
//   `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h (the PDB signature's stream type) + a reader adapter carrying the retail member functions; CRGBColour is the 4-byte
//   D3DCOLOR record (B, G, R, A) with an empty default constructor; the vector is STLport's three words.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CMemoryDataInputStream CMemoryDataInputStreamLayout
#include "engine/CMemoryDataInputStream.h"
#undef CMemoryDataInputStream
class CRGBColour;

class CDataInputStream
{
public:
    CRGBColour ReadColour();   // 0x009945E0
};
class CMemoryDataInputStream : public CDataInputStream, public CMemoryDataInputStreamLayout
{
};
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() { }
};

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
namespace std {
template <class T> class vector {
public:
    T* _M_start;                    // +0x00
    T* _M_finish;                   // +0x04
    T* _M_end_of_storage;           // +0x08
    void resize(unsigned int newSize, const T& x);                     // 0x00468FA5
    void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CRGBColour> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStreamReader& stream, CRGBColour* value) const { *value = stream.ReadColour(); }
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
template void __fastcall GFSerialiseVectorBinaryIn<CRGBColour>(CMemoryDataInputStreamReader& stream, std::vector<CRGBColour>& out);
// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CTextureReplacementDef::CEntry>& out) -- read the element count, resize the entry vector, then read every texture-replacement entry in place
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00469CB2, 149 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CRGBColour> is propagated. Load half of CPersistContext::TransferBinary @ 0x00469C3E (called from
//   CTextureReplacementDef::Transfer); Ego_r.pdb ??$GFSerialiseVectorBinaryIn@VCEntry@CTextureReplacementDef@@@@YIXAAVCMemoryDataInputStream@@...
//   CEntry = {long OriginalTexture, long NewTexture} (8-byte stride).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (slow path of the inlined count Read),
//   vector<CEntry>::resize(n, const CEntry&) @ 0x00469E00, CPersistTraits<CEntry>::TransferBinaryIn @ 0x00469D47 (landed;
//   its file is labelled CWeaponTrailGraphicSet -- the same two-long body).
// Shape: the generic template (landed 0x005C2AD8 / 0x006194A0) with the entry's traits specialisation: Ego_d.pdb names both
//   CEntry() and CEntry(long, long), so the traits DefaultVal is built as `DefaultVal(0, 0)` (two memberwise `and dword ptr
//   [..],0` stores; `DefaultVal()` value-initialisation gives `stosd` x2) while the empty CEntry() leaves STLport's
//   `resize(n, T())` temporary unwritten. The one-argument resize must be `__forceinline` (the size TU calls it out of line);
//   the count Read is inlined; `traits.TransferBinaryIn(stream, &out[i])` stays out of line.
//   Size TU with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a reader adapter carrying the retail member functions; CEntry is local (no engine
//   header); the vector is STLport's three words.
#pragma optimize("y", off)
#pragma optimize("s", on)
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
class CTextureReplacementDef { public:
    class CEntry { public:
        long OriginalTexture;                               // +0x00
        long NewTexture;                                    // +0x04
        CEntry() { }
        CEntry(long originalTexture, long newTexture) : OriginalTexture(originalTexture), NewTexture(newTexture) { }
    };
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize, const T& x);         // 0x00469E00
    __forceinline void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CTextureReplacementDef::CEntry> {
    CTextureReplacementDef::CEntry DefaultVal;
    CPersistTraits() : DefaultVal(0, 0) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, CTextureReplacementDef::CEntry* out) const;   // 0x00469D47
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
template void __fastcall GFSerialiseVectorBinaryIn<CTextureReplacementDef::CEntry>(CMemoryDataInputStreamReader& stream, std::vector<CTextureReplacementDef::CEntry>& out);
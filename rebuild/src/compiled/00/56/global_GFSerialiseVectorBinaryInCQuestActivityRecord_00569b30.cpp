// GFSerialiseVectorBinaryIn<CQuestActivityRecord>(CMemoryDataInputStream& stream, vector<CQuestActivityRecord>& out) -- read the element count, resize the vector, then read every 4-byte record in place through its persist traits
// Leaf: ??$GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569B30, 140 bytes, `__fastcall` (stream in ecx, vector in edx), plain `ret`; a
//   speed TU (no frame pointer, the `push ecx` word holds the count). Only caller: CPersistContext::Transfer<
//   CQuestActivityRecord> 0x00569F20 (landed; key "ActivitiesVec", binary-load arm), whose save arm calls
//   GFSerialiseVectorBinaryOut 0x005665B0 (one 4-byte write per element). The catalog's <CRGBColour> is
//   propagated; the element is the landed caller's CQuestActivityRecord, a 4-byte record (no PDB layout: TLC-only
//   quest data) whose default value is zero.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path),
//   vector<T>::resize(size_type, const T&) @ 0x005697C0 (landed 4-byte resize), CPersistTraits<T>::TransferBinaryIn
//   @ 0x004B85D0 (landed 4-byte read; `this` unused).
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC, chunkLength +0x14).
// Shape: the landed GFSerialiseVectorBinaryIn template (0x005C2AD8 / 0x00469CB2) without the size pragmas: the
//   inline 4-byte count Read (64-bit position range check, the fast path forwards the chunk word straight into
//   ebx), STLport's inline `resize(n) { resize(n, T()); }` (the zeroed temporary in the frame word), then
//   `traits.TransferBinaryIn(stream, &out[i])`. The traits are empty; the read-only TransferBinaryIn body is a
//   same-TU `__declspec(noinline)` stand-in, which lets VC7.1 alias the traits object onto the count's frame word.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
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
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct CQuestActivityRecord {                               // 4 bytes
    long Value;
    CQuestActivityRecord() : Value(0) { }
};

namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize, const T& x);         // 0x005697C0
    void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits {
    __declspec(noinline) void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const   // 0x004B85D0
    {
        long value;
        stream.Read(&value, sizeof(value));
        *(long*)out = value;
    }
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

template void __fastcall GFSerialiseVectorBinaryIn<CQuestActivityRecord>(CMemoryDataInputStreamReader& stream,
                                                                         std::vector<CQuestActivityRecord>& out);
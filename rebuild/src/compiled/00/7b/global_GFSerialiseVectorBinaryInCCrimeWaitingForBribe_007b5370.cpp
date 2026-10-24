// GFSerialiseVectorBinaryIn<CCrimeWaitingForBribe>(CMemoryDataInputStream& stream, vector<CCrimeWaitingForBribe>& out) -- read the element count, resize the vector to it, then read every 8-byte bribe record in place through its persist traits
// Leaf: ??$GFSerialiseVectorBinaryIn@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5370, 133 bytes (+11 int3), `__fastcall` (stream in ecx, vector in edx), plain
//   `ret`; speed TU, frame `sub esp,8`.
// Identity: the landed CPersistContext::TransferVector<CCrimeWaitingForBribe> 0x007B5470 calls it in mode 2 (binary
//   load); its per-element callee is the landed CPersistTraits<CCrimeWaitingForBribe>::TransferBinaryIn 0x007B42D0.
//   The catalog's <CDefString> is propagated.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the count read's slow path), vector<CCrimeWaitingForBribe>
//   ::resize(size_type) 0x007B5210 (out of line), CPersistTraits<CCrimeWaitingForBribe>::TransferBinaryIn 0x007B42D0.
// Shape: the landed GFSerialiseVectorBinaryIn template (0x006DD580): the inline 4-byte count Read (64-bit position range
//   check, the fast path forwards the chunk word into ebx), the one-argument resize, then `traits.TransferBinaryIn(
//   stream, &out[i])` with the index in edi. The traits object (8-byte DefaultVal, never written) shares its frame
//   word with the dead count slot (8-byte frame, not 12): VC7.1 only proves that when TransferBinaryIn's real body
//   (the landed 0x007B42D0, which never touches `this`) is in the TU as a `__declspec(noinline)` member.
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

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize);                      // 0x007B5210
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStreamReader* stream, long* value)
    {
        long number;
        stream->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<CCrimeWaitingForBribe> {
    CCrimeWaitingForBribe DefaultVal;
    __declspec(noinline) void TransferBinaryIn(CMemoryDataInputStreamReader* stream, CCrimeWaitingForBribe* value);   // 0x007B42D0
};

void CPersistTraits<CCrimeWaitingForBribe>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, CCrimeWaitingForBribe* value)
{
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->DeedHandle);
    longTraits.TransferBinaryIn(stream, &value->Type);
}

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
        traits.TransferBinaryIn(&stream, &out[i]);
}

template void __fastcall GFSerialiseVectorBinaryIn<CCrimeWaitingForBribe>(CMemoryDataInputStreamReader& stream,
                                                                          std::vector<CCrimeWaitingForBribe>& out);
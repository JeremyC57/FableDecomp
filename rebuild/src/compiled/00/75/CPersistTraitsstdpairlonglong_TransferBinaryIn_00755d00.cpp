// CPersistTraits<std::pair<long,long> >::TransferBinaryIn(CMemoryDataInputStream* stream, std::pair<long,long>* value) -- binary read of one (long, long) pair: first then second through the inlined 4-byte stream read
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00755D00, 225 bytes, `ret 8`.
// Called only from GFSerialiseMapBinaryIn<std::pair<long,long>, CCharString> (landed 0x007565E0, call at 0x007566BA) for
// the mapped values of a map<CCharString, pair<long,long> >, so the manifest's `CPersistTraits<CCrimeWaitingForBribe>`
// is propagated from the byte twin 0x007B42D0.
// Callee: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out.
// Shape: the landed one-value reader 0x007AD7F0 twice: CMemoryDataInputStream::Read inlined with the position guard in
// an inline bool predicate (64-bit [0, 0x7FFFFFFF] test on position + 4; its exits in the second read jump straight to
// duplicated `store ; pop ; ret 8` tails), `m_position += size` (the `lea ecx,[eax+4]` form). Both members go through
// an inline CPersistTraits<long>::TransferBinaryIn(stream, &member) whose scalar local (a dword move from the chunk)
// shares the dead stream-argument slot; two block locals in the body cost a `push ecx` frame word. `value` is loaded
// into edi once, after the first read. No frame pointer.
#include "engine/CMemoryDataInputStream.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader is CDataInputStream's.
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
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

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, long* value)
    {
        long number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<pair<long, long> > {
    pair<long, long> DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, pair<long, long>* value);
};

void CPersistTraits<pair<long, long> >::TransferBinaryIn(CMemoryDataInputStream* stream, pair<long, long>* value)
{
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->first);
    longTraits.TransferBinaryIn(stream, &value->second);
}
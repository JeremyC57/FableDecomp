// CPersistTraits<CCrimeWaitingForBribe>::TransferBinaryIn(CMemoryDataInputStream* stream, CCrimeWaitingForBribe* value) -- binary read of one bribe record: DeedHandle then Type through the inlined 4-byte stream read
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B42D0, 225 bytes, `ret 8`.
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCCrimeWaitingForBribe@@@@QAEXAAVCMemoryDataInputStream@@
// AAVCCrimeWaitingForBribe@@@Z; CCrimeWaitingForBribe (8 bytes) = { long DeedHandle +0, EOpinionDeedType Type +4 }.
// Callee: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out.
// Called from GFSerialiseVectorBinaryIn<CCrimeWaitingForBribe> 0x007B5370 (call at 0x007B53E3).
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
class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
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
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CCrimeWaitingForBribe { public:                     // 8 bytes
    long DeedHandle;                               // +0x0
    long Type;                                     // +0x4 EOpinionDeedType
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
template <> struct CPersistTraits<CCrimeWaitingForBribe> {
    CCrimeWaitingForBribe DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CCrimeWaitingForBribe* value);
};

void CPersistTraits<CCrimeWaitingForBribe>::TransferBinaryIn(CMemoryDataInputStream* stream, CCrimeWaitingForBribe* value)
{
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->DeedHandle);
    longTraits.TransferBinaryIn(stream, &value->Type);
}
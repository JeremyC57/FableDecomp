// CPersistTraits<CBrainUpdateZone>::TransferBinaryIn(CMemoryDataInputStream* stream, CBrainUpdateZone* value) -- binary read of one brain update zone: Distance, UpdateFrequency, InterruptionFrequency through the inlined 4-byte stream read
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCBrainUpdateZone@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00466C8E, 270 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCBrainUpdateZone@@@@QAEXAAVCMemoryDataInputStream@@AAVCBrainUpdateZone@@@Z;
// CBrainUpdateZone (12 bytes) = { long Distance +0, long UpdateFrequency +4, long InterruptionFrequency +8 }.
// Callee: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out.
// Called from GFSerialiseVectorBinaryIn<CBrainUpdateZone> 0x00466BEB (landed; it names this address).
// Shape: the landed 0x004671CA CBrainBehaviour reader with three longs: CMemoryDataInputStream::Read inlined three
// times, this module's Read storing the widened `newPosition` back (`add eax,4 ; xor ecx,ecx` then `mov [ebx+4],eax`),
// each long through an inline CPersistTraits<long>::TransferBinaryIn(stream, &member) whose local `number` is homed in
// the dead stream-argument slot [ebp+8]. EBP frame + size TU (`#pragma optimize("y", off)` + `("s", on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader is CDataInputStream's.
class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position = (long)newPosition;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CBrainUpdateZone {                               // 12 bytes
public:
    long Distance;                                     // +0x0
    long UpdateFrequency;                              // +0x4
    long InterruptionFrequency;                        // +0x8
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
template <> struct CPersistTraits<CBrainUpdateZone> {
    CBrainUpdateZone DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CBrainUpdateZone* value);
};

void CPersistTraits<CBrainUpdateZone>::TransferBinaryIn(CMemoryDataInputStream* stream, CBrainUpdateZone* value)
{
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->Distance);
    longTraits.TransferBinaryIn(stream, &value->UpdateFrequency);
    longTraits.TransferBinaryIn(stream, &value->InterruptionFrequency);
}
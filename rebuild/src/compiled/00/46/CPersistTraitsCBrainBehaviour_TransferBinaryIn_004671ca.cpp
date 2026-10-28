// CPersistTraits<CBrainBehaviour>::TransferBinaryIn(CMemoryDataInputStream* stream, CBrainBehaviour* value) -- binary read of one brain behaviour: the Name def string through its own reader, then Priority and ScriptAIPriority through the inlined 4-byte stream read
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004671CA, 195 bytes, `ret 8`.
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCBrainBehaviour@@@@QAEXAAVCMemoryDataInputStream@@AAVCBrainBehaviour@@@Z;
// CBrainBehaviour (12 bytes) = { CDefString Name +0, long Priority +4, long ScriptAIPriority +8 }.
// Callees: 0x0044FC00 CDefString::TransferBinaryIn(CDataInputStream&) (thiscall on value->Name; its manifest label
// `CPersistTraits<float>` is propagated), 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long).
// Called from the vector reader loop at 0x004671BA (stride 12).
// Shape: CMemoryDataInputStream::Read inlined twice; this module's Read stores the widened `newPosition` back as the
// position (`add eax,4 ; xor ecx,ecx` then `mov [ebx+4],eax`, where `m_position += size` gives the `lea` form). The two
// longs are read through an inline CPersistTraits<long>::TransferBinaryIn(stream, &member) (local `number` homed in
// the dead stream-argument slot [ebp+8], then `*value = number`): written out in the body, two locals cost a `push ecx`
// frame word and one reused local swaps eax/ecx in the first store. EBP frame + size TU.
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

class CDefString {
public:
    long Index;
    void TransferBinaryIn(CMemoryDataInputStream& stream);   // 0x0044FC00
};

class CBrainBehaviour { public:                               // 12 bytes
    CDefString Name;                                   // +0x0
    long       Priority;                               // +0x4
    long       ScriptAIPriority;                       // +0x8
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
template <> struct CPersistTraits<CBrainBehaviour> {
    CBrainBehaviour DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CBrainBehaviour* value);
};

void CPersistTraits<CBrainBehaviour>::TransferBinaryIn(CMemoryDataInputStream* stream, CBrainBehaviour* value)
{
    value->Name.TransferBinaryIn(*stream);
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->Priority);
    longTraits.TransferBinaryIn(stream, &value->ScriptAIPriority);
}
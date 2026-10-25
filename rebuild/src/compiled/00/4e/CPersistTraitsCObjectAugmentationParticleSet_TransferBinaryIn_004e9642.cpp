// CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn(CMemoryDataInputStream* stream, CObjectAugmentationParticleSet* value) -- binary read of one augmentation particle set: PeriodSecs, the four particle-effect handles, then the StartPoint and EndPoint def-string arrays
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCObjectAugmentationParticleSet@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9642, 460 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCObjectAugmentationParticleSet@@@@QAEXAAVCMemoryDataInputStream@@AAV...;
// CObjectAugmentationParticleSet = { float PeriodSecs +0, long ParticleEffect +4, long ParticleEffectToBlendTo +8,
// long AttackParticleEffect +0xC, long AttackParticleEffectToBlendTo +0x10, CArray<CDefString> StartPoint +0x14,
// CArray<CDefString> EndPoint +0x20 } (retail STLport vectors are 12 bytes, so the PDB's +0x24 EndPoint is +0x20 here).
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out,
// 0x00465852 GFSerialiseVectorBinaryIn<CDefString>(CMemoryDataInputStream&, vector<CDefString>&) (landed; __fastcall).
// Shape: the landed 0x004671CA CBrainBehaviour reader: CMemoryDataInputStream::Read inlined five times (this module's
// Read stores the widened `newPosition` back, `add eax,4 ; xor ecx,ecx` then `mov [ebx+4],eax`), each scalar through
// an inline CPersistTraits<T>::TransferBinaryIn(stream, &member) whose local is homed in the dead stream-argument slot
// [ebp+8]. EBP frame + size TU (`#pragma optimize("y", off)` + `("s", on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader is CDataInputStream's.
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CDefString {
public:
    long Index;
};

namespace std {
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}

template <class T> void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& out);
// 0x00465852 = GFSerialiseVectorBinaryIn<CDefString>

class CObjectAugmentationParticleSet {                 // 0x2C bytes in retail
public:
    float                   PeriodSecs;                // +0x00
    long                    ParticleEffect;            // +0x04
    long                    ParticleEffectToBlendTo;   // +0x08
    long                    AttackParticleEffect;      // +0x0C
    long                    AttackParticleEffectToBlendTo; // +0x10
    std::vector<CDefString> StartPoint;                // +0x14 CArray<CDefString>
    std::vector<CDefString> EndPoint;                  // +0x20 CArray<CDefString>
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
template <> struct CPersistTraits<float> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, float* value)
    {
        float number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<CObjectAugmentationParticleSet> {
    CObjectAugmentationParticleSet DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CObjectAugmentationParticleSet* value);
};

void CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn(CMemoryDataInputStream* stream, CObjectAugmentationParticleSet* value)
{
    CPersistTraits<float> floatTraits;
    floatTraits.TransferBinaryIn(stream, &value->PeriodSecs);
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->ParticleEffect);
    longTraits.TransferBinaryIn(stream, &value->ParticleEffectToBlendTo);
    longTraits.TransferBinaryIn(stream, &value->AttackParticleEffect);
    longTraits.TransferBinaryIn(stream, &value->AttackParticleEffectToBlendTo);
    GFSerialiseVectorBinaryIn(*stream, value->StartPoint);
    GFSerialiseVectorBinaryIn(*stream, value->EndPoint);
}
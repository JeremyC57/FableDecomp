// CPersistTraits<CEngineGraphic>::TransferBinaryIn(CMemoryDataInputStream* stream, CEngineGraphic* value) -- binary read of one engine graphic: the Type as a long, BankIndex, AnimStep, RenderSizeX, then the AdditiveAlpha byte as a bool
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCEngineGraphic@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067D830, 457 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCEngineGraphic@@@@QAEXAAVCMemoryDataInputStream@@AAVCEngineGraphic@@@Z;
// CEngineGraphic (0x0E packed bytes) = { ulong BankIndex +0, float AnimStep +4, float RenderSizeX +8,
// bool AdditiveAlpha +0xC, uchar Type +0xD }. The order mirrors the landed TransferBinaryOut 0x0067D7D0 (WriteLong(Type),
// WriteULong(BankIndex), WriteFloat x2, WriteEBOOL).
// Callee: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out.
// Shape: the landed 0x007B42D0 speed-TU reader: CMemoryDataInputStream::Read inlined five times (`position += size`, the
// `lea ecx,[eax+4]` form), each scalar's local (a dword move from the chunk) shares the dead stream-argument slot; the
// Type long is truncated to its byte (`mov cl,[esp+10h] ; mov [ebp+0Dh],cl`), the bool is read as a byte and normalised
// (`test al,al ; setne`), its fast path keeping the dead local store and its own duplicated `pop ; ret 8` tail.
// No frame pointer (speed TU).
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

#pragma pack(push, 1)
class CEngineGraphic {                                 // 0x0E bytes
public:
    unsigned long BankIndex;                           // +0x0
    float         AnimStep;                            // +0x4
    float         RenderSizeX;                         // +0x8
    bool          AdditiveAlpha;                       // +0xC
    unsigned char Type;                                // +0xD
};
#pragma pack(pop)

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, long* value)
    {
        long number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<unsigned long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, unsigned long* value)
    {
        unsigned long number;
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
template <> struct CPersistTraits<bool> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value)
    {
        unsigned char byte;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&byte, sizeof(byte));
        *value = byte != 0;
    }
};
template <> struct CPersistTraits<CEngineGraphic> {
    CEngineGraphic DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CEngineGraphic* value);
};

void CPersistTraits<CEngineGraphic>::TransferBinaryIn(CMemoryDataInputStream* stream, CEngineGraphic* value)
{
    long type;
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &type);
    value->Type = (unsigned char)type;
    CPersistTraits<unsigned long> ulongTraits;
    ulongTraits.TransferBinaryIn(stream, &value->BankIndex);
    CPersistTraits<float> floatTraits;
    floatTraits.TransferBinaryIn(stream, &value->AnimStep);
    floatTraits.TransferBinaryIn(stream, &value->RenderSizeX);
    CPersistTraits<bool> boolTraits;
    boolTraits.TransferBinaryIn(stream, &value->AdditiveAlpha);
}
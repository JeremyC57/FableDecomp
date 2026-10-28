// CPersistTraits<CDegradableInfo>::TransferBinaryIn(CMemoryDataInputStream* stream, CDegradableInfo* value) -- binary read of one degradable stage: HealthPercentage, the raw 14-byte engine graphic, SmashParticleEmitter, the BlocksNavigation byte as a bool, then the record check word (discarded)
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCDegradableInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067DF60, 452 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCDegradableInfo@@@@QAEXAAVCMemoryDataInputStream@@AAVCDegradableInfo@@@Z;
// CDegradableInfo (28 bytes) = { float HealthPercentage +0, CEngineGraphic EngineGraphic +4 (14 packed bytes),
// long SmashParticleEmitter +0x14, bool BlocksNavigation +0x18 }. Mirrors the landed save arm
// GFSerialiseVectorBinaryOut<CDegradableInfo> 0x0067E720 (float, raw 14-byte graphic, long, bool, check word 0x01AB6CE3).
// Callee: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out.
// Shape: the landed 0x007B42D0 speed-TU reader: CMemoryDataInputStream::Read inlined five times (`position += size`);
// the engine graphic is one 14-byte Read straight into the member (memcpy as 3 dwords + 1 word, the stream fields
// reloaded after it because the destination may alias the stream), the bool is a byte normalised with `setne`, and the
// trailing check word is read into a dead local (its memcpy deleted, only the stream bookkeeping and the overflow
// call's address survive). No frame pointer (speed TU).
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

class CDegradableInfo {                                // 28 bytes
public:
    float          HealthPercentage;                   // +0x00
    CEngineGraphic EngineGraphic;                      // +0x04
    long           SmashParticleEmitter;               // +0x14
    bool           BlocksNavigation;                   // +0x18
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
template <> struct CPersistTraits<bool> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value)
    {
        unsigned char byte;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&byte, sizeof(byte));
        *value = byte != 0;
    }
};
// The record check word every degradable stage ends with: read and dropped.
__forceinline void SkipCheckWord(CMemoryDataInputStream* stream)
{
    long check;
    static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&check, sizeof(check));
}

template <> struct CPersistTraits<CDegradableInfo> {
    CDegradableInfo DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CDegradableInfo* value);
};

void CPersistTraits<CDegradableInfo>::TransferBinaryIn(CMemoryDataInputStream* stream, CDegradableInfo* value)
{
    CPersistTraits<float> floatTraits;
    floatTraits.TransferBinaryIn(stream, &value->HealthPercentage);
    static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&value->EngineGraphic, sizeof(CEngineGraphic));
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->SmashParticleEmitter);
    CPersistTraits<bool> boolTraits;
    boolTraits.TransferBinaryIn(stream, &value->BlocksNavigation);
    SkipCheckWord(stream);
}
// CPersistTraits<CMusicEntry>::TransferBinaryIn(CMemoryDataInputStream* stream, CMusicEntry* value) -- binary read of one music entry: ID, the Filename def string through its own reader, Gain, then the CacheToHD byte as a bool
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCMusicEntry@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046B545, 277 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCMusicEntry@@@@QAEXAAVCMemoryDataInputStream@@AAVCMusicEntry@@@Z;
// CMusicEntry (16 bytes) = { ulong ID +0, CDefString Filename +4, float Gain +8, bool CacheToHD +0xC }.
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out,
// 0x0044FC00 CDefString::TransferBinaryIn(CMemoryDataInputStream&) (thiscall on value->Filename).
// Shape: the landed 0x004671CA CBrainBehaviour reader: CMemoryDataInputStream::Read inlined (this module's Read stores
// the widened `newPosition` back, `add eax,4 ; xor ecx,ecx` then `mov [ebx+4],eax`), each scalar through an inline
// CPersistTraits<T>::TransferBinaryIn(stream, &member) whose local is homed in the dead stream-argument slot [ebp+8];
// the 4-byte float goes through a GPR (memcpy'd float local). The bool is read as a byte (`movsb` into [ebp+0Bh]) and
// normalised (`cmp byte [ebp+0Bh],0 ; setne`). EBP frame + size TU (`#pragma optimize("y", off)` + `("s", on)`).
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
    void TransferBinaryIn(CMemoryDataInputStream& stream);   // 0x0044FC00
};

class CMusicEntry {                                    // 16 bytes
public:
    unsigned long ID;                                  // +0x0
    CDefString    Filename;                            // +0x4
    float         Gain;                                // +0x8
    bool          CacheToHD;                           // +0xC
};

template <class T> struct CPersistTraits;
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
template <> struct CPersistTraits<CMusicEntry> {
    CMusicEntry DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CMusicEntry* value);
};

void CPersistTraits<CMusicEntry>::TransferBinaryIn(CMemoryDataInputStream* stream, CMusicEntry* value)
{
    CPersistTraits<unsigned long> ulongTraits;
    ulongTraits.TransferBinaryIn(stream, &value->ID);
    value->Filename.TransferBinaryIn(*stream);
    CPersistTraits<float> floatTraits;
    floatTraits.TransferBinaryIn(stream, &value->Gain);
    CPersistTraits<bool> boolTraits;
    boolTraits.TransferBinaryIn(stream, &value->CacheToHD);
}
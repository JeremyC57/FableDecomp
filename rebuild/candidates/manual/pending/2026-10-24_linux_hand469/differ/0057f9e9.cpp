// CPersistTraits<CHeroMarriageInfo>::TransferBinaryIn(CMemoryDataInputStream* stream, CHeroMarriageInfo* value) -- binary read of one marriage record: reset it to the default, then WifeUID, MaritalHomeUID, State and the GayMarriage byte as a bool
// Grade: DIFFER(409v413) -- only the chunk-length compare of the first UID and the State read: retail `mov ecx,[ebx+14h] ; cmp ecx,N`, ours `cmp dword ptr [ebx+14h],N` (the second UID and the bool already agree)
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCHeroMarriageInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057F9E9, 413 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCHeroMarriageInfo@@@@QAEXAAVCMemoryDataInputStream@@AAVCHeroMarriageInfo@@@Z
//   (the name set also holds the value-returning overload, hence the long leaf).
// CHeroMarriageInfo (0x18 bytes) = { ulong64 WifeUID +0, ulong64 MaritalHomeUID +8, EMarriageState State +0x10,
//   bool GayMarriage +0x14 }. Local model: engine/CHeroMarriageInfo.h is a POD `struct` with State as bytes, but retail
//   needs the PDB `class` with its member-wise default constructor (five dword zeros + one byte zero, padding untouched).
// Callee: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out.
// Shape: `*value = CHeroMarriageInfo()` (the zeroed temporary at [ebp-20h], copied with `push 6 ; pop ecx ; rep movsd`),
//   then CMemoryDataInputStream::Read inlined four times -- this module's Read stores the widened `newPosition` back
//   (`add eax,N ; xor ecx,ecx` ... `mov [ebx+4],eax`) and reloads the chunk length after the copy -- the 8-byte UIDs
//   through a 64-bit frame local [ebp-8], the state through the dead stream slot [ebp+8], the bool as a byte at
//   [ebp+0Bh] normalised with `setne`. `value` stays in edx (reloaded after each overflow call).
//   EBP frame + size TU (`#pragma optimize("y", off)` + `("s", on)`).
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

enum EMarriageState { MARRIAGE_STATE_NONE = 0, MARRIAGE_STATE_FORCE_DWORD = 0x7fffffff };

class CHeroMarriageInfo {                              // 0x18 bytes
public:
    unsigned __int64 WifeUID;                          // +0x00
    unsigned __int64 MaritalHomeUID;                   // +0x08
    EMarriageState   State;                            // +0x10
    bool             GayMarriage;                      // +0x14
    CHeroMarriageInfo() : WifeUID(0), MaritalHomeUID(0), State(MARRIAGE_STATE_NONE), GayMarriage(false) {}
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<unsigned __int64> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, unsigned __int64* value)
    {
        unsigned __int64 number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<EMarriageState> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, EMarriageState* value)
    {
        EMarriageState number;
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
template <> struct CPersistTraits<CHeroMarriageInfo> {
    CHeroMarriageInfo DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CHeroMarriageInfo* value);
};

void CPersistTraits<CHeroMarriageInfo>::TransferBinaryIn(CMemoryDataInputStream* stream, CHeroMarriageInfo* value)
{
    *value = CHeroMarriageInfo();
    CPersistTraits<unsigned __int64> uidTraits;
    uidTraits.TransferBinaryIn(stream, &value->WifeUID);
    uidTraits.TransferBinaryIn(stream, &value->MaritalHomeUID);
    CPersistTraits<EMarriageState> stateTraits;
    stateTraits.TransferBinaryIn(stream, &value->State);
    CPersistTraits<bool> boolTraits;
    boolTraits.TransferBinaryIn(stream, &value->GayMarriage);
}

// CPersistTraits<std::pair<CDefIndex,CDefString> >::TransferBinaryIn(CMemoryDataInputStream* stream, std::pair<CDefIndex,CDefString>* value) -- binary read of one (def index, def string) element: the index through the inlined 4-byte stream read, then the def string's own binary reader
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046B0D6, 110 bytes, `ret 8`.
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out;
// 0x0044FC00 CDefString::TransferBinaryIn(CDataInputStream&) on value+4 (thiscall, one argument; Ego_r.pdb
// ?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z -- its manifest label `CPersistTraits<float>` is propagated).
// Called only from the vector reader just above (0x0046B0C6 loop: element stride 8, traits object {0, -1}, i.e. a
// zero index and an unset (-1) def string), so the element is an 8-byte (CDefIndex, CDefString) pair; the manifest
// label `CPersistTraits<CDefIndex>` names the inlined first-member read (CPersistTraits<CDefIndex>::TransferBinaryIn
// returns a CDefIndex by value in Ego_r.pdb).
// Shape: CMemoryDataInputStream::Read inlined (64-bit [0, 0x7FFFFFFF] guard on position + 4, chunk copy when >= 4 bytes
// remain, else the overflow reader); CDefIndex is a one-long CLASS, so the 4-byte memcpy is a `movsd` into the local,
// which VC homes in the dead stream-argument slot ([ebp+8]); the stream stays in ebx for the def-string read.
// EBP frame + size TU (optimize("y", off), optimize("s", on)), as the landed 0x0046C98D.
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
            position += size;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CDefIndex {
public:
    long Val;
};

class CDefString {
public:
    long Index;
    void TransferBinaryIn(CMemoryDataInputStream& stream);   // 0x0044FC00
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<pair<CDefIndex, CDefString> > {
    pair<CDefIndex, CDefString> DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, pair<CDefIndex, CDefString>* value);
};

void CPersistTraits<pair<CDefIndex, CDefString> >::TransferBinaryIn(CMemoryDataInputStream* stream, pair<CDefIndex, CDefString>* value)
{
    CDefIndex index;
    static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&index, sizeof(index));
    value->first = index;
    value->second.TransferBinaryIn(*stream);
}
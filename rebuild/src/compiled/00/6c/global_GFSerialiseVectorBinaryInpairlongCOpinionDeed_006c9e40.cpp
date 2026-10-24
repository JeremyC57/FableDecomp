// GFSerialiseVectorBinaryIn<pair<long, COpinionDeed*> >(CMemoryDataInputStream& stream, vector<pair<long, COpinionDeed*> >& out) -- read the element count, resize the vector, then read every (handle, deed) pair through the element persist traits
// Leaf: ??$GFSerialiseVectorBinaryIn@U?$pair@JPAVCOpinionDeed@@@std@@@@YIXAAUCMemoryDataInputStreamReader@@AAV?$vector@U?$pair@JPAVCOpinionDeed@@@std@@@std@@@Z
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- GOTCHAS 1046: noinline same-TU TransferBinaryIn bodies for the two empty element traits (long @ 0x0040FE60, COpinionDeed* @ 0x006C88D0 stand-in) alias the traits this onto the spilled vector-reference word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9E40, length 156 bytes (+ int3 padding), `__fastcall` (stream in ecx,
// vector in edx), plain `ret`; a speed TU (no frame pointer). Single caller 0x006C9F27 (0x006C9EE0, the deed log's
// vector transfer used by CTCHeroOpinionDeedLog::OnSerialise 0x006C7370 on load -- the counterpart of this chunk's
// __copy 0x006C9B70 on save). The manifest label "GFSerialiseVectorBinaryIn<CDefString>" names the wrong element.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, vector<pair<long, COpinionDeed*> >::resize
// (size_type) @ 0x006C9D50, CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60 (landed) and
// CPersistTraits<COpinionDeed*>::TransferBinaryIn @ 0x006C88D0, each on an element-traits temporary.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC, chunkLength +0x14); 8-byte elements.
// Shape: this chunk's 0x006D4B40 template with the pair traits' TransferBinaryIn inline: per element an empty element
// traits object and its out-of-line TransferBinaryIn on `&out->first` / `&out->second`; the pair DefaultVal's zeros
// are dead stores VC7.1 drops.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
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

class COpinionDeed;

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize);                      // 0x006C9D50 for pair<long, COpinionDeed*>
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits;

template <> struct CPersistTraits<long> {                  // scalar traits: empty, no stored default
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, long* out) const;               // 0x0040FE60
};

template <> struct CPersistTraits<COpinionDeed*> {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, COpinionDeed** out) const;      // 0x006C88D0
};

// GOTCHAS 1046: the callees that receive the empty element traits get their real shape in the TU (noinline): VC7.1
// then aliases the traits `this` onto the spilled vector-reference word as retail.
__declspec(noinline) void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, long* out) const
{
    stream.Read(out, sizeof(long));
}
__declspec(noinline) void CPersistTraits<COpinionDeed*>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, COpinionDeed** out) const
{
    stream.Read(out, sizeof(COpinionDeed*));
}

template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    std::pair<T1, T2> DefaultVal;
    CPersistTraits() : DefaultVal(T1(), T2()) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, std::pair<T1, T2>* out) const
    {
        CPersistTraits<T1> firstTraits;
        firstTraits.TransferBinaryIn(stream, &out->first);
        CPersistTraits<T2> secondTraits;
        secondTraits.TransferBinaryIn(stream, &out->second);
    }
};

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}

template void __fastcall GFSerialiseVectorBinaryIn<std::pair<long, COpinionDeed*> >(CMemoryDataInputStreamReader& stream,
                                                                                    std::vector<std::pair<long, COpinionDeed*> >& out);
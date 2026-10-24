// CPersistTraits<std::pair<CCharString,long> >::TransferBinaryIn(CMemoryDataInputStream*, std::pair<CCharString,long>*) -- binary persist read of a {string, long} pair: the string through the stream's virtual string reader, then the inlined 4-byte read
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D3B90, 170 bytes (+6 int3 padding), `__fastcall` (the traits object in ecx is never
//   read), `ret 8`.
// Callees: the stream's virtual string read (slot 6, +0x18, CCharString returned through a hidden pointer),
//   CCharString::operator=(const CCharString&) @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, CDataInputStream::
//   ReadWithSrcChunkOverflow @ 0x00993CA0.
// Shape: the two element reads are inlined in order, with no element-traits temporaries (a CPersistTraits<CCharString>
//   temporary would construct and destroy its CCharString DefaultVal). The string: `out->first = stream->ReadString()`
//   with the returned temporary in the dead stream-argument slot, destroyed right after the assignment. The long: the
//   inlined CMemoryDataInputStream::Read (64-bit [0, 0x7FFFFFFF] position check, fast copy from the chunk pointer +0xC,
//   else ReadWithSrcChunkOverflow) into a local in the same dead slot, copied to out->second with one return tail per exit.
// The stream is modelled locally: the generated engine header carries the vptr as a plain `void*` member, which cannot
// express the virtual string read; the data members match it (+4 position, +0xC chunk pointer, +0x14 chunk remaining).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                                   // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                 // 0x0099EFB0
};

class CMemoryDataInputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCharString ReadString();                                 // slot 6 (+0x18)
    unsigned long        position;                                    // +0x04
    long                 sourceLength;                                // +0x08
    const unsigned char* chunkBase;                                   // +0x0C
    long                 chunkPosition;                               // +0x10
    long                 chunkLength;                                 // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);  // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
};

// The element reads (no element-traits objects: retail builds no CCharString DefaultVal for the string element).
__forceinline void TransferBinaryInElement(CMemoryDataInputStream* stream, CCharString* out)
{
    *out = stream->ReadString();
}

__forceinline void TransferBinaryInElement(CMemoryDataInputStream* stream, long* out)
{
    long value;
    stream->Read(&value, sizeof(value));
    *out = value;
}

template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    void TransferBinaryIn(CMemoryDataInputStream* stream, std::pair<T1, T2>* out) const
    {
        TransferBinaryInElement(stream, &out->first);
        TransferBinaryInElement(stream, &out->second);
    }
};

template struct CPersistTraits<std::pair<CCharString, long> >;
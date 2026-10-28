// GFSerialiseVectorBinaryIn<pair<CCharString, long> >(CMemoryDataInputStream& stream, vector<pair<CCharString, long> >& out) -- read the element count, resize the vector, then read every (string, long) pair in place through the pair's persist traits
// Leaf: ??$GFSerialiseVectorBinaryIn@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4B40, length 187 bytes (+ int3 padding), `__fastcall` (stream in ecx,
// vector in edx), plain `ret`; a speed TU (no frame pointer). Single caller 0x006D4C4E in the landed
// CPersistContext::TransferVector<CNamedLong> 0x006D4C00 (binary-load arm; CNamedLong = pair<CCharString, long>). The
// manifest label "GFSerialiseVectorBinaryIn<CCharString>" names the wrong element.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E, -1), CCharString copy constructor
// @ 0x0099EC30, ~CCharString @ 0x0099EAE0 (x2), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
// vector<pair<CCharString, long> >::resize(size_type) @ 0x006D4A20 (out of line here), CPersistTraits<pair<CCharString,
// long> >::TransferBinaryIn @ 0x006D3B90 (landed; `this` = the traits object).
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC, chunkLength +0x14); 8-byte elements.
// Shape: the landed GFSerialiseVectorBinaryIn template (0x00569B30): `CPersistTraits<T> traits;` first -- the pair
// traits' DefaultVal is built from the element traits' defaults (a CPersistTraits<CCharString> temporary holding
// CCharString(""), copied into first, then destroyed; second = CPersistTraits<long>'s 0) -- then the inline 4-byte
// count Read (64-bit position range check; the count's slow-path buffer reuses the dead temporary's frame word),
// `out.resize(count)`, and `traits.TransferBinaryIn(stream, &out[i])` per element (start reloaded each pass); the
// traits' CCharString is destroyed last.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
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
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

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
    void resize(unsigned int newSize);                      // 0x006D4A20 for pair<CCharString, long>
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits;

template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    CPersistTraits() : DefaultVal("") { }
};

template <> struct CPersistTraits<long> {
    long DefaultVal;
    CPersistTraits() : DefaultVal(0) { }
};

template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    std::pair<T1, T2> DefaultVal;
    CPersistTraits() : DefaultVal(CPersistTraits<T1>().DefaultVal, CPersistTraits<T2>().DefaultVal) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, std::pair<T1, T2>* out) const;   // 0x006D3B90
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

template void __fastcall GFSerialiseVectorBinaryIn<std::pair<CCharString, long> >(CMemoryDataInputStreamReader& stream,
                                                                                  std::vector<std::pair<CCharString, long> >& out);
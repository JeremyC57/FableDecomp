// GFSerialiseVectorBinaryIn<CCharString>(CMemoryDataInputStream&, std::vector<CCharString>&) -- read the count, resize the vector, then read every string in place
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00433273, length 185 bytes, plain `ret` (__fastcall: stream in ecx, vector in edx).
// Name: the CCharString instance of the template the landed 0x005C2AD8 (NInventory::CItem) reconstructs; callers 0x0043321E and
// 0x0049B91E. The catalog name is right.
// Callees: CCharString::CCharString() @ 0x0099E4B0, ~CCharString @ 0x0099EAE0, CCharString::operator=(const CCharString&)
// @ 0x0099EFB0, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, vector<CCharString>::resize(size_type, const
// CCharString&) @ 0x0043332C (out of line; the catalog's "CArray::push_back" is propagated), and the stream's slot 6 (+0x18)
// CMemoryDataInputStream::ReadNullTerminatedString() @ 0x00995D30 (vtable 0x0123675C; FableWin `UAE?AVCCharString@@XZ`).
// Pragmas: #pragma optimize("y", off) + #pragma optimize("s", on) (as the landed 0x005C2AD8).
// Shape: the landed template verbatim -- `CPersistTraits<T> traits` (its DefaultVal is the CCharString at [ebp-0x10], built first,
// destroyed last), the inline 4-byte count read (the __int64 position range test, then memcpy -> `movsd` from the chunk or the
// overflow reader), `out.resize(count)` which here is STLport's inline `resize(n, T())` around the out-of-line two-argument
// resize (the T() temporary at [ebp-0xC]), and `traits.TransferBinaryIn(stream, &out[i])` inlined for CCharString as
// `*out = stream.ReadNullTerminatedString()` (the vector's start reloaded each pass, the result temporary reusing [ebp-0xC]).
// Because CCharString's constructor is out of line, the traits constructor, TransferBinaryIn and the one-argument resize need
// __forceinline in this size TU (without it all three are emitted as calls: 153 bytes).
// Layout: engine/CMemoryDataInputStream.h's members (position +4, chunkBase +0xC, chunkLength +0x14); that header flattens the vptr
// into a void* member and so cannot carry the virtual call, hence a local class with the same members and a real vtable.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
    void* PStringData;
};

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream {                           // vtable 0x0123675C, 0x1C bytes
public:
    virtual ~CMemoryDataInputStream();                   // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCharString ReadNullTerminatedString();      // slot 6 (+0x18) 0x00995D30
    long  position;                                      // +0x04
    long  sourceLength;                                  // +0x08
    long  chunkBase;                                     // +0x0c
    long  chunkPosition;                                 // +0x10
    long  chunkLength;                                   // +0x14
    void* StreamSource;                                  // +0x18

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

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int newSize, const T& x);       // 0x0043332C for CCharString
    __forceinline void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    __forceinline CPersistTraits() : DefaultVal() { }
    __forceinline void TransferBinaryIn(CMemoryDataInputStream& stream, T* out) const { *out = stream.ReadNullTerminatedString(); }
};

__forceinline void ReadCount(CMemoryDataInputStream& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
template void __fastcall GFSerialiseVectorBinaryIn<CCharString>(CMemoryDataInputStream& stream, std::vector<CCharString>& out);
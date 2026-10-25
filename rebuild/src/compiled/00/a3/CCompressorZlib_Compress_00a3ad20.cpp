// CCompressorZlib::Compress(const void* data, long count, long elementSize, CArray<unsigned char>& output) -- compress count*elementSize bytes into a CArray: refuse when the worst-case block size overflows a long, size the array to the bound, deflate through the virtual Compress, trim to the result; returns the compressed size
// Leaf: ?Compress@CCompressorZlib@@QAEJPBXJJAAV?$CArray@E@@@Z
// Retail: Fable.exe @ 0x00A3AD20, 166 bytes (+ int3 padding), thiscall `ret 10h`. FableWin.pdb
//   ?Compress@CCompressorZlib@@QAEJPBXJJAAV?$CArray@E@@@Z (no rel32 caller in TLC: kept, unreferenced).
// Callees: __allmul 0x00BFEB50 (the 64-bit `* 2`), vector<unsigned char>::resize(n, const unsigned char&) 0x00486189
//   (STLport's one-argument resize inline around it, the `unsigned char()` temporary homed in the dead count slot),
//   CCompressorZlib::Compress(const void*, ulong, void*, ulong, CCompressionInfo&) through vtable slot 5 (+0x14).
// Label: catalog "TextureConvert_ResizeBuffer" is descriptive.
// Layout: CCompressionInfo is the packed 9-byte {UncompressedSize, CompressedSize, Method} of the landed slot-5
//   Compress 0x00A3AC40 (a local in the 0xC frame); CArray<unsigned char> is the retail 12-byte STLport vector.
// Shape: the worst case `(__int64)count * elementSize * 2 + 13` (one-operand `imul`, __allmul, `add ; adc`) is range
//   checked against [0, 0x7fffffff]; GetAsCArray() returns the start of the buffer, or the array object itself when it
//   is empty (`sub ecx,eax ; jne ; mov eax,edi` -- inferred from the bytes); the result is the info's CompressedSize.
//   The 32-bit sizes are written inline and CSE'd by VC7.1 (`imul esi,edi`, `lea ebp,[esi+esi+0Dh]`); named size
//   locals swap esi/edi.
#pragma pack(push, 1)
class CCompressionInfo {                                    // 9 bytes, packed
public:
    unsigned long UncompressedSize;                         // +0x00
    unsigned long CompressedSize;                           // +0x04
    unsigned char Method;                                   // +0x08
};
#pragma pack(pop)

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    _Tp& operator[](unsigned int n) { return _M_start[n]; }
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x00486189 (vector<unsigned char>)
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}

template <class T> class CArray : public std::vector<T> {  // retail 12 bytes
public:
    T* GetAsCArray() { return this->size() != 0 ? &(*this)[0] : (T*)this; }
};

class CACompressor {
public:
    virtual ~CACompressor();
    virtual void SetQuality(float quality);
    virtual void CompressAsImage(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize,
                                 long width, long height, long bpp, CCompressionInfo& info);
    virtual void Decompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize,
                            CCompressionInfo& info);
    virtual void v04();
    virtual void Compress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize,
                          CCompressionInfo& info);           // slot 5 (+0x14)
};

class CCompressorZlib : public CACompressor {
public:
    long Compress(const void* data, long count, long elementSize, CArray<unsigned char>& output);
    using CACompressor::Compress;
};

long CCompressorZlib::Compress(const void* data, long count, long elementSize, CArray<unsigned char>& output)
{
    __int64 maxSize = (__int64)count * elementSize * 2 + 13;
    if (maxSize < 0 || maxSize > 0x7fffffff)
        return 0;
    output.resize(count * elementSize * 2 + 13);
    CCompressionInfo info;
    Compress(data, count * elementSize, output.GetAsCArray(), count * elementSize * 2 + 13, info);
    output.resize(info.CompressedSize);
    return info.CompressedSize;
}
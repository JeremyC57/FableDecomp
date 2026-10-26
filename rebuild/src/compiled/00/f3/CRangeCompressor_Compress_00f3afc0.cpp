// CRangeCompressor::Compress(const void* source, long width, long height, CArray<unsigned char>& out) -- range-compress a width x height block into a byte array sized to the worst case, then shrink it to the produced size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3AFC0, length 145 bytes (`ret 10h`).
// Callees: CArray<unsigned char>::resize(n, const unsigned char&) @ 0x00486189 (x2, zero fill; the zero byte is homed
// in the dead width-argument slot), the buffer compressor @ 0x00F3AE50 (catalog "RangeCompressor_Compress", a
// five-argument __fastcall: source, width, height, dest, destSize).
// Layout: the retail 12-byte CArray {first, last, end} as in the landed CCompressorZlib::InternalCompress 0x00A3ADD0,
// whose shape this follows: the worst case `width * height + 1` is checked in signed 64 bits against
// [0, 0x7FFFFFFF] (refused with 0), then recomputed in 32 bits for the resize; the data pointer goes through the
// inline accessor that yields the array object itself when empty. The bound is written out at both uses (a
// `size` local moves the values into other registers).
// Leaf: Compress@CRangeCompressor
template<class T> class CArray { public:
    T* m_first;                                                 // +0
    T* m_last;                                                  // +4
    T* m_end;                                                   // +8
    unsigned long GetSize() const { return (unsigned long)(m_last - m_first); }
    T* GetData() { return GetSize() ? m_first : (T*)this; }
    void resize(unsigned long size, const T& value);           // 0x00486189 (CArray<unsigned char>)
};

struct CRangeCompressor {
    long CompressToBuffer(const void* source, long width, long height, unsigned char* dest, long destSize);  // 0x00F3AE50
    long Compress(const void* source, long width, long height, CArray<unsigned char>& out);
};

long CRangeCompressor::Compress(const void* source, long width, long height, CArray<unsigned char>& out)
{
    __int64 bound = (__int64)width * height + 1;
    if (bound < 0 || bound > 0x7fffffff)
        return 0;
    out.resize(width * height + 1, 0);
    long produced = CompressToBuffer(source, width, height, out.GetData(), width * height + 1);
    out.resize(produced, 0);
    return produced;
}
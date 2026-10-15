// CCompressorZlib::InternalCompress(const void* source, unsigned long sourceSize, CArray<unsigned char>& out) -- deflate into a byte array sized to the zlib worst case (size + size/100 + 16, refused when that overflows 32 bits), then shrink it to the produced size
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3ADD0, 155 bytes (160 with padding).
// Callees: __aulldiv 0x00BFEE00 (the 64-bit bound), CArray<unsigned char>::resize(n, const unsigned char&)
//          0x00486189 (zero fill; the zero byte homed in the dead sourceSize argument slot),
//          CCompressorZlib::InternalCompress(const void*, ulong, void*, ulong) 0x00A3AA40 (the deflate worker),
//          CArray<unsigned char>::DeallocateUnusedMemory 0x009B5DB0.
//
// Shape: the overflow guard is computed in 64 bits (`(unsigned __int64)size / 100 + size + 16`, its high
// word spilled to the frame), the resize recomputes the same bound in 32 bits (reciprocal multiply by
// 0x51EB851F, shr 5 = / 100). The output buffer is the retail 12-byte CArray {first, last, end}; its data
// pointer goes through an inline accessor that yields the array object itself when empty (`size ?
// first : this`, sharing the size subtraction with the size argument).
#include "engine/CCompressorZlib.h"

template<class T> struct CArray {
    T* m_first;                                                 // +0
    T* m_last;                                                  // +4
    T* m_end;                                                   // +8
    unsigned long GetSize() const { return (unsigned long)(m_last - m_first); }
    T* GetData() { return GetSize() ? m_first : (T*)this; }
    void resize(unsigned long size, const T& value);           // 0x00486189 (CArray<unsigned char>)
    void DeallocateUnusedMemory();                              // 0x009B5DB0
};

struct CCompressorZlib_Retail {
    virtual void v00();
    unsigned char _body[0x54];                                  // +0x04..+0x57 (CACompressor, z_stream, flags)
    unsigned long InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AA40
    unsigned long InternalCompress(const void* source, unsigned long sourceSize, CArray<unsigned char>& out);
};

FABLE_STATIC_ASSERT(sizeof(CCompressorZlib_Retail) == sizeof(CCompressorZlib) - 4);

unsigned long CCompressorZlib_Retail::InternalCompress(const void* source, unsigned long sourceSize, CArray<unsigned char>& out)
{
    unsigned __int64 bound = (unsigned __int64)sourceSize / 100 + sourceSize + 16;
    if (bound > 0xFFFFFFFF)
        return 0;
    out.resize(sourceSize / 100 + sourceSize + 16, 0);
    unsigned long produced = InternalCompress(source, sourceSize, out.GetData(), out.GetSize());
    out.resize(produced, 0);
    out.DeallocateUnusedMemory();
    return produced;
}

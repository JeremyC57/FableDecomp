#include <cstdio>
#include <stddef.h>
class CCompressorZlib;
static int g_baseCtors = 0;
struct CBaseClassNonCopyable { virtual void v00() { } CBaseClassNonCopyable() { ++g_baseCtors; } };
struct CDataStream : CBaseClassNonCopyable { unsigned long m_position, m_length, m_flags, m_bufferStart, m_bufferEnd; virtual void v00() { } CDataStream() : m_position(0), m_length(0), m_flags(0), m_bufferStart(0), m_bufferEnd(0) { } };
template<class T> struct CScopedPointer { T* ptr; CScopedPointer() : ptr(0) { } };
struct CCompressedMemoryDataInputStream_Retail : CDataStream {
    unsigned char* Buffer; long BufferSize; CScopedPointer<CCompressorZlib> Compressor;
    virtual void v00() { }
    CCompressedMemoryDataInputStream_Retail() : CDataStream(), Buffer(0), BufferSize(0), Compressor() { }
};
int main()
{
    CCompressedMemoryDataInputStream_Retail object; CCompressedMemoryDataInputStream_Retail* s = &object;
    if (g_baseCtors != 1 || s->Buffer != 0 || s->BufferSize != 0 || s->Compressor.ptr != 0 || s->m_position != 0 || s->m_bufferEnd != 0
        || offsetof(CCompressedMemoryDataInputStream_Retail, Buffer) != 0x18 || offsetof(CCompressedMemoryDataInputStream_Retail, Compressor) != 0x20 || sizeof(*s) != 0x24) { std::printf("00995dc0_TEST FAIL\n"); return 1; }
    std::printf("00995dc0_TEST PASS\n"); return 0;
}
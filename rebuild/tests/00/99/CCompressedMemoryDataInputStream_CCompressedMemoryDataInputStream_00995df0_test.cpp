#include <cstdio>
#include <stddef.h>
static int g_arrayDeletes = 0, g_compressorDeletes = 0, g_baseDtors = 0; static void* g_lastArray = 0;
struct CByte { unsigned char b; static void operator delete[](void* p) { ++g_arrayDeletes; g_lastArray = p; ::operator delete(p); } };
class CCompressorZlib { public: virtual ~CCompressorZlib() { ++g_compressorDeletes; } };
struct CBaseClassNonCopyable { virtual void v00() { } ~CBaseClassNonCopyable() { ++g_baseDtors; } };
struct CDataStream : CBaseClassNonCopyable { unsigned char _state[0x14]; virtual void v00() { } ~CDataStream() { } };
template<class T> struct CScopedPointer { T* ptr; ~CScopedPointer() { if (ptr) delete ptr; } };
struct CCompressedMemoryDataInputStream_Retail : CDataStream {
    CByte* Buffer; long BufferSize; CScopedPointer<CCompressorZlib> Compressor;
    virtual void v00() { }
    ~CCompressedMemoryDataInputStream_Retail() { if (Buffer != 0) delete[] Buffer; }
};
int main()
{
    CByte* buffer = new CByte[16];
    {
        CCompressedMemoryDataInputStream_Retail s; s.Buffer = buffer; s.BufferSize = 16; s.Compressor.ptr = new CCompressorZlib;
    }
    if (g_arrayDeletes != 1 || g_lastArray != buffer || g_compressorDeletes != 1 || g_baseDtors != 1) { std::printf("00995df0_TEST FAIL\n"); return 1; }
    {
        CCompressedMemoryDataInputStream_Retail s; s.Buffer = 0; s.BufferSize = 0; s.Compressor.ptr = 0;
    }
    if (g_arrayDeletes != 1 || g_compressorDeletes != 1 || g_baseDtors != 2) { std::printf("00995df0_TEST FAIL\n"); return 1; }
    std::printf("00995df0_TEST PASS\n"); return 0;
}